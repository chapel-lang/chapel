use Time;

proc foo(amSerial: bool, u: uint) {
  writeln("foo called (", amSerial, ") with ", u);
}

for i in 0..1 do serial i:bool {
  cobegin {
    foo(i:bool, 2);
    foo(i:bool, 1);
  }

  coforall u in 1..2:uint by -1 do
    foo(i:bool, u);

  begin foo(i:bool, 2);
  begin foo(i:bool, 1);
}
