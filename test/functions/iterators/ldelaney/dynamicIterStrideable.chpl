use DynamicIters;
proc main(){
	var r1 = 1..10 by 2;
	var r2 = 7..8;
	var r3 = -2..-1;
	var d1 = {r1, r2, r3};
	var bigArr : [r1][r2][r3] int;

	forall ((x, y, z), xyz) in zip(dynamic(d1, 2), d1) do {
		var sum : int = x + y + z;
		bigArr[x][y][z] = sum;
	}

	for (x, y, z) in d1 do{
		writeln("bigArr (", x, ", ", y, ", ", z, ") is ", bigArr[x][y][z]);
		assert(bigArr[x][y][z] == (x + y + z), "bigArr not properly set");
	}
}