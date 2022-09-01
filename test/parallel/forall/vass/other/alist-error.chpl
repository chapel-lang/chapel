// Reported by a customer on 11/26/2014

use Random;
class A {
  var X: domain(int);

  proc init(N: int) {
    var R = (new owned RandomStream(real, 13)).borrow();
    this.X = [x in 1..N] x;
    var Y = [x in this.X] R.getNext();
  }
}
var a = (new owned A(100)).borrow();
