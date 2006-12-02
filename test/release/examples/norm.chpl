module Norm{
  enum normType {norm1, norm2, normInf, normFrob};

  def norm(x: [], p: normType) where x.rank == 1 {
    select (p) {
      when norm1 do return + reduce abs(x);
      when norm2 do return sqrt(+ reduce (x*x));
      when normInf do return max reduce abs(x);
      when normFrob do halt("From-norm not defined for 1D arrays");
      otherwise halt("Unexpected norm type");
    }
  }

  def norm(x:[?D], p: normType) where x.rank == 2 {
    select (p) {
      when norm1 do
        return max reduce [j in D(2)] (+ reduce abs(x[D(1), j]));

      when norm2 do
        halt("Haven't implemented 2-norm for 2D arrays yet");

      when normInf do
        return max reduce [i in D(1)] (+ reduce abs(x[i, D(2)]));

      when normFrob do return sqrt(+ reduce abs(x));

      otherwise halt("Unexpected norm type");
    }
  }

  def norm(x:[?D], p: normType) where x.rank > 2 {
    compilerError("Norms not implemented for array ranks > 2D");
  }

  def norm(x: []) {
    select (x.rank) {
      when 1 do return norm(x, norm2);
      when 2 do return norm(x, normFrob);
      otherwise compilerError("Norms not implemented for array ranks > 2D");
    }
  }
}


module TestNorm {
  use Norm;

  def main() {
    const D1 = [1..5];
    var a:[D1] real;
    a = 2.0;
    writeln("Test of vector norms.  x = ");
    writeln(a);
    writeln("1-norm = ", norm(a, norm1));
    writeln("2-norm = " , norm(a, norm2));
    writeln("infinity norm = ", norm(a, normInf));
    writeln("default norm = ", norm(a));
    writeln();

    const D2 = [1..2,1..2];
    var b:[D2] real;
    b = 2.0;
    writeln("Test of matrix norms.  A = ");
    writeln(b);
    writeln("1-norm = ", norm(b, norm1));
    writeln("infinity norm = ", norm(b, normInf));
    writeln("frobenius norm = ", norm(b, normFrob));
    writeln("default norm = ", norm(b));
    writeln();

    const D3 = [1..3,1..2];
    var c:[D3] real;
    c = 2.0;
    writeln("Test of matrix norms.  A = ");
    writeln(c);
    writeln("1-norm = " , norm(c, norm1));
    writeln("infinity norm = ", norm(c, normInf));
    writeln("frobenius norm = ", norm(c, normFrob));
    writeln("default norm = ", norm(c));
    writeln();

    var d:[D2] int;
    d = 1;
    writeln("Test of matrix norms.  A = ");
    writeln(d);
    writeln("1-norm = ", norm(d, norm1));
    writeln("infinity norm = ", norm(d, normInf));
    writeln("frobenius norm = ", norm(d, normFrob));
    writeln("default norm = ", norm(d));
    writeln();

    var e:[D2] complex;
    e = 1.0 + 1.0i;
    writeln("Test of matrix norms.  A = ");
    writeln(e);
    writeln("1-norm = ", norm(e, norm1));
    writeln("infinity norm = ", norm(e, normInf));
    writeln("frobenius norm = ", norm(e, normFrob));
    writeln("default norm = ", norm(e));
    writeln();
  }
}
