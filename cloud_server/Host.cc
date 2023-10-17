#include <omnetpp.h>
#include <ComputerMessage_m.h>

using namespace omnetpp;

class Host: public cSimpleModule {
private:
    int numDropped;
    bool left;
    char displayString[20];
    int numSent;
    int numReceived;
    cLabelFigure* total_num_smartphone;


protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    void updateLabels();
};

// The module class needs to be registered with OMNeT++
Define_Module(Host);

void Host::initialize() {
    numDropped = 0;
    numSent = 0;
    numReceived = 0;
    left = this->getParentModule()->par("left").boolValue();
    scheduleAt(simTime() + 38.0, new cMessage("browseBook"));
    scheduleAt(simTime() + 57.0, new cMessage("payBook"));
    EV
              << this->getParentModule()->getParentModule()->getCanvas()->getFigure(
                      "total_num_smartphone")->getFullName() << "\n";
    total_num_smartphone = (cLabelFigure*)(this->getParentModule()->getParentModule()->getCanvas()->getFigure("total_num_smartphone"));

    updateLabels();

}

void Host::handleMessage(cMessage *msg) {
    if (!msg->isSelfMessage()) {
        numReceived++;
    }

    if (strcmp(msg->getName(), "3- Cloud ready to start") == 0
            && numDropped < 5) {
        this->getParentModule()->bubble("Message Lost");
        numDropped++;
    } else if (strcmp(msg->getName(), "3- Cloud ready to start") == 0
            && numDropped >= 5) {
        send(new ComputerMsg("4- ACK"), "gate$o", 0);
        numSent++;
        send(new ComputerMsg("5- Where is the book I am looking for?"),
                "gate$o", 0);
        numSent++;
    } else if (strcmp(msg->getName(),
            "7- The book you are looking for is in the left-hand shelf") == 0
            || strcmp(msg->getName(),
                    "8- The book you are looking for is in the right-hand shelf")
                    == 0) {
        send(new ComputerMsg("9- ACK"), "gate$o", 0);
        numSent++;
    } else if (strcmp(msg->getName(), "browseBook") == 0) {
        this->getParentModule()->bubble("Browse Book");
    } else if (strcmp(msg->getName(), "payBook") == 0) {
        send(new ComputerMsg("10- Pay the Book"), "gate$o", 1);
        numSent++;
    } else if (strcmp(msg->getName(), "12- Book payed") == 0) {
        send(new ComputerMsg("13- ACK"), "gate$o", 1);
        numSent++;
    }

    updateLabels();
}

void Host::updateLabels(){
    sprintf(displayString, "sent:%d rcvd:%d lost:%d", numSent, numReceived-numDropped,
            numDropped);
    this->getParentModule()->getDisplayString().setTagArg("t", 0,
            displayString);

    char temp[80];
    sprintf(temp, "Total number of messages sent/received by the smartphone= %d", numSent+numReceived-numDropped);
    total_num_smartphone->setText(temp);
}

void Host::finish() {
    updateLabels();
}
