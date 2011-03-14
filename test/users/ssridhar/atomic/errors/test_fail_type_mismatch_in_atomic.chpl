var x: uint(64);
proc main() {
  var y: int; 
  coforall i in 1..10 do
    atomic {
      x = x + 1;
      y = x;
    }
  writeln(x);
}

