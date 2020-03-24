#include "KvaserWirelessInterface.h"
#include <QTimer>

KvaserWirelessInterface::KvaserWirelessInterface()
{
    canInitializeLibrary();
}

KvaserWirelessInterface::~KvaserWirelessInterface()
{
    disconnect();
}

void KvaserWirelessInterface::connect(QString channelName, BaudRate baudRate)
{
    if (isConnected) {
        return;
    }
    qDebug() << "Number of channels: " << getChannelCount();

    //TODO: implement channelName and baudRate
    channelNumber = 0;

    handle = canOpenChannel(channelNumber, canOPEN_ACCEPT_VIRTUAL);
    canStatus status = (canStatus)handle;
    checkStatus("canOpenChannel", status);

    status = canSetBusParams(handle, canBITRATE_125K, 0, 0, 0, 0, 0);
    checkStatus("canSetBusParams", status);

    status = canBusOn(handle);
    checkStatus("canBusOn", status);

    isConnected = true;
    shouldListen = true;

    //startListening();
    // TODO: make separete thread for listening
    QTimer *updateTimer = new QTimer(this);
    QObject::connect(updateTimer, &QTimer::timeout, this, &KvaserWirelessInterface::startListening);
    updateTimer->start(100);
}

void KvaserWirelessInterface::disconnect()
{
    if (isConnected) {
        status = canBusOff(handle);
        checkStatus("canBusOff", status);

        isConnected = false;
        shouldListen = false;

        status = canClose(handle);
        checkStatus("canClose", status);
        handle = canINVALID_HANDLE;
    }
}

void KvaserWirelessInterface::sendCanMessage(CanMessage message)
{
    char *data = message.getData().data();
    status = canWriteWait(handle, message.getId(), data, message.getDlc(), 0, 100);
    checkStatus("canWriteWait", status);
}

void KvaserWirelessInterface::startListening()
{
    canStatus stat = canOK;
    long id;
    unsigned int dlc, flags;
    char data[8];
    DWORD timestamp;

    if(shouldListen) {
        stat = canReadWait(handle, &id, data, &dlc, &flags, &timestamp, 100);

        if(stat == canOK) {
            if (flags & canMSG_ERROR_FRAME){
                qDebug() << "***ERROR FRAME RECEIVED***";
            } else {
                try {
                    CanMessage message((uint)id, (quint8)dlc, QByteArray::fromRawData(data, dlc));
                    emit newDataFrame(message);
                } catch (CanMessageException error) {
                    qDebug() << "failed to make message: " << error;
                }
            }

        } else if (stat != canERR_NOMSG){
            checkStatus("canReadWait", stat);
        }
    }
}

int KvaserWirelessInterface::getChannelCount()
{
    int count = 0;
    canStatus stat = canGetNumberOfChannels(&count);
    checkStatus("canGetNumberOfChannels", stat);
    return count;
}

void KvaserWirelessInterface::checkStatus(QString method, canStatus status)
{
    //TODO: refactor throw
    if (status != canOK) {
        char buffer[50];
        buffer[0] = '\0';
        canGetErrorText(status, buffer, sizeof(buffer));
        QString errorMsg = method + " failed with status: " + status + " " + buffer;
        qDebug() << errorMsg;
        throw errorMsg;
    }
}

int KvaserWirelessInterface::getBaudRate(BaudRate baudRate)
{
    switch (baudRate)
}
