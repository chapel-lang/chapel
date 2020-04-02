param b1 = b"bytes1";
param bAnother1 = b"bytes1";
param b2 = b"bytes2";

writeln(b1, " as ", b1.type:string);
param b1CStr = b1.c_str();
writeln(createStringWithNewBuffer(b1CStr), " as ", b1CStr.type:string);

param numBytes = b1.numBytes;
param size = bAnother1.size;
param length = b2.size;

writeln(numBytes);
writeln(size);
writeln(length);

param concatBytes = b1+bAnother1+b2;
writeln(concatBytes, " as ", concatBytes.type:string);

param shortBytes = b"A";
param myByte = shortBytes.toByte();
writeln(myByte);


writeln(b1.byte(2));

{
  param flag = b1==b2;
  writeln(flag);  // false
}

{
  param flag = b1!=b2;
  writeln(flag);  // true
}

{
  param flag = b1!=bAnother1;
  writeln(flag);  // false
}

{
  param flag = b1==bAnother1;
  writeln(flag);  // true
}

{
  param flag = b1>=b2;
  writeln(flag);  // false
}

{
  param flag = b1<=b2;
  writeln(flag);  // true
}

{
  param flag = b1>b2;
  writeln(flag);  // false
}

{
  param flag = b1<b2;
  writeln(flag);  // true
}

{
  param flag = b1>bAnother1;
  writeln(flag);  // false
}

{
  param flag = b1>=bAnother1;
  writeln(flag);  // true
}

{
  param flag = b1<bAnother1;
  writeln(flag);  // false
}

{
  param flag = b1<=bAnother1;
  writeln(flag);  // true
}

// check casts
param s1 = "some string";
param c1 = c"some C string";
param i = 10;
param r = 10.0;
param b = true;
param im = 10i;
param com = 10+10i;

param bFromS = s1:bytes;
param bFromC = c1:bytes;
param bFromI = i:bytes;
param bFromR = r:bytes;
param bFromB = b:bytes;
param bFromIm = im:bytes;
param bFromCom = com:bytes;

writeln(bFromI , " as ", bFromC.type:string);
writeln(bFromR , " as ", bFromR.type:string);
writeln(bFromB , " as ", bFromB.type:string);
writeln(bFromIm , " as ", bFromIm.type:string);
writeln(bFromCom , " as ", bFromCom.type:string);
writeln(bFromS , " as ", bFromS.type:string);
writeln(bFromC , " as ", bFromC.type:string);
