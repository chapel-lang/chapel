class bar {
  implements reduction;
}

function main() {
  class reduction {
  } -- override built-in reduction class?

  class foo {
    implements reduction;
  }

  var D: domain = (1..m, 1..n);
  var A: [D] float;
  var a: float;

  a = max reduce A;
  a = bar reduce A;
  a = foo reduce A;
}

