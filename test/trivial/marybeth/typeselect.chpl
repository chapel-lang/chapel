proc norm(x: [], p) where x.rank == 1 {
  select p.type {
    when int {
      if (p == 2) {
        return sqrt(+ reduce (x*x));
      } else if (p == 1) {
        return + reduce abs(x);
      } else {
        return -1;
        writeln("norm not implemented for p > 2");
      }
    }
    when string {
      if ((p == 'inf') || (p == 'INF') || (p == 'Inf')) {
        return max reduce abs(x);
      } else {
        return -1;
        writeln("invalid norm type");
      }
    }
    otherwise {
      return -1 ;
      writeln("invalid norm type");
    }
  }
}

const D1 = {1..5};
var a:[D1] real;
a = 2.0;
writeln(a);
writeln(norm(a,1));
writeln(norm(a,2));
writeln(norm(a,'inf'));
