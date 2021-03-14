// Variation that takes a domain and creates the array field from it.
class C {
  var D;
  var A: [D] int;

  proc init(dom) {
    D = dom;
  }
}

var arr1 = [1, 3, 5, 7];
var c1 = new owned C(arr1.domain);
writeln(c1);

var arr2 = [2, 4, 6];
var c2 = new owned C(arr2.domain);
writeln(c2);
c2.D = {0..3};
c2.A[3] = 8;
writeln(c2);
