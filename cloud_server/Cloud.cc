#include <omnetpp.h>
#include <ComputerMessage_m.h>
#include <string>

using namespace omnetpp;


class Cloud: public cSimpleModule {
private:
    bool waiting_for_ACK;

protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

};

// The module class needs to be registered with OMNeT++
Define_Module(Cloud);

void Cloud::initialize() {

}

void Cloud::handleMessage(cMessage *msg) {

    if(strcmp(msg->getName(), "1- Contents of Book Table") == 0){
        send(new ComputerMsg("2- ACK"), "gate$o", 1);
        scheduleAt(simTime() + 1.0, new cMessage("timeoutEvent"));
        send(new ComputerMsg("3- Cloud ready to start"), "gate$o", 0);
        waiting_for_ACK = true;
    } else if (strcmp(msg->getName(), "timeoutEvent") == 0){
        if(waiting_for_ACK){
            scheduleAt(simTime() + 1.0, new cMessage("timeoutEvent"));
            send(new ComputerMsg("3- Cloud ready to start"), "gate$o", 0);
        }
    } else if (strcmp(msg->getName(), "4- ACK") == 0){
        bubble("nice");
    }

}

void Cloud::finish() {
}
