`ifdef SHORTESTPATH
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
/*	initial begin
	//Routing Table needs to be initialized here
//		$sformat(pathString,"%s %d %s",str1,INDEX);
		for(i = 0; i < N; i = i + 1)
			RoutingTable[0][i] = 0;//We can populate Routing Table from a file as well
		$readmemb({"/media/madhur/CommonSpace/Work/SystemSimulators/NoC Simulator/NoC_Netlist_Generator/Router/Mesh22/RoutingTable/Node", (INDEX + 48), ".mem"}, RoutingTable);
	end*/
	
	initial begin
	//Routing Table needs to be initialized here
		$sformat(pathString,"/media/madhur/SanDisk/Router/RoutingTable/Node%0d", index);
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
`endif

`ifdef XY
module HeadFlitDecoder #(
			parameter N = 4,	//This is the number of nodes in the network
			parameter INDEX = 1,
			parameter DATA_WIDTH = 8,
			parameter PhitPerFlit = 2,
			parameter REQUEST_WIDTH = 2
			)
	(
	input [PhitPerFlit * DATA_WIDTH - 1 : 0] HeadFlit,
	output reg [REQUEST_WIDTH - 1 : 0] RequestMessage = 0
	);
	
	localparam DIM = $floor($sqrt(N));//Dimension is SQRT(N) X SQRT(N)
	
	//Finding X and Y cordinates of the current Node.
	localparam X = INDEX % DIM;
	localparam Y = INDEX / DIM;

	//Routing Table declaration:
	//As of now it is just a huge register that will store routing info.
	//It should be implemented as a Memory(DRAM preferably) to reduce resource utilization
	reg [REQUEST_WIDTH * N - 1 : 0] RoutingTable[0 : 0];
	reg [8 * 256 - 1 : 0] pathString;

	wire [3 : 0] DestinationX, DestinationY;
	assign DestinationX = HeadFlit[4 +: 4];
	assign DestinationY = HeadFlit[0 +: 4];
	
	//Right now, it is completely asynchronous, but when it is made using memory,
	//it will have clock as well and arbitration unit too.
	always @(*)begin
		if((X == 0 | X == (DIM - 1)) && (Y == 0 | Y == (DIM - 1)))
			if(DestinationX == X && DestinationY == Y)
				RequestMessage = 0;//If DX = X and DY = Y, then we are at the destination.
			else if(DestinationX == X)
				RequestMessage = 2;
			else if(DestinationY == Y)
				RequestMessage = 1;
			else RequestMessage = 1;//X first
			
		else if((X == 0 | X == (DIM - 1)))
			if(DestinationX == X && DestinationY == Y)
				RequestMessage = 0;//If DX = X and DY = Y, then we are at the destination.
			else if(DestinationX == X)
				if(DestinationY > Y)
					RequestMessage = 2;
				else RequestMessage = 3;
			else if(DestinationY == Y)
				RequestMessage = 1;
			else RequestMessage = 1;//X first
		
		else if((Y == 0 | Y == (DIM - 1)))
			if(DestinationX == X && DestinationY == Y)
				RequestMessage = 0;//If DX = X and DY = Y, then we are at the destination.
			else if(DestinationX == X)
				RequestMessage = 2;
			else if(DestinationY == Y)
				if(DestinationX > X)
					RequestMessage = 1;
				else RequestMessage = 3;
			else if(DestinationX > X)
					RequestMessage = 1;
				else RequestMessage = 3;//X first
		else
			if(DestinationX == X && DestinationY == Y)
				RequestMessage = 0;//If DX = X and DY = Y, then we are at the destination.
			else if(DestinationX == X)
				if(DestinationY > Y)
					RequestMessage = 2;
				else RequestMessage = 4;
			else if(DestinationY == Y)
				if(DestinationX > X)
					RequestMessage = 1;
				else RequestMessage = 3;
			else if(DestinationX > X)
					RequestMessage = 1;
				else RequestMessage = 3;//X first
	end
	
endmodule
`endif
