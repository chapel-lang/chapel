use CodeletDist;

proc main
{
	var N = [0..9] dmapped CodeletDist();
	forall i in N do
		writeln("Hello from " + i);
}
