use PrivatizationWrappers;
use Time;

config const numIters = 1000000;

forall i in 1 .. numIters {
	insertPrivatized(new object(), i);
}

var t : Timer;
t.start();
for i in 1 .. numIters {
	var c = getPrivatized(i);
}
t.stop();
writeln(t.elapsed() / numIters);