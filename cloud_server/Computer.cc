#include <omnetpp.h>

using namespace omnetpp;
#include <ComputerMessage_m.h>

class Computer : public cSimpleModule {
   private:
    char displayString[20];
    int numSentCloud;
    int numSentSmart;
    int numReceivedCloud;
    int numReceivedSmart;
    int sendMessagePower[2];
    int receiveMessagePower[2];
    int messageDelay;
    cLabelFigure* total_num_comp;

    cLabelFigure* total_power_comp_to_cloud;
    cLabelFigure* total_power_comp_to_smart;
    cLabelFigure* total_power_rcvd_comp_to_smart;
    cLabelFigure* total_power_rcvd_comp_to_cloud;
    cLabelFigure* total_delay_comp_to_smart;
    cLabelFigure* total_delay_comp_to_cloud;
    cLabelFigure* total_delay_rcvd_comp_to_smart;
    cLabelFigure* total_delay_rcvd_comp_to_cloud;

   protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage* msg) override;
    virtual void finish() override;
    virtual void send(cMessage* msg, const char* gatename, int gateindex = -1) override;
    void updateLabels();
};

// The module class needs to be registered with OMNeT++
Define_Module(Computer);

void Computer::initialize() {
    numSentSmart = 0;
    numSentCloud = 0;
    numReceivedSmart = 0;
    numReceivedCloud = 0;

    sendMessagePower[0] = par("sendMessagePowerCloud").intValue();
    sendMessagePower[1] = par("sendMessagePowerSmart").intValue();

    receiveMessagePower[0] = par("receiveMessagePowerSmart").intValue();
    receiveMessagePower[1] = par("receiveMessagePowerCloud").intValue();

    messageDelay = par("messageDelay").intValue();

    cCanvas* canvas = this->getParentModule()->getCanvas();

    total_num_comp = (cLabelFigure*)(canvas->getFigure("total_num_comp"));

    total_power_comp_to_cloud = (cLabelFigure*)(canvas->getFigure("total_power_comp_to_cloud"));
    total_power_comp_to_smart = (cLabelFigure*)(canvas->getFigure("total_power_comp_to_smart"));
    total_power_rcvd_comp_to_smart = (cLabelFigure*)(canvas->getFigure("total_power_rcvd_comp_to_smart"));
    total_power_rcvd_comp_to_cloud = (cLabelFigure*)(canvas->getFigure("total_power_rcvd_comp_to_cloud"));

    total_delay_comp_to_smart = (cLabelFigure*)(canvas->getFigure("total_delay_comp_to_smart"));
    total_delay_comp_to_cloud = (cLabelFigure*)(canvas->getFigure("total_delay_comp_to_cloud"));
    total_delay_rcvd_comp_to_smart = (cLabelFigure*)(canvas->getFigure("total_delay_rcvd_comp_to_smart"));
    total_delay_rcvd_comp_to_cloud = (cLabelFigure*)(canvas->getFigure("total_delay_rcvd_comp_to_cloud"));
    send(new ComputerMsg("1- Contents of Book Table"), "gate$o", 0);
}

void Computer::handleMessage(cMessage* msg) {
    if (!msg->isSelfMessage()) {
        EV << msg->getArrivalGateId();
        if (msg->getArrivalGateId() == 1048576) {
            numReceivedCloud++;
        } else {
            numReceivedSmart++;
        }
        updateLabels();
    }
    if (strcmp(msg->getName(), "10- Pay the Book") == 0) {
        send(new ComputerMsg("11- ACK"), "gate$o", 1);
        send(new ComputerMsg("12- Book payed"), "gate$o", 1);
        send(new ComputerMsg("14- Book table index"), "gate$o", 0);
    }
}

void Computer::updateLabels() {
    sprintf(displayString, "sent:%d rcvd:%d", numSentSmart + numSentCloud, numReceivedSmart + numReceivedCloud);
    this->getDisplayString().setTagArg("t", 0,
                                       displayString);

    char temp[80];
    sprintf(temp, "Total number of messages sent/received by the computer= %d", numSentSmart + numSentCloud + numReceivedSmart + numReceivedCloud);
    total_num_comp->setText(temp);

    sprintf(temp, "computer (from computer to cloud)= %d", numSentCloud * sendMessagePower[0]);
    total_power_comp_to_cloud->setText(temp);
    sprintf(temp, "computer (from computer to smartphone)= %d", numSentSmart * sendMessagePower[1]);
    total_power_comp_to_smart->setText(temp);

    sprintf(temp, "computer (from computer to smartphone)= %d", numReceivedSmart * receiveMessagePower[0]);
    total_power_rcvd_comp_to_smart->setText(temp);
    sprintf(temp, "computer (from computer to cloud)= %d", numReceivedCloud * receiveMessagePower[1]);
    total_power_rcvd_comp_to_cloud->setText(temp);

    sprintf(temp, "computer (from computer to smartphone)= %d", numReceivedSmart * messageDelay);
    total_delay_comp_to_smart->setText(temp);
    sprintf(temp, "computer (from computer to cloud)= %d", numReceivedCloud * messageDelay);
    total_delay_comp_to_cloud->setText(temp);

    sprintf(temp, "computer (from computer to smartphone)= %d", numReceivedSmart * messageDelay);
    total_delay_rcvd_comp_to_smart->setText(temp);
    sprintf(temp, "computer (from computer to cloud)= %d", numReceivedCloud * messageDelay);
    total_delay_rcvd_comp_to_cloud->setText(temp);
}

void Computer::send(cMessage* msg, const char* gatename, int gateindex) {
    cSimpleModule::send(msg, gatename, gateindex);
    if (gateindex == 0) {
        numSentCloud++;
    } else {
        numSentSmart++;
    }
    updateLabels();
}

void Computer::finish() {
}
