
record myR {
  var base: c_string;
}

pragma "init copy fn"
inline proc chpl__initCopy(ref r: myR) {
  var ret: myR;
  ret.base = r.base;
  return ret;
}

pragma "auto copy fn"
inline proc chpl__autoCopy(ref r: myR) {
  var ret: myR;
  ret.base = r.base;
  return ret;
}


var s0: myR;
s0.base = c"s0";
sync {
  begin writeln(string.createCopyingBuffer(s0.base));
}

use CTypes;
record myRptr {
  var base: c_ptrConst(c_uchar);
}

pragma "init copy fn"
inline proc chpl__initCopy(ref r: myRptr) {
  var ret: myRptr;
  ret.base = r.base;
  return ret;
}

pragma "auto copy fn"
inline proc chpl__autoCopy(ref r: myRptr) {
  var ret: myRptr;
  ret.base = r.base;
  return ret;
}


var s1: myRptr;
s1.base = c_ptrToConst_helper("s1");
sync {
  begin writeln(string.createCopyingBuffer(s1.base));
}

