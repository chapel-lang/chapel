{
  record Arrays {
    var D: domain(1);
    var A: [D] string;
    forwarding A;
  }

  var keysArrays: [1..4] Arrays;
}
