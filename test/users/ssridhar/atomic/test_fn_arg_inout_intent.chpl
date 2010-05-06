var x: uint(64);

def main() {
  x = 0; 
  writeln("Before foo x = ", x);
  coforall i in 1..10 {
    atomic {
      foo(x);  
    }
  }
  writeln("After foo x = ", x);
}

def foo(inout y: uint(64)) {
  y = y + 1;
}
