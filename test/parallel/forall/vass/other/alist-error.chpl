// Reported by a customer on 11/26/2014

use Random;
class A {
  var X: domain(int);

  proc init(N: int) {
    var R = new borrowed RandomStream(real, 13);
    this.X = [x in 1..N] x;
    var Y = [x in this.X] R.getNext();
  }
}
var a = new borrowed A(100);
