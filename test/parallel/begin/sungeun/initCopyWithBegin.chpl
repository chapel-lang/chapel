use Time;

record R {
  var x = 13.7;
}

pragma "init copy fn"
proc chpl__initCopy(r: R) {
  begin ref(r) r.x = getCurrentTime();
  return r;
}

var r0: R;
var r1 = r0;
