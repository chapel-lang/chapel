// Reported by a customer on 11/26/2014

use Random;
class A {
  var X: domain(int);

  proc init(N: int) {
    var ownR = new owned RandomStream(real, 13);
    var R = ownR.borrow();
    this.X = [x in 1..N] x;
    var Y = [x in this.X] R.getNext();
  }
}
var ownA = new owned A(100);
var a = ownA.borrow();
