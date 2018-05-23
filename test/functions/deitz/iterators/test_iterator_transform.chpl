//
// This test illustrates the iterator transformation; the iterator foo
// is transformed into function goo and class bar.
//

//
// BEFORE
//
iter foo(n: int) {
  var i = 0;
  while i < n {
    yield i;
    i = i + 1;
  }
}

for i in foo(10) do
  writeln(i);

//
// AFTER
//
proc goo(n: int)
  return new unmanaged bar(n);

class bar {
  var n, i, result: int;
/*
  proc getNextCursor(c: int) {
    var n = this.n;
    var i = this.i;
    if c == 2 then
      goto L2;
    i = 0;
    this.i = 0;
    while i < n {
      this.result = i;
      return 2;
label L2
      i = i + 1;
      this.i = i;
    }
    return 0;
  }
*/
  proc getNextCursor(c: int) {
    var n = this.n;
    var i = this.i;
    if c != 2 {
      i = 0;
      this.i = 0;
      if i < n {
        this.result = i;
        return 2;
      }
    } else {
      i = i + 1;
      this.i = i;
      if i < n {
        this.result = i;
        return 2;
      }
    }
    return 0;
  }


  proc getHeadCursor() return getNextCursor(1);
  proc isValidCursor(c: int) return c != 0;
  proc getValue(c: int) return result;
}

var ic = goo(10);
var c  = ic.getHeadCursor();

while (ic.isValidCursor(c)) {
  writeln(ic.getValue(c));

  c = ic.getNextCursor(c);
}

delete ic;

