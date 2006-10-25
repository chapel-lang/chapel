class bar {
  implements reduction;
}

def main() {
  class reduction {
  } -- override built-in reduction class?

  class foo {
    implements reduction;
  }

  var D: domain = (1..m, 1..n);
  var A: [D] real;
  var a: real;

  a = max reduce A;
  a = bar reduce A;
  a = foo reduce A;
}

