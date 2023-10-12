#include <omnetpp.h>
#include <ComputerMessage_m.h>

using namespace omnetpp;

class Computer: public cSimpleModule {
private:
    bool waiting_for_ACK;
    bool left;

protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

};

// The module class needs to be registered with OMNeT++
Define_Module(Computer);

void Computer::initialize() {
    left = par("left").boolValue();
    send(new ComputerMsg("1- Contents of Book Table"), "gate$o", 1);
}

void Computer::handleMessage(cMessage *msg) {
    if (strcmp(msg->getName(), "2- ACK") == 0) {
        scheduleAt(simTime() + 1.0, new cMessage("timeoutEvent"));
        send(new ComputerMsg("3- Computer ready to start"), "gate$o", 0);
        waiting_for_ACK = true;
    } else if (strcmp(msg->getName(), "timeoutEvent") == 0) {
        if (waiting_for_ACK) {
            scheduleAt(simTime() + 1.0, new cMessage("timeoutEvent"));
            send(new ComputerMsg("3- Computer ready to start"), "gate$o", 0);
        }
    } else if (strcmp(msg->getName(), "5- Where is the book I am looking for?")
            == 0) {
        waiting_for_ACK = false;
        if (left) {
            send(new ComputerMsg("6- ACK"), "gate$o", 0);
            send(new ComputerMsg("7- The book you are looking for is in the left-hand shelf"), "gate$o", 0);
        } else {
            send(new ComputerMsg("6- ACK"), "gate$o", 0);
            send(new ComputerMsg("8- The book you are looking for is in the right-hand shelf"), "gate$o", 0);
        }
    } else if (strcmp(msg->getName(), "10- Pay the Book") == 0) {
        send(new ComputerMsg("11- ACK"), "gate$o", 0);
        send(new ComputerMsg("12- Book payed"), "gate$o", 0);
        send(new ComputerMsg("14- Book table index"), "gate$o", 1);
    }


}

void Computer::finish() {
}
