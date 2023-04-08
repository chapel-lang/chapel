use decls;
use checkType;

// Note: c strings use param operations for string and then
// the result is coerceible back to c string. This avoids some
// ambiguities.
checkType(string, (c"8"+n:string).type);
checkType(string, ("8"+n:string).type);
checkType(string, (cstr+n:string).type);
checkType(string, (string.createWithNewBuffer(vcstr)+n:string).type);
checkType(string, ("8"+nn:string).type);
checkType(string, (string.createWithNewBuffer(cstr)+nn:string).type);
checkType(string, (string.createWithNewBuffer(vcstr)+nn:string).type);

checkType(string, ("8"+r:string).type);   // no param c_string cast from real
checkType(string, (string.createWithNewBuffer(cstr)+r:string).type);  // no param c_string cast from real
checkType(string, (string.createWithNewBuffer(vcstr)+r:string).type);
checkType(string, ("8"+rr:string).type);
checkType(string, (string.createWithNewBuffer(cstr)+rr:string).type);
checkType(string, (string.createWithNewBuffer(vcstr)+rr:string).type);

checkType(string, ("8"+i:string).type);   // no param c_string cast from imag
checkType(string, (string.createWithNewBuffer(cstr)+i:string).type);  // no param c_string cast from imag
checkType(string, (string.createWithNewBuffer(vcstr)+i:string).type);
checkType(string, ("8"+ii:string).type);
checkType(string, (string.createWithNewBuffer(cstr)+ii:string).type);
checkType(string, (string.createWithNewBuffer(vcstr)+ii:string).type);

checkType(string, ("8"+c:string).type);   // no param complex
checkType(string, (string.createWithNewBuffer(cstr)+c:string).type);
checkType(string, (string.createWithNewBuffer(vcstr)+c:string).type);

checkType(string, (c"8"+pe:string).type);
checkType(string, ("8"+pe:string).type);
checkType(string, (cstr+pe:string).type);
checkType(string, (string.createWithNewBuffer(vcstr)+pe:string).type);
checkType(string, ("8"+ee:string).type);
checkType(string, (string.createWithNewBuffer(cstr)+ee:string).type);
checkType(string, (string.createWithNewBuffer(vcstr)+ee:string).type);

checkType(string, (c"8"+b:string).type);
checkType(string, ("8"+b:string).type);
checkType(string, (cstr+b:string).type);
checkType(string, (string.createWithNewBuffer(vcstr)+b:string).type);
checkType(string, ("8"+bb:string).type);
checkType(string, (string.createWithNewBuffer(cstr)+bb:string).type);
checkType(string, (string.createWithNewBuffer(vcstr)+bb:string).type);

