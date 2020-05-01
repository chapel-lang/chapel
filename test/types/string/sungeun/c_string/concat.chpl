use decls;
use checkType;

// Note: c strings use param operations for string and then
// the result is coerceible back to c string. This avoids some
// ambiguities.
checkType(string, (c"8"+n:string).type);
checkType(string, ("8"+n:string).type);
checkType(string, (cstr+n:string).type);
checkType(string, (createStringWithNewBuffer(vcstr)+n:string).type);
checkType(string, ("8"+nn:string).type);
checkType(string, (createStringWithNewBuffer(cstr)+nn:string).type);
checkType(string, (createStringWithNewBuffer(vcstr)+nn:string).type);

checkType(string, ("8"+r:string).type);   // no param c_string cast from real
checkType(string, (createStringWithNewBuffer(cstr)+r:string).type);  // no param c_string cast from real
checkType(string, (createStringWithNewBuffer(vcstr)+r:string).type);
checkType(string, ("8"+rr:string).type);
checkType(string, (createStringWithNewBuffer(cstr)+rr:string).type);
checkType(string, (createStringWithNewBuffer(vcstr)+rr:string).type);

checkType(string, ("8"+i:string).type);   // no param c_string cast from imag
checkType(string, (createStringWithNewBuffer(cstr)+i:string).type);  // no param c_string cast from imag
checkType(string, (createStringWithNewBuffer(vcstr)+i:string).type);
checkType(string, ("8"+ii:string).type);
checkType(string, (createStringWithNewBuffer(cstr)+ii:string).type);
checkType(string, (createStringWithNewBuffer(vcstr)+ii:string).type);

checkType(string, ("8"+c:string).type);   // no param complex
checkType(string, (createStringWithNewBuffer(cstr)+c:string).type);
checkType(string, (createStringWithNewBuffer(vcstr)+c:string).type);

checkType(string, (c"8"+e:string).type);
checkType(string, ("8"+e:string).type);
checkType(string, (cstr+e:string).type);
checkType(string, (createStringWithNewBuffer(vcstr)+e:string).type);
checkType(string, ("8"+ee:string).type);
checkType(string, (createStringWithNewBuffer(cstr)+ee:string).type);
checkType(string, (createStringWithNewBuffer(vcstr)+ee:string).type);

checkType(string, (c"8"+b:string).type);
checkType(string, ("8"+b:string).type);
checkType(string, (cstr+b:string).type);
checkType(string, (createStringWithNewBuffer(vcstr)+b:string).type);
checkType(string, ("8"+bb:string).type);
checkType(string, (createStringWithNewBuffer(cstr)+bb:string).type);
checkType(string, (createStringWithNewBuffer(vcstr)+bb:string).type);

