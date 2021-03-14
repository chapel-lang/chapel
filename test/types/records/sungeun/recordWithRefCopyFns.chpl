
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
  begin writeln(createStringWithNewBuffer(s0.base));
}
