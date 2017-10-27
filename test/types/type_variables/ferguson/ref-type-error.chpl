record R {
  type A;
}

var r = new R(int);
var A = [1,2,3];

// each of these should be an error
      ref a = r.A;
const ref b = r.A;
      ref e = A.type;
const ref f = A.type;

writeln(a);
writeln(b);
