#include <omnetpp.h>
#include <ComputerMessage_m.h>

using namespace omnetpp;

class Computer: public cSimpleModule {
private:
    bool waiting_for_ACK;
    bool left;
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
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    virtual void send(cMessage *msg, const char *gatename, int gateindex=-1) override;
    void updateLabels();

};

// The module class needs to be registered with OMNeT++
Define_Module(Computer);

void Computer::initialize() {
    left = par("left").boolValue();
    numSentSmart = 0;
    numSentCloud = 0;
    numReceivedSmart = 0;
    numReceivedCloud = 0;

    sendMessagePower[0] = 312;
    sendMessagePower[1] = 243;

    receiveMessagePower[0] = 247;
    receiveMessagePower[1] = 454;


    messageDelay = 154;

    cCanvas* canvas = this->getParentModule()->getCanvas();

    total_num_comp= (cLabelFigure*)(canvas->getFigure("total_num_comp"));

    total_power_comp_to_cloud= (cLabelFigure*)(canvas->getFigure("total_power_comp_to_cloud"));
    total_power_comp_to_smart= (cLabelFigure*)(canvas->getFigure("total_power_comp_to_smart"));
    total_power_rcvd_comp_to_smart= (cLabelFigure*)(canvas->getFigure("total_power_rcvd_comp_to_smart"));
    total_power_rcvd_comp_to_cloud= (cLabelFigure*)(canvas->getFigure("total_power_rcvd_comp_to_cloud"));

    total_delay_comp_to_smart= (cLabelFigure*)(canvas->getFigure("total_delay_comp_to_smart"));
    total_delay_comp_to_cloud= (cLabelFigure*)(canvas->getFigure("total_delay_comp_to_cloud"));
    total_delay_rcvd_comp_to_smart= (cLabelFigure*)(canvas->getFigure("total_delay_rcvd_comp_to_smart"));
    total_delay_rcvd_comp_to_cloud= (cLabelFigure*)(canvas->getFigure("total_delay_rcvd_comp_to_cloud"));
    send(new ComputerMsg("1- Contents of Book Table"), "gate$o", 1);
}

void Computer::handleMessage(cMessage *msg) {
        if (!msg->isSelfMessage()) {
        EV << msg->getArrivalGateId();
        if(msg->getArrivalGateId()==1048576){
            numReceivedSmart++;
        } else {
            numReceivedCloud++;
        }
        updateLabels();
    }
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

void Computer::updateLabels(){
    sprintf(displayString, "sent:%d rcvd:%d", numSentSmart+numSentCloud, numReceivedSmart+numReceivedCloud);
    this->getDisplayString().setTagArg("t", 0,
            displayString);

    char temp[80];
    sprintf(temp, "Total number of messages sent/received by the computer= %d", numSentSmart+numSentCloud+numReceivedSmart+numReceivedCloud);
    total_num_comp->setText(temp);

    sprintf(temp, "computer (from computer to cloud)= %d", numSentCloud*sendMessagePower[0]);
    total_power_comp_to_cloud->setText(temp);
    sprintf(temp, "computer (from computer to smartphone)= %d", numSentSmart*sendMessagePower[1]);
    total_power_comp_to_smart->setText(temp);

    sprintf(temp, "computer (from computer to smartphone)= %d", numReceivedSmart*receiveMessagePower[0]);
    total_power_rcvd_comp_to_smart->setText(temp);
    sprintf(temp, "computer (from computer to cloud)= %d", numReceivedCloud*receiveMessagePower[1]);
    total_power_rcvd_comp_to_cloud->setText(temp);

    sprintf(temp, "computer (from computer to smartphone)= %d", numReceivedSmart*messageDelay);
    total_delay_comp_to_smart->setText(temp);
    sprintf(temp, "computer (from computer to cloud)= %d", numReceivedCloud*messageDelay);
    total_delay_comp_to_cloud->setText(temp);

    sprintf(temp, "computer (from computer to smartphone)= %d", numReceivedSmart*messageDelay);
    total_delay_rcvd_comp_to_smart->setText(temp);
    sprintf(temp, "computer (from computer to cloud)= %d", numReceivedCloud*messageDelay);
    total_delay_rcvd_comp_to_cloud->setText(temp);
}

void Computer::send(cMessage *msg, const char *gatename, int gateindex){
    cSimpleModule::send(msg, gatename, gateindex);
    if(gateindex==0){
        numSentSmart++;
    } else {
        numSentCloud++;
    }
    updateLabels();
}

void Computer::finish() {
}
