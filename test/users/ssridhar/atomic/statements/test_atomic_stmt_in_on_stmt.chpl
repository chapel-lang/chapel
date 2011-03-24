var x: uint(64);

proc main() {
  coforall i in 1..1000 do
    on x.locale do atomic x = x + 1;
  writeln("x = ", x);
}

