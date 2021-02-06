record R {
  var x: int;
}

record S {
  var y: int;
}

var hettup = (new R(1), new S(2));

byDefaultIntent(hettup);

proc byDefaultIntent(tupArg) {
  for ttt in tupArg do
    writeln(ttt);
}
