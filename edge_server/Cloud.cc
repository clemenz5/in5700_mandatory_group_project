#include <ComputerMessage_m.h>
#include <omnetpp.h>

using namespace omnetpp;

class Cloud : public cSimpleModule {
   private:
    char displayString[20];  // Buffer for creating display strings on the simulation canvas.
    int numSentSmart;        // Counter for messages sent to smartphone.
    int numSentComp;         // Counter for messages sent to computer.
    int numReceivedSmart;    // Counter for messages received from smartphone.
    int numReceivedComp;     // Counter for messages received from computer.

    // Arrays for power consumption and delay values, with two members each:
    // [0] - for computer, [1] - for smartphone.
    int sendMessagePower[2];     // Power consumption values for sending messages.
    int receiveMessagePower[2];  // Power consumption values for receiving messages.
    int sendMessageDelay[2];     // Delay times for sending messages.
    int receiveMessageDelay[2];  // Delay times for receiving messages.

    // Pointers to label figures for displaying statistics on the simulation canvas.
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
    virtual void handleMessage(cMessage* msg) override;
    virtual void finish() override;
    virtual void send(cMessage* msg, const char* gatename, int gateindex = -1) override;
    void updateLabels();
};

// The module class needs to be registered with OMNeT++
Define_Module(Cloud);

void Cloud::initialize() {
    numSentComp = 0;
    numSentSmart = 0;
    numReceivedComp = 0;
    numReceivedSmart = 0;

    // Initialize power and delay values from the simulation configuration.
    // The array index [0] corresponds to smartphone and [1] to cloud.
    sendMessagePower[0] = par("sendMessagePowerComputer").intValue();
    sendMessagePower[1] = par("sendMessagePowerSmart").intValue();

    receiveMessagePower[0] = par("receiveMessagePowerComputer").intValue();
    receiveMessagePower[1] = par("receiveMessagePowerSmart").intValue();

    sendMessageDelay[0] = par("sendMessageDelayComputer").intValue();
    sendMessageDelay[1] = par("sendMessageDelaySmart").intValue();

    receiveMessageDelay[0] = par("receiveMessageDelayComputer").intValue();
    receiveMessageDelay[1] = par("receiveMessageDelaySmart").intValue();

    // Initialize canvas for displaying statistics
    cCanvas* canvas = this->getParentModule()->getCanvas();

    // Setting up the labels for the statistics
    total_num_cloud = (cLabelFigure*)(canvas->getFigure("total_num_cloud"));

    total_power_cloud_to_smart = (cLabelFigure*)(canvas->getFigure("total_power_cloud_to_smart"));
    total_power_cloud_to_comp = (cLabelFigure*)(canvas->getFigure("total_power_cloud_to_comp"));
    total_power_rcvd_cloud_to_smart = (cLabelFigure*)(canvas->getFigure("total_power_rcvd_cloud_to_smart"));
    total_power_rcvd_cloud_to_comp = (cLabelFigure*)(canvas->getFigure("total_power_rcvd_cloud_to_comp"));

    total_delay_cloud_to_smart = (cLabelFigure*)(canvas->getFigure("total_delay_cloud_to_smart"));
    total_delay_cloud_to_comp = (cLabelFigure*)(canvas->getFigure("total_delay_cloud_to_comp"));
    total_delay_rcvd_cloud_to_smart = (cLabelFigure*)(canvas->getFigure("total_delay_rcvd_cloud_to_smart"));
    total_delay_rcvd_cloud_to_comp = (cLabelFigure*)(canvas->getFigure("total_delay_rcvd_cloud_to_comp"));

    updateLabels();  // Update the labels initially
}

void Cloud::handleMessage(cMessage* msg) {
    // Update the received message counters based on the message's arrival gate.
    if (!msg->isSelfMessage()) {
        if (msg->getArrivalGateId() == 1048576) {  // Message from computer
            numReceivedComp++;
        } else {
            numReceivedSmart++;
        }
        updateLabels();
    }

    // Handle specific messages and respond with ACKs as needed.
    if (strcmp(msg->getName(), "1- Contents of Book Table") == 0) {
        send(new ComputerMsg("2- ACK"), "gate$o", 0);
    } else if (strcmp(msg->getName(), "14- Book table index") == 0) {
        send(new ComputerMsg("15- ACK"), "gate$o", 0);
    }
}

void Cloud::updateLabels() {
    // Update the display labels with current statistics.

    sprintf(displayString, "sent:%d rcvd:%d", numSentComp + numSentSmart, numReceivedComp + numReceivedSmart);
    this->getDisplayString().setTagArg("t", 0,
                                       displayString);

    char temp[100];
    sprintf(temp, "Total number of messages sent/received by the cloud= %d", numSentComp + numSentSmart + numReceivedComp + numReceivedSmart);
    total_num_cloud->setText(temp);
    sprintf(temp, "cloud (from cloud to comp)= %d", numSentComp * sendMessagePower[0]);
    total_power_cloud_to_comp->setText(temp);
    sprintf(temp, "cloud (from cloud to smartphone)= %d", numSentSmart * sendMessagePower[1]);
    total_power_cloud_to_smart->setText(temp);

    sprintf(temp, "cloud (from cloud to comp)= %d", numReceivedComp * receiveMessagePower[0]);
    total_power_rcvd_cloud_to_comp->setText(temp);
    sprintf(temp, "cloud (from cloud to smartphone)= %d", numReceivedSmart * receiveMessagePower[1]);
    total_power_rcvd_cloud_to_smart->setText(temp);

    sprintf(temp, "cloud (from cloud to comp)= %d", numSentComp * sendMessageDelay[0]);
    total_delay_cloud_to_comp->setText(temp);
    sprintf(temp, "cloud (from cloud to smartphone)= %d", numSentSmart * sendMessageDelay[1]);
    total_delay_cloud_to_smart->setText(temp);

    sprintf(temp, "cloud (from cloud to comp)= %d", numReceivedComp * receiveMessageDelay[0]);
    total_delay_rcvd_cloud_to_comp->setText(temp);
    sprintf(temp, "cloud (from cloud to smartphone)= %d", numReceivedSmart * receiveMessageDelay[1]);
    total_delay_rcvd_cloud_to_smart->setText(temp);
}

void Cloud::send(cMessage* msg, const char* gatename, int gateindex) {
    // Update the sent message counters based on the gate index.
    cSimpleModule::send(msg, gatename, gateindex);
    if (gateindex == 0) {
        numSentComp++;
    } else {
        numSentSmart++;
    }
    updateLabels();
}

void Cloud::finish() {
}
