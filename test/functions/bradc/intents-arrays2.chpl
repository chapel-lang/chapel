config var n: integer = 6;
var D: domain(1) = (1..n);
var A: [D] float;

function callin(in x) {
  writeln("in callin, x is: ", x);
  forall i in D {
    x(i) += 1.0;
  }
  writeln("re-assigned to be: ", x);
}


function callout(out x) {
  writeln("in callout, x is: ", x);
  forall i in D {
    x(i) = i;
  }
  writeln("re-assigned to be: ", x);
}


function callinout(inout x) {
  writeln("in callinout, x is: ", x);
  forall i in D {
    x(i) += 1.0;
  }
  writeln("re-assigned to be: ", x);
}


function callblank(x) {
  writeln("in callblank, x is: ", x);
  forall i in D {
    x(i) += 1.0;
  }
  writeln("re-assigned to be: ", x);
}


function main() {

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
