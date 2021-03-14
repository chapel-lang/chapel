use decls;
use checkType;

checkType(2*string, ("hi", "bye").type);
checkType(2*c_string, (cstr, cstr).type);
checkType(2*c_string, (vcstr, vcstr).type);

checkType(1*string, ("hi", ).type);
checkType(1*c_string, (cstr, ).type);
checkType(1*c_string, (vcstr, ).type);

checkType((string, int(64)), ("hi", 2).type);
checkType((c_string, int(64)), (cstr, 2).type);
checkType((c_string, int(64)), (vcstr, 2).type);

checkType((int(64), string), (1, "hi").type);
checkType((int(64), c_string), (1, cstr).type);
checkType((int(64), c_string), (1, vcstr).type);

checkType((int(64), string, int(64)), (1, "hi", 3).type);
checkType((int(64), c_string, int(64)), (1, cstr, 3).type);
checkType((int(64), c_string, int(64)), (1, vcstr, 3).type);
