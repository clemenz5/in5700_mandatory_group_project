simple Host
{
    parameters:
        @display("i=device/pocketpc"); // add a default icon
    gates:
        inout gate[2];
}

simple Computer
{
    parameters:
        @display("i=device/pc"); // add a default icon
    gates:
        inout gate[2];
}


simple Cloud
{
    parameters:
        @display("i=misc/cloud"); // add a default icon
    gates:
        inout gate[2];
}

module Library
{
	@display(p=580,425);
}


network cloudComputingNetwork
{
    @display("bgb=1000,700");
    @figure[Library](type=rectangle; pos=200,200; size=600,350; fillColor=gray; fillOpacity=0.2);
    @figure[Library.label](type=text; pos=210,210; text=Library);
    
    @figure[Pillar1](type=rectangle; pos=400,300; size=20,250; fillColor=black; fillOpacity=1);
    @figure[Pillar1.label](type=text; pos=210,210; text=Library);
    @figure[Shelf1](type=rectangle; pos=360,300; size=40,250; fillColor=white; fillOpacity=0.8);
    @figure[Shelf1.label](type=text; pos=210,210; text=Library);
    
    
    @figure[Pillar2](type=rectangle; pos=590,300; size=20,250; fillColor=black; fillOpacity=1);
    @figure[Pillar2.label](type=text; pos=210,210; text=Library);
    @figure[Shelf2](type=rectangle; pos=610,300; size=40,250; fillColor=white; fillOpacity=0.8);
    @figure[Shelf2.label](type=text; pos=210,210; text=Library);
    types:
        channel Channel extends ned.DelayChannel
        {
            delay = 100ms;
        }
    submodules:
        cloud: Cloud {
            @display("p=900,100;r=480");
        }
        host: Host {
            @display("p=500,500;r=70");
        }
        computer: Computer {
            @display("p=500,315;r=140");
        }

    connections:
        host.gate[0] <--> Channel <--> cloud.gate[0];
        host.gate[1] <--> Channel <--> computer.gate[1];
        computer.gate[0] <--> Channel <--> cloud.gate[1];
}