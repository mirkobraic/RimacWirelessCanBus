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
    Q_UNUSED(baudRate); // its not possible to configure baude rate programatically on wiicom device
    quint16 portNumber = static_cast<quint16>(port.toULong());
    socket->connectToHost(deviceIpAddress, portNumber, QIODevice::ReadWrite, QAbstractSocket::IPv4Protocol);

    emit fetchingInProgress(true);
}

void WiicomWirelessCan::disconnectFromDevice()
{
    socket->disconnectFromHost();
    emit toggleConnection(false);
}

void WiicomWirelessCan::sendCanMessage(isotp::can_layer_message &message)
{
    QByteArray csvMsg = csvParser.convertMessage(message, message.id > maxStdCanId);
    socket->write(csvMsg);
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
    QVector<isotp::can_layer_message> messages = csvParser.parseInput(socket->readAll());
    for(isotp::can_layer_message msg : messages)  {
        messageRecievedUdsCallback(std::make_unique<isotp::can_layer_message>(msg));
        newDirectCanMessage(msg.id, msg.data);
    }
}

void WiicomWirelessCan::errorOccurred(QAbstractSocket::SocketError)
{
    emit fetchingInProgress(false);
    emit showAlert("Error", socket->errorString());
}

