// Reported by a customer on 11/26/2014

use Random;
class A {
  var X: domain(int);

  proc init(N: int) {
    var R = new randomStream(real, 13);
    this.X = [x in 1..N] x;
    var y: [this.X] real;
    for i in this.X do y[i] = R.next();
  }
}
var ownA = new owned A(100);
var a = ownA.borrow();
