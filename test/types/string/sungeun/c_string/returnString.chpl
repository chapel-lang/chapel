use checkType;

proc rcs() {
  var s = c"hi";
  var ss = createStringWithNewBuffer(s) + createStringWithNewBuffer(s);
  var cs = ss.c_str();
  return cs;
}

checkType(c_string, rcs().type);

proc rcss():string {
  var s = c"hi";
  var ss = createStringWithNewBuffer(s) + createStringWithNewBuffer(s);
  var cs = ss.c_str();
  return createStringWithNewBuffer(cs);
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

