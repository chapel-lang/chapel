var x: int;

proc main() {
  coforall i in 1..1000 do
    atomic on x.locale do x = x + 1;
  writeln("x = ", x);
}

