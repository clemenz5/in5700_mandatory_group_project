#include <omnetpp.h>

using namespace omnetpp;
#include <ComputerMessage_m.h>

class Computer: public cSimpleModule {
private:
    char displayString[20];
    int numSent;
    int numReceived;

protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

};

// The module class needs to be registered with OMNeT++
Define_Module(Computer);

void Computer::initialize() {
    numSent = 0;
    numReceived = 0;
    sprintf(displayString, "sent:%d rcvd:%d", numSent, numReceived);
    this->getDisplayString().setTagArg("t", 0,
            displayString);
    send(new ComputerMsg("1- Contents of Book Table"), "gate$o", 0);
    numSent++;
    sprintf(displayString, "sent:%d rcvd:%d", numSent, numReceived);
    this->getDisplayString().setTagArg("t", 0,
            displayString);
}

void Computer::handleMessage(cMessage *msg) {
    if (!msg->isSelfMessage()) {
        numReceived++;
    }
    if (strcmp(msg->getName(), "10- Pay the Book") == 0) {
        send(new ComputerMsg("11- ACK"), "gate$o", 1);
        numSent++;
        send(new ComputerMsg("12- Book payed"), "gate$o", 1);
        numSent++;
        send(new ComputerMsg("14- Book table index"), "gate$o", 0);
        numSent++;
    }
    sprintf(displayString, "sent:%d rcvd:%d", numSent, numReceived);
    this->getDisplayString().setTagArg("t", 0,
            displayString);
}

void Computer::finish() {
}
