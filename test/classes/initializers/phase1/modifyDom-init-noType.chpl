// Variation that takes in the array field and sets the domain field based on
// it, but the domain field is generic.
class C {
  var D;
  var A: [D] int;

  proc init(arr: [] int) {
    D = arr.domain;
    A = arr;
  }
}

var arr1 = [1, 3, 5, 7];
var c1 = new owned C(arr1);
writeln(c1);

var arr2 = [2, 4, 6];
var c2 = new owned C(arr2);
writeln(c2);
c2.D = {0..3};
c2.A[3] = 8;
writeln(c2);
