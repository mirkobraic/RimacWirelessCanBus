#include "WiicomWirelessCan.h"

WiicomWirelessCan::WiicomWirelessCan()
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)),this, SLOT(errorOccurred(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
}

WiicomWirelessCan::~WiicomWirelessCan()
{
    disconnectFromDevice();
    delete socket;
}

void WiicomWirelessCan::connectToDevice(QString deviceIpAddress, QString port, BaudRate baudRate)
{
    Q_UNUSED(baudRate); // it's not possible to configure baude rate programatically on wiicom device
    quint16 portNumber = static_cast<quint16>(port.toULong());
    emit fetchingInProgress(true);
    socket->connectToHost(deviceIpAddress, portNumber, QIODevice::ReadWrite, QAbstractSocket::IPv4Protocol);
}

void WiicomWirelessCan::disconnectFromDevice()
{
    socket->disconnectFromHost();
    emit toggleConnection(false);
}

void WiicomWirelessCan::sendCanMessage(CanMessage &message)
{
    bool isExtended = message.getId() > maxStdCanId;
    QByteArray csvMsg = csvParser.convertMessage(message, isExtended);
    qDebug() << "CanLayer: Sending " << csvMsg;
    dispatchToMainThread([this, csvMsg] {
        socket->write(csvMsg);
    });
}

void WiicomWirelessCan::connected()
{
    emit toggleConnection(true);
    emit fetchingInProgress(false);
}

void WiicomWirelessCan::disconnected()
{
    emit toggleConnection(false);
}

void WiicomWirelessCan::readyRead()
{
    QByteArray input = socket->readAll();

    }
}

void WiicomWirelessCan::errorOccurred(QAbstractSocket::SocketError)
{
    emit showAlert("Error", socket->errorString());
    emit fetchingInProgress(false);
}

