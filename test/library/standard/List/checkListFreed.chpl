use Memory;
use List;

var mem : uint;
proc output()
{
	mem=memoryUsed();
	var ls : list(int);
	ls.push_back(5);
	ls.push_back(8);
	ls.push_back(10);
	ls.push_back(12);
	ls.push_back(15);
}

memTrack;
writeln(memoryUsed());
output();
writeln(memoryUsed()-mem);
