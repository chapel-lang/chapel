use Time;

proc foo(u: uint) {
  sleep(u);
  writeln("foo called with delay ", u);
}

for i in 0..1 do serial i:bool {
  cobegin {
    foo(2);
    foo(1);
  }

  coforall u in 1..2:uint by -1 do
    foo(u);

  begin foo(2);
  begin foo(1);
}
