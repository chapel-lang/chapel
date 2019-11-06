param b1 = b"bytes1";
param bAnother1 = b"bytes1";
param b2 = b"bytes2";

writeln(b1, " as ", b1.type:string);
var b1CStr = b1.c_str();
writeln(createStringWithNewBuffer(b1CStr), " as ", b1CStr.type:string);

param numBytes = b1.numBytes;
param size = bAnother1.size;
param length = b2.length;

writeln(numBytes);
writeln(size);
writeln(length);

param concatBytes = b1+bAnother1+b2;
writeln(concatBytes, " as ", concatBytes.type:string);

param shortBytes = b"A";
param myByte = shortBytes.toByte();
writeln(myByte);


writeln(b1.byte(3));

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

