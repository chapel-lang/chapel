var x: uint(64);

proc main() {
  x = 0; 
  writeln("Before foo x = ", x);
  coforall i in 1..1000 {
    atomic {
      foo(x);  
    }
  }
  writeln("After foo x = ", x);
}

proc foo(inout y: uint(64)) {
  y = y + 1;
}
