use checkType;
use CTypes;

proc rcs() {
  var s = c_ptrToConst_helper("hi");
  var ss = string.createCopyingBuffer(s) + string.createCopyingBuffer(s);
  var cs = c_ptrToConst_helper(ss);
  return cs;
}

checkType(c_ptrConst(c_char), rcs().type);

proc rcss():string {
  var s = c_ptrToConst_helper("hi");
  var ss = string.createCopyingBuffer(s) + string.createCopyingBuffer(s);
  var cs = c_ptrToConst_helper(ss);
  return string.createCopyingBuffer(cs);
}

checkType(rcss().type);

proc rsl() { return "hi"; }

checkType(rsl().type);

proc rsls():string { return "hi"; }

checkType(rsls().type);

proc rs(t: bool) {
  if t then
    return "blah";
  else
    return "blah":string;
}

checkType(rs(true).type);
checkType(rs(false).type);

