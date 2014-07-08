

proc test() {
  if 2:bool then writeln("yes");
  var a = 18445618173802865439;
  var b = 206880;
  if !((a & b):bool) {
    writeln("no");
  } else {
    writeln("yes");
  }
}

test();
