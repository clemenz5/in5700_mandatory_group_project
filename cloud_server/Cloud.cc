#include <omnetpp.h>

using namespace omnetpp;


class Cloud: public cSimpleModule {
private:

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

}

void Cloud::finish() {
}
