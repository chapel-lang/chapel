record R {
  type A;
}

var r = new R(int);
var A = [1,2,3];
param p = 1;

// each of these should be an error
      ref a = r.A;
const ref b = r.A;
      ref e = A.type;
const ref f = A.type;
      ref g = p;
const ref h = p;

writeln(a);
writeln(b);
