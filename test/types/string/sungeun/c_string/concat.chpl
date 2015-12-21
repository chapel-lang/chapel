use decls;
use checkType;

checkType(c_string, (c"8"+n).type);
checkType(string, ("8"+n).type);
checkType(c_string, (cstr+n).type);
checkType(string, (vcstr+n).type);
checkType(string, ("8"+nn).type);
checkType(string, (cstr+nn).type);
checkType(string, (vcstr+nn).type);

checkType(string, ("8"+r).type);   // no param c_string cast from real
checkType(string, (cstr+r).type);  // no param c_string cast from real
checkType(string, (vcstr+r).type);
checkType(string, ("8"+rr).type);
checkType(string, (cstr+rr).type);
checkType(string, (vcstr+rr).type);

checkType(string, ("8"+i).type);   // no param c_string cast from imag
checkType(string, (cstr+i).type);  // no param c_string cast from imag
checkType(string, (vcstr+i).type);
checkType(string, ("8"+ii).type);
checkType(string, (cstr+ii).type);
checkType(string, (vcstr+ii).type);

checkType(string, ("8"+c).type);   // no param complex
checkType(string, (cstr+c).type);
checkType(string, (vcstr+c).type);

checkType(c_string, (c"8"+e).type);
checkType(string, ("8"+e).type);
checkType(c_string, (cstr+e).type);
checkType(string, (vcstr+e).type);
checkType(string, ("8"+ee).type);
checkType(string, (cstr+ee).type);
checkType(string, (vcstr+ee).type);

checkType(c_string, (c"8"+b).type);
checkType(string, ("8"+b).type);
checkType(c_string, (cstr+b).type);
checkType(string, (vcstr+b).type);
checkType(string, ("8"+bb).type);
checkType(string, (cstr+bb).type);
checkType(string, (vcstr+bb).type);

