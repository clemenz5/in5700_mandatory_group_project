#include <ComputerMessage_m.h>
#include <omnetpp.h>

using namespace omnetpp;

class Computer : public cSimpleModule {
   private:
    bool waiting_for_ACK;    // Flag to indicate if waiting for an acknowledgment
    bool left;               // Flag indicating whether the book is in the left-hand shelf
    char displayString[20];  // Buffer for creating display strings on the simulation canvas
    int numSentCloud;        // Number of messages sent to the cloud
    int numSentSmart;        // Number of messages sent to smartphone
    int numReceivedCloud;    // Number of messages received from the cloud
    int numReceivedSmart;    // Number of messages received from smartphone

    // Arrays for power consumption and delay values, with two members each:
    // [0] - for smartphone, [1] - for the cloud
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
    left = par("left").boolValue();

    // The array index [0] corresponds to smartphone and [1] to cloud.
    sendMessagePower[0] = par("sendMessagePowerSmart").intValue();
    sendMessagePower[1] = par("sendMessagePowerCloud").intValue();

    receiveMessagePower[0] = par("receiveMessagePowerSmart").intValue();
    receiveMessagePower[1] = par("receiveMessagePowerCloud").intValue();

    sendMessageDelay[0] = par("sendMessageDelaySmart").intValue();
    sendMessageDelay[1] = par("sendMessageDelayCloud").intValue();

    receiveMessageDelay[0] = par("receiveMessageDelaySmart").intValue();
    receiveMessageDelay[1] = par("receiveMessageDelayCloud").intValue();

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
    send(new ComputerMsg("1- Contents of Book Table"), "gate$o", 1);
}

void Computer::handleMessage(cMessage* msg) {
    if (!msg->isSelfMessage()) {
        EV << msg->getArrivalGateId();
        if (msg->getArrivalGateId() == 1048576) {  // Message from smartphone
            numReceivedSmart++;
        } else {
            numReceivedCloud++;
        }
        updateLabels();
    }

    if (strcmp(msg->getName(), "2- ACK") == 0) {
        scheduleAt(simTime() + 1.0, new cMessage("timeoutEvent"));  // Schedule timeout event
        send(new ComputerMsg("3- Computer ready to start"), "gate$o", 0);
        waiting_for_ACK = true;
    } else if (strcmp(msg->getName(), "timeoutEvent") == 0) {
        // Handle timeout event
        if (waiting_for_ACK) {
            // If still waiting for an ACK, reschedule the timeout event and resend the ready message
            scheduleAt(simTime() + 1.0, new cMessage("timeoutEvent"));
            send(new ComputerMsg("3- Computer ready to start"), "gate$o", 0);
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
    } else if (strcmp(msg->getName(), "10- Pay the Book") == 0) {
        send(new ComputerMsg("11- ACK"), "gate$o", 0);
        send(new ComputerMsg("12- Book payed"), "gate$o", 0);
        send(new ComputerMsg("14- Book table index"), "gate$o", 1);
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

    sprintf(temp, "computer (from computer to smartphone)= %d", numSentSmart * sendMessagePower[0]);
    total_power_comp_to_smart->setText(temp);
    sprintf(temp, "computer (from computer to cloud)= %d", numSentCloud * sendMessagePower[1]);
    total_power_comp_to_cloud->setText(temp);

    sprintf(temp, "computer (from computer to smartphone)= %d", numReceivedSmart * receiveMessagePower[0]);
    total_power_rcvd_comp_to_smart->setText(temp);
    sprintf(temp, "computer (from computer to cloud)= %d", numReceivedCloud * receiveMessagePower[1]);
    total_power_rcvd_comp_to_cloud->setText(temp);

    sprintf(temp, "computer (from computer to smartphone)= %d", numSentSmart * sendMessageDelay[0]);
    total_delay_comp_to_smart->setText(temp);
    sprintf(temp, "computer (from computer to cloud)= %d", numReceivedCloud * sendMessageDelay[1]);
    total_delay_comp_to_cloud->setText(temp);

    sprintf(temp, "computer (from computer to smartphone)= %d", numReceivedSmart * receiveMessageDelay[0]);
    total_delay_rcvd_comp_to_smart->setText(temp);
    sprintf(temp, "computer (from computer to cloud)= %d", numReceivedCloud * receiveMessageDelay[1]);
    total_delay_rcvd_comp_to_cloud->setText(temp);
}

void Computer::send(cMessage* msg, const char* gatename, int gateindex) {
    // Update the sent message counters based on the gate index.
    cSimpleModule::send(msg, gatename, gateindex);
    if (gateindex == 0) {
        numSentSmart++;
    } else {
        numSentCloud++;
    }
    updateLabels();
}

void Computer::finish() {
}
