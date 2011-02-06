use Time;

// Test multiple threads waiting at the same time.  Similar to simple3.chpl,
// but use cobegin instead.

var go: single int;

proc jam( i: int) {
  writeln( i, " Pizza!");
}

begin cobegin {
  jam( go);
  jam( go);
  jam( go);
  jam( go);
  jam( go);
}

writeln( "I'm sleepy");
sleep( 3);
writeln( "I'm awake");
go = 9;
