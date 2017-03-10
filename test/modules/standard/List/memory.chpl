use Memory;
use List;

proc output()
{
	var ls : list(int);
	ls.push_back(5);
	ls.push_back(8);
	ls.push_back(10);
	ls.push_back(12);
	ls.push_back(15);
	writeln(memoryUsed());
}

memTrack;
writeln(memoryUsed());
output();
writeln(memoryUsed());
