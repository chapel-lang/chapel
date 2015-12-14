use checkType;

proc rcs() {
  var s:c_string = "hi";
  s += s;
  return s;
}

checkType(c_string, rcs().type);

proc rcss():string {
  var s:c_string = "hi";
  s += s;
  return s;
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

