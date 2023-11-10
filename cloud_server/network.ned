import inet.mobility.contract.IMobility;
import inet.mobility.single.TurtleMobility;
import inet.visualizer.integrated.IntegratedVisualizer;


module MobileHost  {
    parameters:
        bool left = default(false); // Flag indicating whether the book is in the left-hand shelf
        @display("i=device/pocketpc"); // add a default icon
	gates:
	    inout gate[2];
    submodules:
        mobility: <default("StationaryMobility")> like IMobility {
            @display("p=100,100;is=s");
        }
        host: Host;
    connections:
        host.gate[0] <-->  gate[0];
        host.gate[1] <-->  gate[1];
}

simple Computer
{
    parameters:
        @display("i=device/pc"); // add a default icon
        int sendMessagePowerSmart = default(0);
        int sendMessagePowerCloud = default(0);
        int receiveMessagePowerSmart = default(0);
        int receiveMessagePowerCloud = default(0);
        int sendMessageDelaySmart = default(0);
        int sendMessageDelayCloud = default(0);
        int receiveMessageDelaySmart = default(0);
        int receiveMessageDelayCloud = default(0);
    gates:
        inout gate[2];
}


simple Host
{
    parameters:
        @display("i=misc/cloud"); // add a default icon
        int sendMessagePowerCloud = default(0);
        int sendMessagePowerComp = default(0);
        int receiveMessagePowerCloud = default(0);
        int receiveMessagePowerComp = default(0);
        int sendMessageDelayCloud = default(0);
        int sendMessageDelayComp = default(0);
        int receiveMessageDelayCloud = default(0);
        int receiveMessageDelayComp = default(0);
    gates:
        inout gate[2];
}

simple Cloud
{
    parameters:
        @display("i=misc/cloud"); // add a default icon
        bool left = default(false);
        int sendMessagePowerSmart = default(0);
        int sendMessagePowerComputer = default(0);
        int receiveMessagePowerSmart = default(0);
        int receiveMessagePowerComputer = default(0);
        int sendMessageDelaySmart = default(0);
        int sendMessageDelayComputer = default(0);
        int receiveMessageDelaySmart = default(0);
        int receiveMessageDelayComputer = default(0);
    gates:
        inout gate[2];
}


network cloudComputingNetwork
{
    // Setup graphical figures and labels
    
    @display("bgb=1300,700");
    @figure[entry_exit](type=text; pos=760,550; text=entry/exit);
    
    @figure[total_num_cloud](type=text; pos=18,100; text=Total number of messages sent/received by the cloud: N/A);
	@figure[total_num_comp](type=text; pos=18,115; text=Total number of messages sent/received by the computer: N/A);
	@figure[total_num_smartphone](type=text; pos=18,130; text=Total number of messages sent/received by the smartphone: N/A);
	
	@figure[total_power](type=text; pos=18,160; text=Total power spent when sending messages by:);
	@figure[total_power_cloud_to_smart](type=text; pos=18,175; text=cloud (from cloud to smartphone): N/A);
	@figure[total_power_cloud_to_comp](type=text; pos=18,190; text=cloud (from cloud to computer): N/A);
	@figure[total_power_comp_to_cloud](type=text; pos=18,205; text=computer (from computer to cloud): N/A);
	@figure[total_power_comp_to_smart](type=text; pos=18,220; text=computer (from computer to smartphone): N/A);
	@figure[total_power_smart_to_cloud](type=text; pos=18,235; text=smartphone (from smartphone to cloud): N/A);
	@figure[total_power_smart_to_comp](type=text; pos=18,250; text=smartphone (from smartphone to computer): N/A);
	
	@figure[total_power_rcvd](type=text; pos=18,280; text=Total power spent when receiving messages by:);
	@figure[total_power_rcvd_cloud_to_smart](type=text; pos=18,295; text=cloud (from cloud to smartphone): N/A);
	@figure[total_power_rcvd_cloud_to_comp](type=text; pos=18,310; text=cloud (from cloud to computer): N/A);
	@figure[total_power_rcvd_comp_to_smart](type=text; pos=18,325; text=computer (from computer to smartphone): N/A);
	@figure[total_power_rcvd_comp_to_cloud](type=text; pos=18,340; text=computer (from computer to cloud): N/A);
	@figure[total_power_rcvd_smart_to_cloud](type=text; pos=18,355; text=smartphone (from smartphone to cloud): N/A);
	@figure[total_power_rcvd_smart_to_comp](type=text; pos=18,370; text=smartphone (from smartphone to computer): N/A);
	
	@figure[total_delay](type=text; pos=18,400; text=Total delay when sending messages by:);
	@figure[total_delay_cloud_to_smart](type=text; pos=18,415; text=cloud (from cloud to smartphone): N/A);
	@figure[total_delay_cloud_to_comp](type=text; pos=18,430; text=cloud (from cloud to computer): N/A);
	@figure[total_delay_comp_to_smart](type=text; pos=18,445; text=computer (from computer to smartphone): N/A);
	@figure[total_delay_comp_to_cloud](type=text; pos=18,460; text=computer (from computer to cloud): N/A);
	@figure[total_delay_smart_to_cloud](type=text; pos=18,475; text=smartphone (from smartphone to cloud): N/A);
	@figure[total_delay_smart_to_comp](type=text; pos=18,490; text=smartphone (from smartphone to computer): N/A);
	
	@figure[total_delay_rcvd](type=text; pos=18,520; text=Total delay when receiving messages by:);
	@figure[total_delay_rcvd_cloud_to_smart](type=text; pos=18,535; text=cloud (from cloud to smartphone): N/A);
	@figure[total_delay_rcvd_cloud_to_comp](type=text; pos=18,550; text=cloud (from cloud to computer): N/A);
	@figure[total_delay_rcvd_comp_to_smart](type=text; pos=18,565; text=computer (from computer to smartphone): N/A);
	@figure[total_delay_rcvd_comp_to_cloud](type=text; pos=18,580; text=computer (from computer to cloud): N/A);
	@figure[total_delay_rcvd_smart_to_cloud](type=text; pos=18,595; text=smartphone (from smartphone to cloud): N/A);
	@figure[total_delay_rcvd_smart_to_comp](type=text; pos=18,610; text=smartphone (from smartphone to computer): N/A);
    
    
    @figure[Library](type=rectangle; pos=500,200; size=600,350; fillColor=gray; fillOpacity=0.2; zIndex=1);
    @figure[Library.label](type=text; pos=510,210; text=Library);
    
    @figure[Pillar1](type=rectangle; pos=700,300; size=20,250; fillColor=black; fillOpacity=1);
    @figure[Library.label_p1](type=text; pos=705,310; text=P; zIndex=10;color=white);
    @figure[Library.label_i1](type=text; pos=705,353; text=I; zIndex=10;color=white);
    @figure[Library.label_l3](type=text; pos=705,396; text=L; zIndex=10;color=white);
    @figure[Library.label_l4](type=text; pos=705,439; text=L; zIndex=10;color=white);
    @figure[Library.label_a1](type=text; pos=705,482; text=A; zIndex=10;color=white);
    @figure[Library.label_r1](type=text; pos=705,526; text=R; zIndex=10;color=white);

    @figure[Shelf1](type=rectangle; pos=660,300; size=40,250; fillColor=white; fillOpacity=0.8);
    @figure[Library.label_s1](type=text; pos=675,310; text=S; zIndex=10);
    @figure[Library.label_h1](type=text; pos=675,364; text=H);
    @figure[Library.label_e1](type=text; pos=675,418; text=E);
    @figure[Library.label_l1](type=text; pos=675,472; text=L);
    @figure[Library.label_f1](type=text; pos=675,526; text=F);
    
    @figure[Pillar2](type=rectangle; pos=890,300; size=20,250; fillColor=black; fillOpacity=1);
    @figure[Library.label_p2](type=text; pos=896,310; text=P; zIndex=10;color=white);
    @figure[Library.label_i2](type=text; pos=896,353; text=I; zIndex=10;color=white);
    @figure[Library.label_l5](type=text; pos=896,396; text=L; zIndex=10;color=white);
    @figure[Library.label_l6](type=text; pos=896,439; text=L; zIndex=10;color=white);
    @figure[Library.label_a2](type=text; pos=896,482; text=A; zIndex=10;color=white);
    @figure[Library.label_r2](type=text; pos=896,526; text=R; zIndex=10;color=white);

    @figure[Shelf2](type=rectangle; pos=910,300; size=40,250; fillColor=white; fillOpacity=0.8);
    @figure[Library.label_s2](type=text; pos=923,310; text=S; zIndex=10);
    @figure[Library.label_h2](type=text; pos=923,364; text=H);
    @figure[Library.label_e2](type=text; pos=923,418; text=E);
    @figure[Library.label_l2](type=text; pos=923,472; text=L);
    @figure[Library.label_f2](type=text; pos=923,526; text=F);
    
    types:
        channel Channel extends ned.DelayChannel
        {
            delay = 100ms;
        }
    submodules:
        cloud: Cloud {
            @display("p=1200,100;r=480");
        }
        host[1]: MobileHost {
            @display("p=800,500;r=70");
        }
        computer: Computer {
            @display("p=800,315;r=110");
        }
        visualizer: IntegratedVisualizer {
            @display("p=50,50");
        }

    connections:
	    host[0].gate[0] <--> Channel <--> cloud.gate[0];
        host[0].gate[1] <--> Channel <--> computer.gate[1];
        computer.gate[0] <--> Channel <--> cloud.gate[1];
}
