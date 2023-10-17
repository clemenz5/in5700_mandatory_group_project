#include <omnetpp.h>
#include <ComputerMessage_m.h>

using namespace omnetpp;

class Cloud: public cSimpleModule {
private:
    bool waiting_for_ACK;
    bool left;
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
Define_Module(Cloud);

void Cloud::initialize() {
    left = par("left").boolValue();
    numSent = 0;
    numReceived = 0;

    sprintf(displayString, "sent:%d rcvd:%d", numSent, numReceived);
    this->getDisplayString().setTagArg("t", 0,
            displayString);
}

void Cloud::handleMessage(cMessage *msg) {
    if (!msg->isSelfMessage()) {
        numReceived++;
    }

    if (strcmp(msg->getName(), "1- Contents of Book Table") == 0) {
        send(new ComputerMsg("2- ACK"), "gate$o", 1);
        numSent++;
        scheduleAt(simTime() + 1.0, new cMessage("timeoutEvent"));
        send(new ComputerMsg("3- Cloud ready to start"), "gate$o", 0);
        numSent++;
        waiting_for_ACK = true;
    } else if (strcmp(msg->getName(), "timeoutEvent") == 0) {
        if (waiting_for_ACK) {
            scheduleAt(simTime() + 1.0, new cMessage("timeoutEvent"));
            send(new ComputerMsg("3- Cloud ready to start"), "gate$o", 0);
            numSent++;
        }
    } else if (strcmp(msg->getName(), "5- Where is the book I am looking for?")
            == 0) {
        waiting_for_ACK = false;
        if (left) {
            send(new ComputerMsg("6- ACK"), "gate$o", 0);
            numSent++;
            send(new ComputerMsg("7- The book you are looking for is in the left-hand shelf"), "gate$o", 0);
            numSent++;
        } else {
            send(new ComputerMsg("6- ACK"), "gate$o", 0);
            numSent++;
            send(new ComputerMsg("8- The book you are looking for is in the right-hand shelf"), "gate$o", 0);
            numSent++;
        }
    } else if (strcmp(msg->getName(), "14- Book table index") == 0) {
        send(new ComputerMsg("15- ACK"), "gate$o", 1);
        numSent++;
    }

    sprintf(displayString, "sent:%d rcvd:%d", numSent, numReceived);
    this->getDisplayString().setTagArg("t", 0,
            displayString);
}

void Cloud::finish() {
}
