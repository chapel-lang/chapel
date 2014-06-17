proc foo(x: range(?eltType,BoundedRangeType.bounded,?stridable)) {
  writeln(x);
}

foo(1..5 by 2);
foo(1..5);
