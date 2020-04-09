#include "KvaserWirelessCan.h"

KvaserWirelessCan::KvaserWirelessCan()
{
    canInitializeLibrary();
}

KvaserWirelessCan::~KvaserWirelessCan()
{
    disconnect();
}

void KvaserWirelessCan::connect(QString channelName, BaudRate baudRate)
{
    if (isConnected) {
        return;
    }
//    kvrGetVersion();
    qDebug() << "Number of channels: " << getChannelCount();

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

void KvaserWirelessCan::sendCanMessage(CanMessage message)
{
    char *data = reinterpret_cast<char*>(message.getData().data());
    unsigned int flag = message.isExtended() ? canMSG_EXT : canMSG_STD;
    txStatus = canWriteWait(txHandle, message.getId(), data, message.getDlc(), flag, 100);
    checkStatus("canWriteWait", txStatus);
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
    DWORD timestamp;

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
                try {
                    unsigned int arraySize = sizeof(data) / sizeof(char);
                    if (dlc > arraySize) {
                        dlc = arraySize;
                    }
                    std::vector<uint8_t> vecData = std::vector<uint8_t>(data, data + dlc);
                    CanMessage message((uint32_t)id, (uint8_t)dlc, vecData);
                    emit newDataFrame(message);
                } catch (const std::exception& ex) {
                    qDebug() << "Exception: " << ex.what();
                }

                // ISOTP
//                isotp::can_layer_message msg;
//                msg.id = id;
//                unsigned int arraySize = sizeof(data) / sizeof(char);
//                // assure that dlc is not greater than array size
//                if (dlc > arraySize) {
//                    dlc = arraySize;
//                }
//                msg.data = std::vector<uint8_t>(data, data + dlc);
//                recievedMessageCallback(std::make_unique<isotp::can_layer_message>(msg));
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
        QString errorMsg = method + " failed with status: " + QString::number(status) + " " + buffer;
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
