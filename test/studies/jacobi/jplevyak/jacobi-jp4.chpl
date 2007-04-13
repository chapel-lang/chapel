config const n = 4;
config const epsilon = 1.0e-5;
var D = [1..n, 1..n];
var t = [D.expand(1)] 0.0;
t[n+1,..] = 1.0;

const stencil = makeList((-1, -1), (-1, 1), (1, -1), (1, 1));

def jacobi(t) {
  var tt = t;
  var again = 0;
  [i in D] { 
    var t(i) = tt(i + stencil)/+ / 4.0; 
    again ||= abs(tt(i) - t(i)) > epsilon;
  }
  if (again) 
    jacobi(t);
}

jacobi(t);

writeln(t);
