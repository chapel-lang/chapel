
proc test1(d: domain(?r,?IT,?)) {
  writeln(d);
}

proc test2(d: domain(?r, ?IT, ?)): domain(r, IT, true) {
  return d by 1;
}

test1([1..2]);
writeln(test2([1..2]));
