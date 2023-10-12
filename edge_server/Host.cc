#include <omnetpp.h>
#include <ComputerMessage_m.h>

using namespace omnetpp;

class Host: public cSimpleModule {
private:
    int drop_count;

protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

};

// The module class needs to be registered with OMNeT++
Define_Module(Host);

void Host::initialize() {
    drop_count = 5;
    scheduleAt(simTime() + 38.0, new cMessage("browseBook"));
    scheduleAt(simTime() + 57.0, new cMessage("payBook"));
}

void Host::handleMessage(cMessage *msg) {
    if (strcmp(msg->getName(), "3- Computer ready to start") == 0
            && drop_count > 0) {
        this->getParentModule()->bubble("Message Lost");
        drop_count--;
    } else if (strcmp(msg->getName(), "3- Computer ready to start") == 0
            && drop_count <= 0) {
        send(new ComputerMsg("4- ACK"), "gate$o", 0);
        send(new ComputerMsg("5- Where is the book I am looking for?"),
                "gate$o", 0);
    } else if (strcmp(msg->getName(),
            "7- The book you are looking for is in the right-hand shelf") == 0
            || strcmp(msg->getName(),
                    "8- The book you are looking for is in the left-hand shelf")
                    == 0) {
        send(new ComputerMsg("9- ACK"), "gate$o", 0);
    } else if (strcmp(msg->getName(), "browseBook") == 0) {
        this->getParentModule()->bubble("Browse Book");
    } else if (strcmp(msg->getName(), "payBook") == 0) {
        send(new ComputerMsg("10- Pay the Book"), "gate$o", 0);
    } else if (strcmp(msg->getName(), "12- Book payed") == 0) {
        send(new ComputerMsg("13- ACK"), "gate$o", 0);
    }
}

void Host::finish() {
}
