config const n:uint(64) = 10000;

var x:uint(64);

forall i in 1..n with (ref x) { // data race does not matter
  x += i;
}

writeln("Done");
