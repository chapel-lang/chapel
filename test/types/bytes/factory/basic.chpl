use CTypes;
const targetLocale = Locales[numLocales-1];

var myBytes = b"A Chapel bytes";
on targetLocale {

  var localMyBytes = b"A local Chapel bytes";
  writeln("Initialize from bytes");
  var sBorrowedFromRemote = bytes.createBorrowingBuffer(myBytes);
  var sBorrowedFromLocal = bytes.createBorrowingBuffer(localMyBytes);

  writeln(sBorrowedFromRemote);
  writeln(sBorrowedFromLocal);
}

writeln();

var cPtrTmp = allocate(uint(8), 4);
cPtrTmp[0] = 65:uint(8);
cPtrTmp[1] = 66:uint(8);
cPtrTmp[2] = 67:uint(8);
cPtrTmp[3] = 0:uint(8);
var cStr = cPtrTmp:c_ptrConst(c_char);
{
  // there should be 1 allocation 1 free
  writeln("Initialize from c_ptrConst(c_char)");
  var sNew = bytes.createCopyingBuffer(cStr);
  var sBorrowed = bytes.createBorrowingBuffer(cStr);
  var sOwned = bytes.createAdoptingBuffer(cStr);
  /*var sOwned = new string(cStr, isowned=true, needToCopy=false);*/

  writeln(sNew);
  writeln(sBorrowed);
  writeln(sOwned);
}

writeln();

var cPtr = allocate(uint(8), 4);
cPtr[0] = 65:uint(8);
cPtr[1] = 66:uint(8);
cPtr[2] = 67:uint(8);
cPtr[3] = 0:uint(8);
{
  // there should be 1 allocate, 2 frees
  writeln("Initialize from c_ptr(uint(8))");

  var sNew = bytes.createCopyingBuffer(cPtr, length=3, size=4);
  var sBorrowed = bytes.createBorrowingBuffer(cPtr, length=3, size=4);
  var sOwned = bytes.createAdoptingBuffer(cPtr, length=3, size=4);

  writeln(sNew);
  writeln(sBorrowed);
  writeln(sOwned);

  cPtr[1] = 32:uint(8);

  writeln(sNew);
  writeln(sBorrowed);
  writeln(sOwned);
}

writeln();

var cCharPtr = allocate(uint(8), 4);
cCharPtr[0] = 65:uint(8);
cCharPtr[1] = 66:uint(8);
cCharPtr[2] = 67:uint(8);
cCharPtr[3] = 0:uint(8);
{
  // there should be 1 allocate, 2 frees
  writeln("Initialize from c_ptr(c_char)");

  var sNew = bytes.createCopyingBuffer(cCharPtr, length=3, size=4);
  var sBorrowed = bytes.createBorrowingBuffer(cCharPtr, length=3, size=4);
  var sOwned = bytes.createAdoptingBuffer(cCharPtr, length=3, size=4);

  writeln(sNew);
  writeln(sBorrowed);
  writeln(sOwned);

  cCharPtr[1] = 32:uint(8);

  writeln(sNew);
  writeln(sBorrowed);
  writeln(sOwned);
}

writeln();

const cUCharConstPtr = allocate(uint(8), 4);
cUCharConstPtr[0] = 65:uint(8);
cUCharConstPtr[1] = 66:uint(8);
cUCharConstPtr[2] = 67:uint(8);
cUCharConstPtr[3] = 0:uint(8);
{
  // there should be 1 allocate, 2 frees
  writeln("Initialize from c_ptrConst(c_uchar)");

  var sNew = bytes.createCopyingBuffer(cUCharConstPtr, length=3, size=4);
  var sBorrowed = bytes.createBorrowingBuffer(cUCharConstPtr, length=3, size=4);
  var sOwned = bytes.createAdoptingBuffer(cUCharConstPtr, length=3, size=4);

  writeln(sNew);
  writeln(sBorrowed);
  writeln(sOwned);

  cUCharConstPtr[1] = 32:uint(8);

  writeln(sNew);
  writeln(sBorrowed);
  writeln(sOwned);
}

writeln();

const cCharConstPtr = allocate(uint(8), 4);
cCharConstPtr[0] = 65:uint(8);
cCharConstPtr[1] = 66:uint(8);
cCharConstPtr[2] = 67:uint(8);
cCharConstPtr[3] = 0:uint(8);
{
  // there should be 1 allocate, 2 frees
  writeln("Initialize from c_ptrConst(c_char)");

  var sNew = bytes.createCopyingBuffer(cCharConstPtr, length=3, size=4);
  var sBorrowed = bytes.createBorrowingBuffer(cCharConstPtr, length=3, size=4);
  var sOwned = bytes.createAdoptingBuffer(cCharConstPtr, length=3, size=4);

  writeln(sNew);
  writeln(sBorrowed);
  writeln(sOwned);

  cCharConstPtr[1] = 32:uint(8);

  writeln(sNew);
  writeln(sBorrowed);
  writeln(sOwned);
}

writeln();
