record R {
  var x: int = 0;
}

var printcopies = true;

pragma "auto copy fn"
proc chpl__autoCopy(arg: R) {

  // TODO - is no auto destroy necessary here?
  pragma "no auto destroy"
  var ret: R;

  ret.x = arg.x;

  if printcopies then
    writeln("copy/assign from ", arg.x);

  return ret;
}

// I'd like this to be ref, but that breaks
//    var outerX: R; begin { var x = outerX; }
pragma "init copy fn"
proc chpl__initCopy(arg: R) {
  // TODO - is no auto destroy necessary here?
  pragma "no auto destroy"
  var ret: R;

  ret.x = arg.x;

  if printcopies then
    writeln("copy/assign from ", arg.x);

  return ret;
}

proc =(ref lhs: R, rhs: R) {
  if printcopies then
    writeln("copy/assign from ", rhs.x);
  lhs.x = rhs.x;
}

proc R.writeThis(writer) throws {
  writer.write(x);
}


