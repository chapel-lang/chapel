// Reported by a customer on 11/26/2014

use Random;
class A {
  var X: domain(int);
  proc A(N: int) {
    var R = new RandomStream(13);
    this.X = [x in 1..N] x;
    var Y = [x in this.X] R.getNext();
    delete R;
  }
}
var a = new A(100);
delete a;
