
record myR {
  var base: chpl_c_string;
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
s0.base = "s0";
sync {
  begin writeln(string.createCopyingBuffer(s0.base:c_ptrConst(c_char)));
}

use CTypes;
record myRptr {
  var base: c_ptrConst(c_char);
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
s1.base = "s1".c_str();
sync {
  begin writeln(string.createCopyingBuffer(s1.base));
}

