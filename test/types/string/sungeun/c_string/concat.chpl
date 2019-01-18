use decls;
use checkType;

// Note: c strings use param operations for string and then
// the result is coerceible back to c string. This avoids some
// ambiguities.
checkType(string, (c"8"+n).type);
checkType(string, ("8"+n).type);
checkType(string, (cstr+n).type);
checkType(string, (vcstr:string+n).type);
checkType(string, ("8"+nn).type);
checkType(string, (cstr:string+nn).type);
checkType(string, (vcstr:string+nn).type);

checkType(string, ("8"+r).type);   // no param c_string cast from real
checkType(string, (cstr:string+r).type);  // no param c_string cast from real
checkType(string, (vcstr:string+r).type);
checkType(string, ("8"+rr).type);
checkType(string, (cstr:string+rr).type);
checkType(string, (vcstr:string+rr).type);

checkType(string, ("8"+i).type);   // no param c_string cast from imag
checkType(string, (cstr:string+i).type);  // no param c_string cast from imag
checkType(string, (vcstr:string+i).type);
checkType(string, ("8"+ii).type);
checkType(string, (cstr:string+ii).type);
checkType(string, (vcstr:string+ii).type);

checkType(string, ("8"+c).type);   // no param complex
checkType(string, (cstr:string+c).type);
checkType(string, (vcstr:string+c).type);

checkType(string, (c"8"+e).type);
checkType(string, ("8"+e).type);
checkType(string, (cstr+e).type);
checkType(string, (vcstr:string+e).type);
checkType(string, ("8"+ee).type);
checkType(string, (cstr:string+ee).type);
checkType(string, (vcstr:string+ee).type);

checkType(string, (c"8"+b).type);
checkType(string, ("8"+b).type);
checkType(string, (cstr+b).type);
checkType(string, (vcstr:string+b).type);
checkType(string, ("8"+bb).type);
checkType(string, (cstr:string+bb).type);
checkType(string, (vcstr:string+bb).type);

