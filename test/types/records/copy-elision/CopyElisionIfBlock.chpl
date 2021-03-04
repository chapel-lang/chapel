record r {
  proc init() { writeln('default init'); }
  proc init=(other: r) { writeln('init='); }
  proc deinit() { writeln('deinit'); }
}

proc =(ref lhs: r, rhs: r) { writeln('assign'); }

proc consumeElement(in elem) {}

// I would expect elision to occur here for 'x', but it does not.
proc test1() {
  writeln('T1');
  var doBlock = true;
  if doBlock {
    var x = new r();
    consumeElement(x);
  }
}
test1();

// Same test but in an on block.
proc test2() {
  writeln('T2');
  on Locales[0] {
    var doBlock = true;
    if doBlock {
      var x = new r();
      consumeElement(x);
    }
  }
}
test2();

// This test mimics what I am trying to do in 'set._addElem()'.
proc test3() {
  use Memory.Initialization;

  writeln('T3');
  pragma "no auto destroy"
  var x = new r();
  on Locales[0] {
    var doBlock = true;
    if doBlock {
      var moved = moveToValue(x);
      consumeElement(moved);
    }
  }
}
test3();

// Check copy elision for locals in conditional blocks.
proc test4() {
  writeln('T4');
  var doBlock = true;
  if doBlock {
    var x = new r();
    consumeElement(x);
  } else {
    var y = new r();
    consumeElement(y);
  }
}
test4();

// Same as above, but from other direction.
proc test5() {
  writeln('T5');
  var doBlock = false;
  if doBlock {
    var x = new r();
    consumeElement(x);
  } else {
    var y = new r();
    consumeElement(y);
  }
}
test5();

proc test6() {
  writeln('T6');

  proc innerTest6(doBlock: bool) {
    var branchName = if doBlock then "if" else "else";
    writeln(branchName);

    var x = new r();
    if doBlock {
      var a = x;
      consumeElement(a);
    } else {
      var b = x;
      return b;
    }
    return new r();
  }

  var x1 = innerTest6(doBlock=true);
  var x2 = innerTest6(doBlock=false);
}
test6();

proc test7() {
  use Memory.Initialization;

  writeln('T7');

  pragma "no auto destroy"
  var x = new r();

  on Locales[0] {
    var moved = moveToValue(x);
    var doBlock = true;
    if doBlock {
      consumeElement(moved);
    } else {
      var last = moved;
    }
  }
}
test7();

