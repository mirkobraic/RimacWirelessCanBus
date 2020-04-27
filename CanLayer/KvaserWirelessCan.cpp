#include "KvaserWirelessCan.h"

KvaserWirelessCan::KvaserWirelessCan()
{
    canInitializeLibrary();
}

KvaserWirelessCan::~KvaserWirelessCan()
{
    disconnect();
}

void KvaserWirelessCan::connect(std::string channelName, BaudRate baudRate)
{
    if (isConnected) {
        return;
    }

    qDebug() << "--CanLayer: number of channels: " << getChannelCount();

    //TODO: implement channelName
    channelNumber = 0;

    txHandle = canOpenChannel(channelNumber, canOPEN_ACCEPT_VIRTUAL);
    if (txHandle < 0){
        txStatus = (canStatus)txHandle;
        checkStatus("canOpenChannel", txStatus);
    }

    txStatus = canSetBusParams(txHandle, getBaudRate(baudRate), 0, 0, 0, 0, 0);
    checkStatus("canSetBusParams", txStatus);

    txStatus = canBusOn(txHandle);
    checkStatus("canBusOn", txStatus);

    isConnected = true;
    shouldListen = true;

    // start rx thread
    QtConcurrent::run(this, &KvaserWirelessCan::startListening);
}

void KvaserWirelessCan::disconnect()
{
    if (isConnected) {        
        isConnected = false;
        shouldListen = false;

        txStatus = canBusOff(txHandle);
        checkStatus("canBusOff", txStatus);

        txStatus = canClose(txHandle);
        checkStatus("canClose", txStatus);
        txHandle = canINVALID_HANDLE;
    }
}

void KvaserWirelessCan::sendCanMessage(isotp::can_layer_message &message)
{
    char *data = reinterpret_cast<char*>(message.data.data());
    unsigned int flag = message.id > maxStdCanId ? canMSG_EXT : canMSG_STD;

    canFlushTransmitQueue(txHandle);
    txStatus = canWrite(txHandle, message.id, data, message.data.size(), flag);
    checkStatus("canWrite", txStatus);
    qDebug() << "--CanLayer: writing ID =" << message.id << " data =" << message.data;

//    txStatus = canWriteSync(txHandle, 100);
//    checkStatus("canWriteSync", txStatus);
}

void KvaserWirelessCan::startListening()
{
    // different handle is reqired for running on a different thread
    canHandle rxHandle = canINVALID_HANDLE;
    canStatus rxStatus = canOK;

    rxHandle = canOpenChannel(channelNumber, canOPEN_ACCEPT_VIRTUAL);
    if (rxHandle < 0){
        rxStatus = (canStatus)rxHandle;
        checkStatus("canOpenChannel", rxStatus);
    }

    // TODO: implement this for final version, commented out because it interfers with virtual can
    // stops messages sent from another thread from being recieved in this thread
//    unsigned char buf = 0x00;
//    rxStatus = canIoCtl(rxHandle, canIOCTL_SET_LOCAL_TXECHO, &buf, sizeof(buf));
//    checkStatus("canIoCtl", rxStatus);

    rxStatus = canBusOn(rxHandle);
    checkStatus("canBusOn", rxStatus);

    long id;
    unsigned int dlc, flags;
    char data[8];
    unsigned long timestamp;

    while (shouldListen) {
        rxStatus = canReadWait(rxHandle, &id, data, &dlc, &flags, &timestamp, 100);
        if (rxStatus == canERR_NOMSG){
            // QUESTION: should we add sleep?
            continue;
        }

        if (rxStatus == canOK) {
            if (flags & canMSG_ERROR_FRAME){
                qDebug() << "***ERROR FRAME RECEIVED***";
            } else {
                isotp::can_layer_message msg;
                msg.id = id;
                unsigned int arraySize = sizeof(data) / sizeof(char);
                // assure that dlc is not greater than array size
                if (dlc > arraySize) {
                    dlc = arraySize;
                }
                msg.data = std::vector<uint8_t>(data, data + dlc);
                qDebug() << "--CanLayer: recieved ID =" << id << " data =" << msg.data;
                recievedMessageCallback(std::make_unique<isotp::can_layer_message>(msg));
            }
        } else {
            checkStatus("canReadWait", rxStatus);
        }
    }

    rxStatus = canBusOff(rxHandle);
    checkStatus("canBusOff", rxStatus);

    rxStatus = canClose(rxHandle);
    checkStatus("canClose", rxStatus);
    rxHandle = canINVALID_HANDLE;
}

int KvaserWirelessCan::getChannelCount()
{
    int count = 0;
    canStatus stat = canGetNumberOfChannels(&count);
    checkStatus("canGetNumberOfChannels", stat);
    return count;
}

void KvaserWirelessCan::checkStatus(QString method, canStatus status)
{
    if (status != canOK) {
        char buffer[50];
        buffer[0] = '\0';
        canGetErrorText(status, buffer, sizeof(buffer));
        QString errorMsg = "CanLayer: " + method + " failed with status: " + QString::number(status) + " " + buffer;
        qDebug() << errorMsg;
    }
}

int KvaserWirelessCan::getBaudRate(BaudRate baudRate)
{
    switch (baudRate){
    case Baud_125:
        return canBITRATE_125K;
    case Baud_250:
        return canBITRATE_250K;
    case Baud_500:
        return canBITRATE_500K;
    case Baud_1000:
        return canBITRATE_1M;
    default:
        return canBITRATE_500K;
    }
}
