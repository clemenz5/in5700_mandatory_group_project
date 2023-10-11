#include <omnetpp.h>

using namespace omnetpp;
#include <ComputerMessage_m.h>

class Computer: public cSimpleModule {
private:

protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

};

// The module class needs to be registered with OMNeT++
Define_Module(Computer);

void Computer::initialize() {
    send(new ComputerMsg("1- Contents of Book Table"), "gate$o", 0);
}

void Computer::handleMessage(cMessage *msg) {
    if (strcmp(msg->getName(), "10- Pay the Book") == 0) {
            send(new ComputerMsg("11- ACK"), "gate$o", 1);
            send(new ComputerMsg("12- Book payed"), "gate$o", 1);
            send(new ComputerMsg("14- Book table index"), "gate$o", 0);
        }
}

void Computer::finish() {
}
