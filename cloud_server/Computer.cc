#include <omnetpp.h>

using namespace omnetpp;
#include <ComputerMessage_m.h>

class Computer : public cSimpleModule {
   private:
    char displayString[20];  // Buffer for creating display strings on the simulation canvas
    int numSentCloud;        // Number of messages sent to the cloud
    int numSentSmart;        // Number of messages sent to smartphone
    int numReceivedCloud;    // Number of messages received from the cloud
    int numReceivedSmart;    // Number of messages received from smartphone

    // Arrays for power consumption and delay values, with two members each:
    // [0] - for cloud, [1] - for the smartphone
    int sendMessagePower[2];     // Power consumption values for sending messages
    int receiveMessagePower[2];  // Power consumption values for receiving messages
    int sendMessageDelay[2];     // Delay times for sending messages
    int receiveMessageDelay[2];  // Delay times for receiving messages

    // Pointers to label figures for displaying statistics on the simulation canvas
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

    // Initialize power and delay values from the simulation configuration.
    // The array index [0] corresponds to cloud and [1] to smartphone.
    sendMessagePower[0] = par("sendMessagePowerCloud").intValue();
    sendMessagePower[1] = par("sendMessagePowerSmart").intValue();

    receiveMessagePower[0] = par("receiveMessagePowerCloud").intValue();
    receiveMessagePower[1] = par("receiveMessagePowerSmart").intValue();

    sendMessageDelay[0] = par("sendMessageDelayCloud").intValue();
    sendMessageDelay[1] = par("sendMessageDelaySmart").intValue();

    receiveMessageDelay[0] = par("receiveMessageDelayCloud").intValue();
    receiveMessageDelay[1] = par("receiveMessageDelaySmart").intValue();

    // Initialize canvas for displaying statistics
    cCanvas* canvas = this->getParentModule()->getCanvas();

    // Setting up the labels for the statistics
    total_num_comp = (cLabelFigure*)(canvas->getFigure("total_num_comp"));

    total_power_comp_to_cloud = (cLabelFigure*)(canvas->getFigure("total_power_comp_to_cloud"));
    total_power_comp_to_smart = (cLabelFigure*)(canvas->getFigure("total_power_comp_to_smart"));
    total_power_rcvd_comp_to_smart = (cLabelFigure*)(canvas->getFigure("total_power_rcvd_comp_to_smart"));
    total_power_rcvd_comp_to_cloud = (cLabelFigure*)(canvas->getFigure("total_power_rcvd_comp_to_cloud"));

    total_delay_comp_to_smart = (cLabelFigure*)(canvas->getFigure("total_delay_comp_to_smart"));
    total_delay_comp_to_cloud = (cLabelFigure*)(canvas->getFigure("total_delay_comp_to_cloud"));
    total_delay_rcvd_comp_to_smart = (cLabelFigure*)(canvas->getFigure("total_delay_rcvd_comp_to_smart"));
    total_delay_rcvd_comp_to_cloud = (cLabelFigure*)(canvas->getFigure("total_delay_rcvd_comp_to_cloud"));

    // Sending initial message
    send(new ComputerMsg("1- Contents of Book Table"), "gate$o", 0);
}

void Computer::handleMessage(cMessage* msg) {
    if (!msg->isSelfMessage()) {
        if (msg->getArrivalGateId() == 1048576) {  // Message from cloud
            numReceivedCloud++;
        } else {
            numReceivedSmart++;
        }
        updateLabels();
    }

    // Handle specific messages and respond with ACKs as needed.
    if (strcmp(msg->getName(), "10- Pay the Book") == 0) {
        send(new ComputerMsg("11- ACK"), "gate$o", 1);
        send(new ComputerMsg("12- Book payed"), "gate$o", 1);
        send(new ComputerMsg("14- Book table index"), "gate$o", 0);
    }
}

void Computer::updateLabels() {
    // Update the display labels with current statistics.
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

    sprintf(temp, "computer (from computer to cloud)= %d", numReceivedCloud * receiveMessagePower[0]);
    total_power_rcvd_comp_to_cloud->setText(temp);
    sprintf(temp, "computer (from computer to smartphone)= %d", numReceivedSmart * receiveMessagePower[1]);
    total_power_rcvd_comp_to_smart->setText(temp);

    sprintf(temp, "computer (from computer to cloud)= %d", numReceivedCloud * sendMessageDelay[0]);
    total_delay_comp_to_cloud->setText(temp);
    sprintf(temp, "computer (from computer to smartphone)= %d", numSentSmart * sendMessageDelay[1]);
    total_delay_comp_to_smart->setText(temp);

    sprintf(temp, "computer (from computer to cloud)= %d", numReceivedCloud * receiveMessageDelay[0]);
    total_delay_rcvd_comp_to_cloud->setText(temp);
    sprintf(temp, "computer (from computer to smartphone)= %d", numReceivedSmart * receiveMessageDelay[1]);
    total_delay_rcvd_comp_to_smart->setText(temp);
}

void Computer::send(cMessage* msg, const char* gatename, int gateindex) {
    // Update the sent message counters based on the gate index.
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
