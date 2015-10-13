use checkType;

const s = "0123456789";

checkType(s[-1].type);
checkType(s[5].type);
checkType(s[6..9].type);

///// These tests no longer work after .substring() was renamed as [] ('this').
// Chapel does not have explicit operator invocation syntax, so
// <string-literal> '[]' results in a syntax error.
//checkType("0123456789"[-1].type);
//checkType("0123456789"[5].type);
//checkType("0123456789"[6..9].type);

const cs = s.c_str();

// The substring method is still implemented for c_string objects.
// The interface has changed, so now c_string.substring() returns a c_string_copy.
checkType(c_string_copy, cs.substring(-1).type);
checkType(c_string_copy, cs.substring(5).type);
checkType(c_string_copy, cs.substring(6..9).type);
