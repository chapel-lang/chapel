config var n: int = 6;
var D: domain(1) = {1..n};
var A: [D] real;

proc callin(in x) {
  writeln("in callin, x is: ", x);
  forall i in D {
    x(i) += 1.0;
  }
  writeln("re-assigned to be: ", x);
}


proc callout(out x) {
  var tmp: [D] real;
  x = tmp;
  writeln("in callout, x is: ", x);
  forall i in D {
    x(i) = i;
  }
  writeln("re-assigned to be: ", x);
}


proc callinout(inout x) {
  writeln("in callinout, x is: ", x);
  forall i in D {
    x(i) += 1.0;
  }
  writeln("re-assigned to be: ", x);
}


proc callblank(x) {
  writeln("in callblank, x is: ", x);
  forall i in D {
    x(i) += 1.0;
  }
  writeln("re-assigned to be: ", x);
}


proc main() {

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
