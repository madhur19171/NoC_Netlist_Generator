module HeadFlitDecoder #(
			parameter N = 4,	//This is the number of nodes in the network
			parameter INDEX = 1,
			parameter DATA_WIDTH = 8,
			parameter PhitPerFlit = 2,
			parameter REQUEST_WIDTH = 2
			)
	(
	input [PhitPerFlit * DATA_WIDTH - 1 : 0] HeadFlit,
	output [REQUEST_WIDTH - 1 : 0] RequestMessage
	);

	//Routing Table declaration:
	//As of now it is just a huge register that will store routing info.
	//It should be implemented as a Memory(DRAM preferably) to reduce resource utilization
	reg [REQUEST_WIDTH * N - 1 : 0] RoutingTable[0 : 0];
	reg [8 * 256 - 1 : 0] pathString;
	integer index = INDEX;
	
	integer i;
//	initial begin
//	//Routing Table needs to be initialized here
//		$sformat(pathString,"%s %d %s",str1,INDEX);
//		for(i = 0; i < N; i = i + 1)
//			RoutingTable[0][i] = 0;//We can populate Routing Table from a file as well
//		$readmemb({"/media/madhur/CommonSpace/Work/SystemSimulators/NoC Simulator/NoC_Netlist_Generator/Verilog/RoutingTable/Node", (INDEX + 48)}, RoutingTable);
//	end
	
	initial begin
	//Routing Table needs to be initialized here
		$sformat(pathString,"/media/madhur/CommonSpace/Work/SystemSimulators/NoC Simulator/NoC_Netlist_Generator/Verilog/RoutingTable/Node%0d", index);
		for(i = 0; i < N; i = i + 1)
			RoutingTable[0][i] = 0;//We can populate Routing Table from a file as well
		$readmemb(pathString, RoutingTable);
	end

	wire [$clog2(N) - 1 : 0] Destination;
	
	assign Destination = HeadFlit[0 +: $clog2(N)];
	
	//Right now, it is completely asynchronous, but when it is made using memory,
	//it will have clock as well and arbitration unit too.
	assign RequestMessage = RoutingTable[0][REQUEST_WIDTH * Destination +: REQUEST_WIDTH];
	
endmodule

