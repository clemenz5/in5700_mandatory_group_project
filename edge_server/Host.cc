#include <omnetpp.h>
#include <ComputerMessage_m.h>

using namespace omnetpp;

class Host: public cSimpleModule {
private:
    int numDropped;
    bool left;
    char displayString[20];
    int numSentCloud;
    int numSentComp;
    int numReceivedCloud;
    int numReceivedComp;
    int sendMessagePower[2];
    int receiveMessagePower[2];
    int sendMessageDelay[2];
    int receiveMessageDelay[2];
    cLabelFigure* total_num_smartphone;
    cLabelFigure* total_power_smart_to_cloud;
    cLabelFigure* total_power_smart_to_comp;
    cLabelFigure* total_power_rcvd_smart_to_cloud;
    cLabelFigure* total_power_rcvd_smart_to_comp;
    cLabelFigure* total_delay_smart_to_cloud;
    cLabelFigure* total_delay_smart_to_comp;
    cLabelFigure* total_delay_rcvd_smart_to_cloud;
    cLabelFigure* total_delay_rcvd_smart_to_comp;

protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    virtual void send(cMessage *msg, const char *gatename, int gateindex=-1) override;
    void updateLabels();

};

// The module class needs to be registered with OMNeT++
Define_Module(Host);

void Host::initialize() {
    numDropped = 0;
    numSentComp = 0;
    numSentCloud = 0;
    numReceivedComp = 0;
    numReceivedCloud = 0;

    sendMessagePower[0] = par("sendMessagePowerCloud").intValue();
    sendMessagePower[1] = par("sendMessagePowerComp").intValue();

    receiveMessagePower[0] = par("receiveMessagePowerCloud").intValue();
    receiveMessagePower[1] = par("receiveMessagePowerComp").intValue();

    sendMessageDelay[0] = par("sendMessageDelayCloud").intValue();
    sendMessageDelay[1] = par("sendMessageDelayComp").intValue();

    receiveMessageDelay[0] = par("receiveMessageDelayCloud").intValue();
    receiveMessageDelay[1] = par("receiveMessageDelayComp").intValue();

    left = this->getParentModule()->par("left").boolValue();
    scheduleAt(simTime() + 38.0, new cMessage("browseBook"));
    scheduleAt(simTime() + 57.0, new cMessage("payBook"));

    cCanvas* canvas = this->getParentModule()->getParentModule()->getCanvas();
    total_num_smartphone = (cLabelFigure*)(canvas->getFigure("total_num_smartphone"));

    total_power_smart_to_cloud= (cLabelFigure*)(canvas->getFigure("total_power_smart_to_cloud"));
    total_power_smart_to_comp= (cLabelFigure*)(canvas->getFigure("total_power_smart_to_comp"));
    total_power_rcvd_smart_to_cloud= (cLabelFigure*)(canvas->getFigure("total_power_rcvd_smart_to_cloud"));
    total_power_rcvd_smart_to_comp= (cLabelFigure*)(canvas->getFigure("total_power_rcvd_smart_to_comp"));

    total_delay_smart_to_cloud= (cLabelFigure*)(canvas->getFigure("total_delay_smart_to_cloud"));
    total_delay_smart_to_comp= (cLabelFigure*)(canvas->getFigure("total_delay_smart_to_comp"));
    total_delay_rcvd_smart_to_cloud= (cLabelFigure*)(canvas->getFigure("total_delay_rcvd_smart_to_cloud"));
    total_delay_rcvd_smart_to_comp= (cLabelFigure*)(canvas->getFigure("total_delay_rcvd_smart_to_comp"));
    updateLabels();
}

void Host::handleMessage(cMessage *msg) {
    if (!msg->isSelfMessage()) {
        if(msg->getArrivalGateId()==1048576){
            numReceivedComp++;
        } else {
            numReceivedCloud++;
        }
    }
    if (strcmp(msg->getName(), "3- Computer ready to start") == 0
            && numDropped < 5) {
        this->getParentModule()->bubble("Message Lost");
        numDropped++;
    } else if (strcmp(msg->getName(), "3- Computer ready to start") == 0
            && numDropped >= 5) {
        send(new ComputerMsg("4- ACK"), "gate$o", 0);
        send(new ComputerMsg("5- Where is the book I am looking for?"),
                "gate$o", 0);
    } else if (strcmp(msg->getName(),
            "7- The book you are looking for is in the left-hand shelf") == 0
            || strcmp(msg->getName(),
                    "8- The book you are looking for is in the right-hand shelf")
                    == 0) {
        send(new ComputerMsg("9- ACK"), "gate$o", 0);
    } else if (strcmp(msg->getName(), "browseBook") == 0) {
        this->getParentModule()->bubble("Browse Book");
    } else if (strcmp(msg->getName(), "payBook") == 0) {
        send(new ComputerMsg("10- Pay the Book"), "gate$o", 0);
    } else if (strcmp(msg->getName(), "12- Book payed") == 0) {
        send(new ComputerMsg("13- ACK"), "gate$o", 0);
    }
    updateLabels();
}

void Host::updateLabels(){
    sprintf(displayString, "sent:%d rcvd:%d lost:%d", numSentComp+numSentCloud, numReceivedComp+numReceivedCloud-numDropped, numDropped);
    this->getParentModule()->getDisplayString().setTagArg("t", 0,
            displayString);

    char temp[80];
    sprintf(temp, "Total number of messages sent/received by the smartphone= %d", numSentComp+numSentCloud+numReceivedComp+numReceivedCloud-numDropped);
    total_num_smartphone->setText(temp);
    sprintf(temp, "smartphone (from smartphone to cloud)= %d", numSentCloud*sendMessagePower[0]);
    total_power_smart_to_cloud->setText(temp);
    sprintf(temp, "smartphone (from smartphone to comp)= %d", numSentComp*sendMessagePower[1]);
    total_power_smart_to_comp->setText(temp);

    sprintf(temp, "smartphone (from smartphone to cloud)= %d", numReceivedCloud*receiveMessagePower[0]);
    total_power_rcvd_smart_to_cloud->setText(temp);
    sprintf(temp, "smartphone (from smartphone to comp)= %d", (numReceivedComp-numDropped)*receiveMessagePower[1]);
    total_power_rcvd_smart_to_comp->setText(temp);

    sprintf(temp, "smartphone (from smartphone to cloud)= %d", numSentCloud * sendMessageDelay[0]);
    total_delay_smart_to_cloud->setText(temp);
    sprintf(temp, "smartphone (from smartphone to comp)= %d", numSentComp * sendMessageDelay[1]);
    total_delay_smart_to_comp->setText(temp);

    sprintf(temp, "smartphone (from smartphone to cloud)= %d", numReceivedCloud * receiveMessageDelay[0]);
    total_delay_rcvd_smart_to_cloud->setText(temp);
    sprintf(temp, "smartphone (from smartphone to comp)= %d", numReceivedComp * receiveMessageDelay[1]);
    total_delay_rcvd_smart_to_comp->setText(temp);
}

void Host::send(cMessage *msg, const char *gatename, int gateindex){
    cSimpleModule::send(msg, gatename, gateindex);
    if(gateindex==0){
        numSentComp++;
    } else {
        numSentCloud++;
    }
    updateLabels();
}

void Host::finish() {
}
