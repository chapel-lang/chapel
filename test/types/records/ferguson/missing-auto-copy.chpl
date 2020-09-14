record R {
  var x: int = 0;
}
// I'd like this to be ref, but that breaks
//    var outerX: R; begin { var x = outerX; }
pragma "init copy fn"
proc chpl__initCopy(arg: R, definedConst: bool) {
  // TODO - is no auto destroy necessary here?
  pragma "no auto destroy"
  var ret: R;

  ret.x = arg.x + 1;

  writeln("initCopy");

  return ret;
}

proc =(ref lhs: R, rhs: R) {
  writeln("assign");
  lhs.x = rhs.x;
}

proc doAutoCopyIt(ref x)
{
  return x;
}
proc main() {
  var r = new R(1);
  var a = doAutoCopyIt(r);
  writeln(a);
}

