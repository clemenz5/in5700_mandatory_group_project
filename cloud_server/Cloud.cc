#include <ComputerMessage_m.h>
#include <omnetpp.h>

using namespace omnetpp;

class Cloud : public cSimpleModule {
   private:
    bool waiting_for_ACK;    // Flag to indicate if waiting for an acknowledgment
    bool left;               // Flag indicating whether the book is in the left-hand shelf
    char displayString[20];  // Buffer for display strings on the simulation canvas.
    int numSentSmart;        // Number of messages sent to smartphone
    int numSentComp;         // Number of messages sent to computer
    int numReceivedSmart;    // Number of messages received from smartphone
    int numReceivedComp;     // Number of messages received from computer

    // Arrays for power consumption and delay values, with two members each:
    // [0] - for smartphone, [1] - for the computer
    int sendMessagePower[2];     // Power consumption values for sending messages
    int receiveMessagePower[2];  // Power consumption values for receiving messages
    int sendMessageDelay[2];     // Delay times for sending messages
    int receiveMessageDelay[2];  // Delay times for receiving messages

    // Pointers to label figures for displaying statistics on the simulation canvas
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
    left = par("left").boolValue();
    numSentComp = 0;
    numSentSmart = 0;
    numReceivedComp = 0;
    numReceivedSmart = 0;

    // Initialize power and delay values from the simulation configuration.
    // The array index [0] corresponds to smartphone and [1] to computer.
    sendMessagePower[0] = par("sendMessagePowerSmart").intValue();
    sendMessagePower[1] = par("sendMessagePowerComputer").intValue();

    receiveMessagePower[0] = par("receiveMessagePowerSmart").intValue();
    receiveMessagePower[1] = par("receiveMessagePowerComputer").intValue();

    sendMessageDelay[0] = par("sendMessageDelaySmart").intValue();
    sendMessageDelay[1] = par("sendMessageDelayComputer").intValue();

    receiveMessageDelay[0] = par("receiveMessageDelaySmart").intValue();
    receiveMessageDelay[1] = par("receiveMessageDelayComputer").intValue();

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
    if (!msg->isSelfMessage()) {
        if (msg->getArrivalGateId() == 1048576) {  // Message from smartphone
            numReceivedSmart++;
        } else {
            numReceivedComp++;
        }
        updateLabels();
    }

    if (strcmp(msg->getName(), "1- Contents of Book Table") == 0) {
        send(new ComputerMsg("2- ACK"), "gate$o", 1);
        scheduleAt(simTime() + 1.0, new cMessage("timeoutEvent"));
        send(new ComputerMsg("3- Cloud ready to start"), "gate$o", 0);
        waiting_for_ACK = true;
    } else if (strcmp(msg->getName(), "timeoutEvent") == 0) {
        // Handle timeout event
        if (waiting_for_ACK) {
            // If still waiting for an ACK, reschedule the timeout event and resend the ready message
            scheduleAt(simTime() + 1.0, new cMessage("timeoutEvent"));
            send(new ComputerMsg("3- Cloud ready to start"), "gate$o", 0);
        }
    } else if (strcmp(msg->getName(), "5- Where is the book I am looking for?") == 0) {
        waiting_for_ACK = false;  // ACK received, no longer waiting

        // Send the appropriate response based on the book's location
        if (left) {
            send(new ComputerMsg("6- ACK"), "gate$o", 0);
            send(new ComputerMsg("7- The book you are looking for is in the left-hand shelf"), "gate$o", 0);
        } else {
            send(new ComputerMsg("6- ACK"), "gate$o", 0);
            send(new ComputerMsg("8- The book you are looking for is in the right-hand shelf"), "gate$o", 0);
        }
    } else if (strcmp(msg->getName(), "14- Book table index") == 0) {
        send(new ComputerMsg("15- ACK"), "gate$o", 1);
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
    sprintf(temp, "cloud (from cloud to smartphone)= %d", numSentSmart * sendMessagePower[0]);
    total_power_cloud_to_smart->setText(temp);
    sprintf(temp, "cloud (from cloud to comp)= %d", numSentComp * sendMessagePower[1]);
    total_power_cloud_to_comp->setText(temp);

    sprintf(temp, "cloud (from cloud to smartphone)= %d", numReceivedSmart * receiveMessagePower[0]);
    total_power_rcvd_cloud_to_smart->setText(temp);
    sprintf(temp, "cloud (from cloud to comp)= %d", numReceivedComp * receiveMessagePower[1]);
    total_power_rcvd_cloud_to_comp->setText(temp);

    sprintf(temp, "cloud (from cloud to smartphone)= %d", numSentSmart * sendMessageDelay[0]);
    total_delay_cloud_to_smart->setText(temp);
    sprintf(temp, "cloud (from cloud to comp)= %d", numSentComp * sendMessageDelay[1]);
    total_delay_cloud_to_comp->setText(temp);

    sprintf(temp, "cloud (from cloud to smartphone)= %d", numReceivedSmart * receiveMessageDelay[0]);
    total_delay_rcvd_cloud_to_smart->setText(temp);
    sprintf(temp, "cloud (from cloud to comp)= %d", numReceivedComp * receiveMessageDelay[1]);
    total_delay_rcvd_cloud_to_comp->setText(temp);
}

void Cloud::send(cMessage* msg, const char* gatename, int gateindex) {
    cSimpleModule::send(msg, gatename, gateindex);
    if (gateindex == 0) {
        numSentSmart++;
    } else {
        numSentComp++;
    }
    updateLabels();
}

void Cloud::finish() {
}
