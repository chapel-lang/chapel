var x: uint(64);

def main() {
  bar();
}

def bar() {
  var i, j: int;
  coforall i in 1..10 {
    atomic {
      x = x + 1;
      foo();
      j += 1;
    }
  }
}

def foo() {
  x = x + 1;
}