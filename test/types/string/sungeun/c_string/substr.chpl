use checkType;

const s = "0123456789";

checkType(s.substring(-1).type);
checkType(s.substring(5).type);
checkType(s.substring(6..9).type);

checkType(c_string, "0123456789".substring(-1).type);
checkType(c_string, "0123456789".substring(5).type);
checkType(c_string, "0123456789".substring(6..9).type);

const cs = s.c_str();

checkType(c_string, cs.substring(-1).type);
checkType(c_string, cs.substring(5).type);
checkType(c_string, cs.substring(6..9).type);
