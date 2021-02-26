use SysCTypes;
use CPtr;

const targetLocale = Locales[numLocales-1];

var chplStr = "A Chapel string";
on targetLocale {
  // there should be 2 allocations, 2 frees
  writeln("Initialize from string");
  var sNew = createStringWithNewBuffer(chplStr);
  var sBorrowed = createStringWithBorrowedBuffer(sNew);
  var sNewFromNew = createStringWithNewBuffer(sNew);

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
  try! {
    var sNew = createStringWithNewBuffer(cStr);
    var sBorrowed = createStringWithBorrowedBuffer(cStr);
    var sOwned = createStringWithOwnedBuffer(cStr);
    /*var sOwned = new string(cStr, isowned=true, needToCopy=false);*/

    writeln(sNew);
    writeln(sBorrowed);
    writeln(sOwned);
  }
}

writeln();

var cPtr = c_malloc(uint(8), 4);
cPtr[0] = 65:uint(8);
cPtr[1] = 66:uint(8);
cPtr[2] = 67:uint(8);
cPtr[3] = 0:uint(8);
{
  // there should be 1 allocate, 2 frees
  writeln("Initialize from c_ptr(uint(8))");

  try! {
    var sNew = createStringWithNewBuffer(cPtr, length=3, size=4);
    var sBorrowed = createStringWithBorrowedBuffer(cPtr, length=3, size=4);
    var sOwned = createStringWithOwnedBuffer(cPtr, length=3, size=4);

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

var cCharPtr = c_malloc(c_char, 4);
cCharPtr[0] = 65:uint(8);
cCharPtr[1] = 66:uint(8);
cCharPtr[2] = 67:uint(8);
cCharPtr[3] = 0:uint(8);
{
  // there should be 1 allocate, 2 frees
  writeln("Initialize from c_ptr(c_char)");

  try! {
    var sNew = createStringWithNewBuffer(cCharPtr, length=3, size=4);
    var sBorrowed = createStringWithBorrowedBuffer(cCharPtr, length=3, size=4);
    var sOwned = createStringWithOwnedBuffer(cCharPtr, length=3, size=4);

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

