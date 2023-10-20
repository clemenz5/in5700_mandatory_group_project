#include <omnetpp.h>
#include <ComputerMessage_m.h>

using namespace omnetpp;


class Cloud: public cSimpleModule {
private:
    char displayString[20];
    int numSentSmart;
    int numSentComp;
    int numReceivedSmart;
    int numReceivedComp;
    int sendMessagePower[2];
    int receiveMessagePower[2];
    int messageDelay;

    cLabelFigure* total_num_cloud;
    cLabelFigure* total_power_cloud_to_smart;
    cLabelFigure* total_power_cloud_to_comp;
    cLabelFigure* total_power_rcvd_cloud_to_smart;
    cLabelFigure* total_power_rcvd_cloud_to_comp;
    cLabelFigure* total_delay_cloud_to_smart;
    cLabelFigure* total_delay_cloud_to_comp;
    cLabelFigure* total_delay_rcvd_cloud_to_smart;
    cLabelFigure* total_delay_rcvd_cloud_to_comp;


protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    virtual void send(cMessage *msg, const char *gatename, int gateindex=-1) override;
    void updateLabels();

};

// The module class needs to be registered with OMNeT++
Define_Module(Cloud);

void Cloud::initialize() {
    numSentComp = 0;
    numSentSmart = 0;
    numReceivedComp = 0;
    numReceivedSmart = 0;

    sendMessagePower[0] = 200;
    sendMessagePower[1] = 100;

    receiveMessagePower[0] = 300;
    receiveMessagePower[1] = 300;

    messageDelay = 100;

    cCanvas* canvas = this->getParentModule()->getCanvas();

    total_num_cloud= (cLabelFigure*)(canvas->getFigure("total_num_cloud"));
    total_power_cloud_to_smart= (cLabelFigure*)(canvas->getFigure("total_power_cloud_to_smart"));
    total_power_cloud_to_comp= (cLabelFigure*)(canvas->getFigure("total_power_cloud_to_comp"));
    total_power_rcvd_cloud_to_smart= (cLabelFigure*)(canvas->getFigure("total_power_rcvd_cloud_to_smart"));
    total_power_rcvd_cloud_to_comp= (cLabelFigure*)(canvas->getFigure("total_power_rcvd_cloud_to_comp"));

    total_delay_cloud_to_smart= (cLabelFigure*)(canvas->getFigure("total_delay_cloud_to_smart"));
    total_delay_cloud_to_comp= (cLabelFigure*)(canvas->getFigure("total_delay_cloud_to_comp"));
    total_delay_rcvd_cloud_to_smart= (cLabelFigure*)(canvas->getFigure("total_delay_rcvd_cloud_to_smart"));
    total_delay_rcvd_cloud_to_comp= (cLabelFigure*)(canvas->getFigure("total_delay_rcvd_cloud_to_comp"));
    updateLabels();

}

void Cloud::handleMessage(cMessage *msg) {
    if (!msg->isSelfMessage()) {
        
        if(msg->getArrivalGateId()==1048576){
            numReceivedComp++;
        } else {
            numReceivedSmart++;
        }
        updateLabels();
    }
    if (strcmp(msg->getName(), "1- Contents of Book Table") == 0) {
            send(new ComputerMsg("2- ACK"), "gate$o", 0);
        }
    else if (strcmp(msg->getName(), "14- Book table index") == 0){
            send(new ComputerMsg("15- ACK"), "gate$o", 0);
    }

}

void Cloud::updateLabels(){
    sprintf(displayString, "sent:%d rcvd:%d", numSentComp+numSentSmart, numReceivedComp+numReceivedSmart);
    this->getDisplayString().setTagArg("t", 0,
                displayString);

    char temp[100];
    sprintf(temp, "Total number of messages sent/received by the cloud= %d", numSentComp+numSentSmart+numReceivedComp+numReceivedSmart);
    total_num_cloud->setText(temp);
    sprintf(temp, "cloud (from cloud to smartphone)= %d", numSentSmart*sendMessagePower[0]);
    total_power_cloud_to_smart->setText(temp);
    sprintf(temp, "cloud (from cloud to comp)= %d", numSentComp*sendMessagePower[1]);
    total_power_cloud_to_comp->setText(temp);

    sprintf(temp, "cloud (from cloud to smartphone)= %d", numReceivedSmart*receiveMessagePower[0]);
    total_power_rcvd_cloud_to_smart->setText(temp);
    sprintf(temp, "cloud (from cloud to comp)= %d", numReceivedComp*receiveMessagePower[1]);
    total_power_rcvd_cloud_to_comp->setText(temp);

    sprintf(temp, "cloud (from cloud to smartphone)= %d", numReceivedSmart*messageDelay);
    total_delay_cloud_to_smart->setText(temp);
    sprintf(temp, "cloud (from cloud to comp)= %d", numReceivedComp*messageDelay);
    total_delay_cloud_to_comp->setText(temp);

    sprintf(temp, "cloud (from cloud to smartphone)= %d", numReceivedSmart*messageDelay);
    total_delay_rcvd_cloud_to_smart->setText(temp);
    sprintf(temp, "cloud (from cloud to comp)= %d", numReceivedComp*messageDelay);
    total_delay_rcvd_cloud_to_comp->setText(temp);
}

void Cloud::send(cMessage *msg, const char *gatename, int gateindex){
    cSimpleModule::send(msg, gatename, gateindex);
    if(gateindex==0){
        numSentComp++;
    } else {
        numSentSmart++;
    }
    updateLabels();
}

void Cloud::finish() {
}
