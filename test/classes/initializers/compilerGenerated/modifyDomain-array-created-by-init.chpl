// Note: this test will fail with default constructors, because the domain is
// copied and thus decoupled from the array that tries to rely on it.
class C {
  var D;
  var A: [D] int;
}

var arr1 = [1, 3, 5, 7];
var c1 = new C(arr1.domain);
writeln(c1);

var arr2 = [2, 4, 6];
var c2 = new C(arr2.domain);
writeln(c2);
c2.D = {1..4};
c2.A[4] = 8;
writeln(c2);
