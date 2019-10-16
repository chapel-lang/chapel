use decls;
use checkType;

// Note: c strings use param operations for string and then
// the result is coerceible back to c string. This avoids some
// ambiguities.
checkType(string, (c"8"+n:string).type);
checkType(string, ("8"+n:string).type);
checkType(string, (cstr+n:string).type);
checkType(string, (vcstr:string+n:string).type);
checkType(string, ("8"+nn:string).type);
checkType(string, (cstr:string+nn:string).type);
checkType(string, (vcstr:string+nn:string).type);

checkType(string, ("8"+r:string).type);   // no param c_string cast from real
checkType(string, (cstr:string+r:string).type);  // no param c_string cast from real
checkType(string, (vcstr:string+r:string).type);
checkType(string, ("8"+rr:string).type);
checkType(string, (cstr:string+rr:string).type);
checkType(string, (vcstr:string+rr:string).type);

checkType(string, ("8"+i:string).type);   // no param c_string cast from imag
checkType(string, (cstr:string+i:string).type);  // no param c_string cast from imag
checkType(string, (vcstr:string+i:string).type);
checkType(string, ("8"+ii:string).type);
checkType(string, (cstr:string+ii:string).type);
checkType(string, (vcstr:string+ii:string).type);

checkType(string, ("8"+c:string).type);   // no param complex
checkType(string, (cstr:string+c:string).type);
checkType(string, (vcstr:string+c:string).type);

checkType(string, (c"8"+e:string).type);
checkType(string, ("8"+e:string).type);
checkType(string, (cstr+e:string).type);
checkType(string, (vcstr:string+e:string).type);
checkType(string, ("8"+ee:string).type);
checkType(string, (cstr:string+ee:string).type);
checkType(string, (vcstr:string+ee:string).type);

checkType(string, (c"8"+b:string).type);
checkType(string, ("8"+b:string).type);
checkType(string, (cstr+b:string).type);
checkType(string, (vcstr:string+b:string).type);
checkType(string, ("8"+bb:string).type);
checkType(string, (cstr:string+bb:string).type);
checkType(string, (vcstr:string+bb:string).type);

