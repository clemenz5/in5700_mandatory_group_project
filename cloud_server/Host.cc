#include <ComputerMessage_m.h>
#include <omnetpp.h>

using namespace omnetpp;

class Host : public cSimpleModule {
   private:
    int numDropped;          // Number of dropped messages.
    bool left;               // Flag indicating whether the book is in the left-hand shelf
    char displayString[20];  // Buffer for display strings on the simulation canvas.
    int numSentCloud;        // Count of messages sent to the cloud.
    int numSentComp;         // Count of messages sent to computer.
    int numReceivedCloud;    // Count of messages received from the cloud.
    int numReceivedComp;     // Count of messages received from computer.

    // Arrays for power consumption and delay values, with two members each:
    // [0] - for cloud, [1] - for computer
    int sendMessagePower[2];     // Power consumption values for sending messages
    int receiveMessagePower[2];  // Power consumption values for receiving messages
    int sendMessageDelay[2];     // Delay times for sending messages
    int receiveMessageDelay[2];  // Delay times for receiving messages

    // Pointers to label figures for displaying statistics on the simulation canvas
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
    virtual void handleMessage(cMessage* msg) override;
    virtual void finish() override;
    virtual void send(cMessage* msg, const char* gatename, int gateindex = -1) override;
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

    // Initialize power and delay values from the simulation configuration.
    left = this->getParentModule()->par("left").boolValue();

    // The array index [0] corresponds to cloud and [1] to computer.
    sendMessagePower[0] = par("sendMessagePowerCloud").intValue();
    sendMessagePower[1] = par("sendMessagePowerComp").intValue();

    receiveMessagePower[0] = par("receiveMessagePowerCloud").intValue();
    receiveMessagePower[1] = par("receiveMessagePowerComp").intValue();

    sendMessageDelay[0] = par("sendMessageDelayCloud").intValue();
    sendMessageDelay[1] = par("sendMessageDelayComp").intValue();

    receiveMessageDelay[0] = par("receiveMessageDelayCloud").intValue();
    receiveMessageDelay[1] = par("receiveMessageDelayComp").intValue();

    // Schedule self-messages for simulating specific actions.
    scheduleAt(simTime() + 38.0, new cMessage("browseBook"));
    scheduleAt(simTime() + 57.0, new cMessage("payBook"));

    // Initialize canvas for displaying statistics.
    cCanvas* canvas = this->getParentModule()->getParentModule()->getCanvas();
    total_num_smartphone = (cLabelFigure*)(canvas->getFigure("total_num_smartphone"));

    total_power_smart_to_cloud = (cLabelFigure*)(canvas->getFigure("total_power_smart_to_cloud"));
    total_power_smart_to_comp = (cLabelFigure*)(canvas->getFigure("total_power_smart_to_comp"));
    total_power_rcvd_smart_to_cloud = (cLabelFigure*)(canvas->getFigure("total_power_rcvd_smart_to_cloud"));
    total_power_rcvd_smart_to_comp = (cLabelFigure*)(canvas->getFigure("total_power_rcvd_smart_to_comp"));

    total_delay_smart_to_cloud = (cLabelFigure*)(canvas->getFigure("total_delay_smart_to_cloud"));
    total_delay_smart_to_comp = (cLabelFigure*)(canvas->getFigure("total_delay_smart_to_comp"));
    total_delay_rcvd_smart_to_cloud = (cLabelFigure*)(canvas->getFigure("total_delay_rcvd_smart_to_cloud"));
    total_delay_rcvd_smart_to_comp = (cLabelFigure*)(canvas->getFigure("total_delay_rcvd_smart_to_comp"));

    updateLabels();  // Update the labels initially
}

void Host::handleMessage(cMessage* msg) {
    if (!msg->isSelfMessage()) {
        if (msg->getArrivalGateId() == 1048576) {  // Message from cloud
            numReceivedCloud++;
        } else {
            numReceivedComp++;
        }
        updateLabels();
    }

    if (strcmp(msg->getName(), "3- Cloud ready to start") == 0 && numDropped < 5) {
        // Simulate message loss
        this->getParentModule()->bubble("Message Lost");
        numDropped++;
    } else if (strcmp(msg->getName(), "3- Cloud ready to start") == 0 && numDropped >= 5) {
        // Simulate message receival
        send(new ComputerMsg("4- ACK"), "gate$o", 0);
        send(new ComputerMsg("5- Where is the book I am looking for?"),
             "gate$o", 0);
    } else if (strcmp(msg->getName(),
                      "7- The book you are looking for is in the left-hand shelf") == 0 ||
               strcmp(msg->getName(),
                      "8- The book you are looking for is in the right-hand shelf") == 0) {
        send(new ComputerMsg("9- ACK"), "gate$o", 0);
    } else if (strcmp(msg->getName(), "browseBook") == 0) {
        this->getParentModule()->bubble("Browse Book");
    } else if (strcmp(msg->getName(), "payBook") == 0) {
        send(new ComputerMsg("10- Pay the Book"), "gate$o", 1);
    } else if (strcmp(msg->getName(), "12- Book payed") == 0) {
        send(new ComputerMsg("13- ACK"), "gate$o", 1);
    }
}

void Host::updateLabels() {
    sprintf(displayString, "sent:%d rcvd:%d lost:%d", numSentComp + numSentCloud, numReceivedComp + numReceivedCloud - numDropped, numDropped);
    this->getParentModule()->getDisplayString().setTagArg("t", 0,
                                                          displayString);

    char temp[80];
    sprintf(temp, "Total number of messages sent/received by the smartphone= %d", numSentComp + numSentCloud + numReceivedComp + numReceivedCloud - numDropped);
    total_num_smartphone->setText(temp);
    sprintf(temp, "smartphone (from smartphone to cloud)= %d", numSentCloud * sendMessagePower[0]);
    total_power_smart_to_cloud->setText(temp);
    sprintf(temp, "smartphone (from smartphone to comp)= %d", numSentComp * sendMessagePower[1]);
    total_power_smart_to_comp->setText(temp);

    sprintf(temp, "smartphone (from smartphone to cloud)= %d", (numReceivedCloud - numDropped) * receiveMessagePower[0]);
    total_power_rcvd_smart_to_cloud->setText(temp);
    sprintf(temp, "smartphone (from smartphone to comp)= %d", numReceivedComp * receiveMessagePower[1]);
    total_power_rcvd_smart_to_comp->setText(temp);

    sprintf(temp, "smartphone (from smartphone to cloud)= %d", numSentCloud * sendMessageDelay[0]);
    total_delay_smart_to_cloud->setText(temp);
    sprintf(temp, "smartphone (from smartphone to comp)= %d", numSentComp * sendMessageDelay[1]);
    total_delay_smart_to_comp->setText(temp);

    sprintf(temp, "smartphone (from smartphone to cloud)= %d", (numReceivedCloud - numDropped) * receiveMessageDelay[0]);
    total_delay_rcvd_smart_to_cloud->setText(temp);
    sprintf(temp, "smartphone (from smartphone to comp)= %d", numReceivedComp * receiveMessageDelay[1]);
    total_delay_rcvd_smart_to_comp->setText(temp);
}

void Host::send(cMessage* msg, const char* gatename, int gateindex) {
    cSimpleModule::send(msg, gatename, gateindex);
    if (gateindex == 0) {
        numSentCloud++;
    } else {
        numSentComp++;
    }

    updateLabels();  // Update labels after sending a message.
}

void Host::finish() {
    updateLabels();
}
