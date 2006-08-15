config var n: int = 6;
var D: domain(1) = (1..n);

def callin(in x) {
  writeln("in callin, x is: ", x);
  forall i in D {
    x(i) += 1.0;
  }
  writeln("re-assigned to be: ", x);
}


def callout(out x) {
  forall i in D {
    x(i) = i;
  }
  writeln("re-assigned to be: ", x);
}


def callinout(inout x) {
  writeln("in callinout, x is: ", x);
  forall i in D {
    x(i) += 1.0;
  }
  writeln("re-assigned to be: ", x);
}


def callblank(x) {
  writeln("in callblank, x is: ", x);
  forall i in D {
    x(i) += 1.0;
  }
  writeln("re-assigned to be: ", x);
}


def main() {
  var A: [D] float;

  forall i in D {
    A(i) = i;
  }

  callin(A);
  writeln("back at callsite, A is: ", A);
  writeln();

  callout(A);
  writeln("back at callsite, A is: ", A);
  writeln();

  callinout(A);
  writeln("back at callsite, A is: ", A);
  writeln();

  callblank(A);
  writeln("back at callsite, A is: ", A);
}
