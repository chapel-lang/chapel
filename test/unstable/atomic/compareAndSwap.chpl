var ab: atomic bool;
var ai: atomic int;

ab.compareAndSwap(false, true);
ai.compareAndSwap(0, 1);

writeln(ab);
writeln(ai);
