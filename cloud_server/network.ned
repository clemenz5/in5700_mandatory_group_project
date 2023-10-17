import inet.mobility.contract.IMobility;
import inet.mobility.single.TurtleMobility;
import inet.visualizer.integrated.IntegratedVisualizer;


module MobileHost  {
    parameters:
        bool left = default(false);
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
    gates:
        inout gate[2];
}


simple Host
{
    parameters:
        @display("i=misc/cloud"); // add a default icon
    gates:
        inout gate[2];
}

simple Cloud
{
    parameters:
        bool left = default(false);
        @display("i=misc/cloud"); // add a default icon
    gates:
        inout gate[2];
}


network cloudComputingNetwork
{
    
    
    @display("bgb=1300,700");
    
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
	@figure[total_delay_rcvd_comp_tp_cloud](type=text; pos=18,580; text=computer (from computer to cloud): N/A);
	@figure[total_delay_rcvd_smart_to_cloud](type=text; pos=18,595; text=smartphone (from smartphone to cloud): N/A);
	@figure[total_delay_rcvd_smart_to_comp](type=text; pos=18,610; text=smartphone (from smartphone to computer): N/A);
    
    
    @figure[Library](type=rectangle; pos=500,200; size=600,350; fillColor=gray; fillOpacity=0.2);
    @figure[Library.label](type=text; pos=510,210; text=Library);
    
    @figure[Pillar1](type=rectangle; pos=700,300; size=20,250; fillColor=black; fillOpacity=1);
    @figure[Pillar1.label](type=text; pos=510,210; text=Library);
    @figure[Shelf1](type=rectangle; pos=660,300; size=40,250; fillColor=white; fillOpacity=0.8);
    @figure[Shelf1.label](type=text; pos=510,210; text=Library);
    
    
    @figure[Pillar2](type=rectangle; pos=890,300; size=20,250; fillColor=black; fillOpacity=1);
    @figure[Pillar2.label](type=text; pos=510,210; text=Library);
    @figure[Shelf2](type=rectangle; pos=910,300; size=40,250; fillColor=white; fillOpacity=0.8);
    @figure[Shelf2.label](type=text; pos=510,210; text=Library);
    types:
        channel Channel extends ned.DelayChannel
        {
            delay = 100ms;
        }
    submodules:
        cloud: Cloud {
            @display("p=1200,100;r=480");
        }
        host: MobileHost {
            @display("p=800,500;r=70");
        }
        computer: Computer {
            @display("p=800,315;r=110");
        }
        visualizer: IntegratedVisualizer {
            @display("p=50,50");
        }

    connections:
	    host.gate[0] <--> Channel <--> cloud.gate[0];
        host.gate[1] <--> Channel <--> computer.gate[1];
        computer.gate[0] <--> Channel <--> cloud.gate[1];
}
