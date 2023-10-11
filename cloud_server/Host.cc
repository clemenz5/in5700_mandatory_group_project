#include <omnetpp.h>

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
}

void Host::handleMessage(cMessage *msg) {
    bubble("Message Lost");
    if (strcmp(msg->getName(), "3- Cloud ready to start") == 0
            && drop_count > 0) {
        bubble("Message Lost");
    }
}

void Host::finish() {
}
