use checkType;

const s = "0123456789";

checkType(s[-1].type);
checkType(s[5].type);
checkType(s[6..9].type);

checkType("0123456789"[-1].type);
checkType("0123456789"[5].type);
checkType("0123456789"[6..9].type);

const cs = s.c_str();

checkType(c_string, cs[-1].type);
checkType(c_string, cs[5].type);
checkType(c_string, cs[6..9].type);
