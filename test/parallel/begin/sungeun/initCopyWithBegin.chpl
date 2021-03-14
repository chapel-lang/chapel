use Time;

record R {
  var x = 13.7;
}

var glob: real;

pragma "init copy fn"
proc chpl__initCopy(r: R) {
  begin with (ref glob) glob = getCurrentTime();
  return r;
}

var r0: R;
var r1 = r0;
