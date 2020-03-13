#ifndef CANBUSINTERFACE_H
#define CANBUSINTERFACE_H

class CanBusInterface {
public:
    virtual ~CanBusInterface() {}

    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual void sendCanMessage() = 0;
    virtual void recievedDataFrame() = 0;

    // QUESTION: why is this not working
//    static CanBusInterface* getBusInterfaceForProvider(CanBusProvider provider) {
//        switch (provider) {
//        case kvaser:
//            return new KvaserWirelessInterface();
//        default:
//            return nullptr;
//        }
//    }
};


#endif // CANBUSINTERFACE_H
