pragma "safe"
module yy {

proc identity2(ref x:int, ref y:int) ref {
  return x;
}

proc bar1(ref x: int) ref {
  var y:int;
  return identity2(x,y);
}

proc bar2(ref x: int) ref {
  var y:int;
  return identity2(y,x);
}


proc test() {
  var arg:int;
  ref r1 = bar1(arg);
  ref r2 = bar2(arg);
  writeln(r1);
  writeln(r2);
}

test();

}
