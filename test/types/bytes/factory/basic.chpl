const targetLocale = Locales[numLocales-1];

var myBytes = b"A Chapel bytes";
on targetLocale {
  // there should be 2 allocations, 2 frees
  writeln("Initialize from bytes");
  var sNew = bytes.createWithNewBuffer(myBytes);
  var sBorrowed = bytes.createWithBorrowedBuffer(sNew);
  var sNewFromNew = bytes.createWithNewBuffer(sNew);

  writeln(sNew);
  writeln(sBorrowed);
  writeln(sNewFromNew);
}

writeln();

var cPtrTmp = c_malloc(uint(8), 4);
cPtrTmp[0] = 65:uint(8);
cPtrTmp[1] = 66:uint(8);
cPtrTmp[2] = 67:uint(8);
cPtrTmp[3] = 0:uint(8);
var cStr = cPtrTmp:c_string;
{
  // there should be 1 allocation 1 free
  writeln("Initialize from c_string");
  var sNew = bytes.createWithNewBuffer(cStr);
  var sBorrowed = bytes.createWithBorrowedBuffer(cStr);
  var sOwned = bytes.createWithOwnedBuffer(cStr);
  /*var sOwned = new string(cStr, isowned=true, needToCopy=false);*/

  writeln(sNew);
  writeln(sBorrowed);
  writeln(sOwned);
}

writeln();

var cPtr = c_malloc(uint(8), 4);
cPtr[0] = 65:uint(8);
cPtr[1] = 66:uint(8);
cPtr[2] = 67:uint(8);
cPtr[3] = 0:uint(8);
{
  // there should be 1 allocate, 2 frees
  writeln("Initialize from c_ptr");

  var sNew = bytes.createWithNewBuffer(cPtr, length=3, size=4);
  var sBorrowed = bytes.createWithBorrowedBuffer(cPtr, length=3, size=4);
  var sOwned = bytes.createWithOwnedBuffer(cPtr, length=3, size=4);

  writeln(sNew);
  writeln(sBorrowed);
  writeln(sOwned);

  cPtr[1] = 32:uint(8);

  writeln(sNew);
  writeln(sBorrowed);
  writeln(sOwned);
}

writeln();

