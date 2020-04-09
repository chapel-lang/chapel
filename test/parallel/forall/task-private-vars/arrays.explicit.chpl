// #11422 Task-private arrays.

config const n = 8;

const DDD = {1..n};
const Dtpv = {1..3};

proc main {

  forall idx1 in DDD with (var ARR1: [Dtpv] int) {
    ARR1 += 1;
    writeln(ARR1);
  }
  writeln(5);

  forall idx6 in DDD with (var ARR6: [Dtpv] int = 600) {
    ARR6 += 1;
    writeln(ARR6);
  }
  writeln(660);

  forall idx7 in DDD with (var ARR7 = [ 700 ]) {
    ARR7 += 1;
    writeln(ARR7);
  }
  writeln(770);

  // look ma, I can have them all at once
  forall idx7 in DDD with (
    var ARR9a: [Dtpv] int,
    var ARR9b: [Dtpv] int = 900,
    var ARR9c = [ 900 ]
  ) {
    ARR9a += 1;
    ARR9b += 2;
    ARR9c += 3;
    writeln(ARR9c, "  ", ARR9b, "  ", ARR9a);
  }
  writeln(999);

}
