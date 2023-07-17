use decls;
use checkType;

// Note: c strings use param operations for string and then
// the result is coerceible back to c string. This avoids some
// ambiguities.
checkType(string, (c"8"+n:string).type);
checkType(string, ("8"+n:string).type);
checkType(string, (cstr+n:string).type);
checkType(string, (string.createCopyingBuffer(vcstr)+n:string).type);
checkType(string, ("8"+nn:string).type);
checkType(string, (string.createCopyingBuffer(cstr)+nn:string).type);
checkType(string, (string.createCopyingBuffer(vcstr)+nn:string).type);

checkType(string, ("8"+r:string).type);   // no param c_string cast from real
checkType(string, (string.createCopyingBuffer(cstr)+r:string).type);  // no param c_string cast from real
checkType(string, (string.createCopyingBuffer(vcstr)+r:string).type);
checkType(string, ("8"+rr:string).type);
checkType(string, (string.createCopyingBuffer(cstr)+rr:string).type);
checkType(string, (string.createCopyingBuffer(vcstr)+rr:string).type);

checkType(string, ("8"+i:string).type);   // no param c_string cast from imag
checkType(string, (string.createCopyingBuffer(cstr)+i:string).type);  // no param c_string cast from imag
checkType(string, (string.createCopyingBuffer(vcstr)+i:string).type);
checkType(string, ("8"+ii:string).type);
checkType(string, (string.createCopyingBuffer(cstr)+ii:string).type);
checkType(string, (string.createCopyingBuffer(vcstr)+ii:string).type);

checkType(string, ("8"+c:string).type);   // no param complex
checkType(string, (string.createCopyingBuffer(cstr)+c:string).type);
checkType(string, (string.createCopyingBuffer(vcstr)+c:string).type);

checkType(string, (c"8"+pe:string).type);
checkType(string, ("8"+pe:string).type);
checkType(string, (cstr+pe:string).type);
checkType(string, (string.createCopyingBuffer(vcstr)+pe:string).type);
checkType(string, ("8"+ee:string).type);
checkType(string, (string.createCopyingBuffer(cstr)+ee:string).type);
checkType(string, (string.createCopyingBuffer(vcstr)+ee:string).type);

checkType(string, (c"8"+b:string).type);
checkType(string, ("8"+b:string).type);
checkType(string, (cstr+b:string).type);
checkType(string, (string.createCopyingBuffer(vcstr)+b:string).type);
checkType(string, ("8"+bb:string).type);
checkType(string, (string.createCopyingBuffer(cstr)+bb:string).type);
checkType(string, (string.createCopyingBuffer(vcstr)+bb:string).type);

