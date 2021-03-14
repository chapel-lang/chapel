const rAll = 1..10;

writeln("rAll is: ", rAll);

var rAllSlice = rAll(3..8 by 2);

writeln("rAllSlice = ", rAllSlice);

rAllSlice = rAll(2..8 by 2);

writeln("rAllSlice = ", rAllSlice);

rAllSlice = rAll(3..8);

writeln("rAllSlice = ", rAllSlice);

rAllSlice = rAll(2..8);

writeln("rAllSlice = ", rAllSlice);

rAllSlice = rAll(0..11);

writeln("rAllSlice = ", rAllSlice);
