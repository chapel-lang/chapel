use CTypes;


const targetLocale = Locales[numLocales-1];

var chplStr = "A Chapel string";
on targetLocale {

  var localChplStr = "A local Chapel string";
  writeln("Initialize from string");
  var sBorrowedFromRemote = string.createBorrowingBuffer(chplStr);
  var sBorrowedFromLocal = string.createBorrowingBuffer(localChplStr);

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
  try! {
    var sNew = string.createCopyingBuffer(cStr);
    var sBorrowed = string.createBorrowingBuffer(cStr);
    var sOwned = string.createAdoptingBuffer(cStr);
    /*var sOwned = new string(cStr, isowned=true, needToCopy=false);*/

    writeln(sNew);
    writeln(sBorrowed);
    writeln(sOwned);
  }
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

  try! {
    var sNew = string.createCopyingBuffer(cPtr, length=3, size=4);
    var sBorrowed = string.createBorrowingBuffer(cPtr, length=3, size=4);
    var sOwned = string.createAdoptingBuffer(cPtr, length=3, size=4);

    writeln(sNew);
    writeln(sBorrowed);
    writeln(sOwned);

    cPtr[1] = 32:uint(8);

    writeln(sNew);
    writeln(sBorrowed);
    writeln(sOwned);
  }
}

writeln();

var cCharPtr = allocate(c_char, 4);
cCharPtr[0] = 65:uint(8);
cCharPtr[1] = 66:uint(8);
cCharPtr[2] = 67:uint(8);
cCharPtr[3] = 0:uint(8);
{
  // there should be 1 allocate, 2 frees
  writeln("Initialize from c_ptr(c_char)");

  try! {
    var sNew = string.createCopyingBuffer(cCharPtr, length=3, size=4);
    var sBorrowed = string.createBorrowingBuffer(cCharPtr, length=3, size=4);
    var sOwned = string.createAdoptingBuffer(cCharPtr, length=3, size=4);

    writeln(sNew);
    writeln(sBorrowed);
    writeln(sOwned);

    cCharPtr[1] = 32:uint(8);

    writeln(sNew);
    writeln(sBorrowed);
    writeln(sOwned);
  }
}

writeln();

const cPtrConst = allocate(c_uchar, 4);
cPtrConst[0] = 65:uint(8);
cPtrConst[1] = 66:uint(8);
cPtrConst[2] = 67:uint(8);
cPtrConst[3] = 0:uint(8);
{
  // there should be 1 allocate, 2 frees
  writeln("Initialize from c_ptrConst(c_uchar)");

  try! {
    var sNew = string.createCopyingBuffer(cPtrConst, length=3, size=4);
    var sBorrowed = string.createBorrowingBuffer(cPtrConst, length=3, size=4);
    var sOwned = string.createAdoptingBuffer(cPtrConst, length=3, size=4);

    writeln(sNew);
    writeln(sBorrowed);
    writeln(sOwned);

    cPtrConst[1] = 32:uint(8);

    writeln(sNew);
    writeln(sBorrowed);
    writeln(sOwned);
  }
}

writeln();

const cPtrConstChar = allocate(c_char, 4);
cPtrConstChar[0] = 65:uint(8);
cPtrConstChar[1] = 66:uint(8);
cPtrConstChar[2] = 67:uint(8);
cPtrConstChar[3] = 0:uint(8);
{
  // there should be 1 allocate, 2 frees
  writeln("Initialize from c_ptrConst(c_char)");

  try! {
    var sNew = string.createCopyingBuffer(cPtrConstChar, length=3, size=4);
    var sBorrowed = string.createBorrowingBuffer(cPtrConstChar, length=3, size=4);
    var sOwned = string.createAdoptingBuffer(cPtrConstChar, length=3, size=4);

    writeln(sNew);
    writeln(sBorrowed);
    writeln(sOwned);

    cPtrConstChar[1] = 32:uint(8);

    writeln(sNew);
    writeln(sBorrowed);
    writeln(sOwned);
  }
}

writeln();
