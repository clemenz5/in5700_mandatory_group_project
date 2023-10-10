#include <string.h>
#include <omnetpp.h>
#include <string>
#include <computer_msg_m.h>

using namespace omnetpp;

/**
 * Derive the Txc1 class from cSimpleModule. In the Tictoc1 network,
 * both the `tic' and `toc' modules are Txc1 objects, created by OMNeT++
 * at the beginning of the simulation.
 */
class Client: public cSimpleModule {
private:
    ComputerMsg *lastMsg;
    simtime_t timeout;  // timeout
    cMessage *timeoutEvent;  // holds pointer to the timeout self-message
    bool message_sent;

protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

};

// The module class needs to be registered with OMNeT++
Define_Module(Client);

void Client::initialize() {
    message_sent = false;
    timeout = 1.0;
    timeoutEvent = new cMessage("timeoutEvent");

    lastMsg = new ComputerMsg("1: Hello");
    lastMsg->setId(1);
    lastMsg->setHeader("Hello");
    send(lastMsg, "gate$o");
    scheduleAt(simTime() + timeout, timeoutEvent);
}

void Client::handleMessage(cMessage *msg) {
    if (msg == timeoutEvent) {
        // If we receive the timeout event, that means the packet hasn't
        // arrived in time and we have to re-send it.
        EV << "Timeout expired, resending message and restarting timer\n";
        ComputerMsg *nMsg = new ComputerMsg("3: TestMsg");
        nMsg->setId(lastMsg->getId());
        nMsg->setHeader(lastMsg->getHeader());
        send(nMsg, "gate$o");
        scheduleAt(simTime() + timeout, timeoutEvent);
    } else {
        cancelEvent(timeoutEvent);
        ComputerMsg *cMsg = check_and_cast<ComputerMsg*>(msg);
        bubble(cMsg->getHeader());

        if (!message_sent) {
            ComputerMsg *rMsg = new ComputerMsg("3: TestMsg");
            rMsg->setId(cMsg->getId() + 1);
            rMsg->setHeader("TestMsg");
            lastMsg = rMsg;
            send(rMsg, "gate$o");
            message_sent = true;
            scheduleAt(simTime() + timeout, timeoutEvent);
        }

    }

}

void Client::finish() {
}
