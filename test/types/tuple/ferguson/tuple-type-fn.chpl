proc typefn(type rangeType) type {
  return (rangeType, );
}

proc run() {
  var x = 1..10;

  type t = typefn(x.type);

  var y:t;
  writeln(y);
}

run();
