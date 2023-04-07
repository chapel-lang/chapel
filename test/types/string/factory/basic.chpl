use CTypes;


const targetLocale = Locales[numLocales-1];

var chplStr = "A Chapel string";
on targetLocale {
  // there should be 2 allocations, 2 frees
  writeln("Initialize from string");
  var sBorrowed = string.createWithBorrowedBuffer(chplStr);

  writeln(sBorrowed);
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
    var sNew = string.createWithNewBuffer(cStr);
    var sBorrowed = string.createWithBorrowedBuffer(cStr);
    var sOwned = string.createWithOwnedBuffer(cStr);
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
    var sNew = string.createWithNewBuffer(cPtr, length=3, size=4);
    var sBorrowed = string.createWithBorrowedBuffer(cPtr, length=3, size=4);
    var sOwned = string.createWithOwnedBuffer(cPtr, length=3, size=4);

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
    var sNew = string.createWithNewBuffer(cCharPtr, length=3, size=4);
    var sBorrowed = string.createWithBorrowedBuffer(cCharPtr, length=3, size=4);
    var sOwned = string.createWithOwnedBuffer(cCharPtr, length=3, size=4);

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

