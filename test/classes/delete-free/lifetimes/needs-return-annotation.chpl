class C { var x: int; } 


var globalValue = new borrowed C(1);
proc getGlobalHashtableElement (key: C) {
  return globalValue;
}

proc bad1() {
  var bb: borrowed C?;
  {
    var own = new owned C(2);
    var b = getGlobalHashtableElement(own.borrow());
    bb = b;
  }
}
bad1();

var globalInt = 0;
proc getGlobalHashtableElementRef (const ref key: int) const ref {
  return globalInt;
}

proc bad2 () const ref {
  var key = 0;
  return getGlobalHashtableElementRef(key);
}

proc callBad2() {
  const ref x = bad2();
  writeln(x);
}
callBad2();

proc returnOneOfThem (a: borrowed C, b: borrowed C) {
  return b;
}

proc bad3() {
  var bb: borrowed C?;
  var outerOwn = new owned C(1);
  {
    var innerOwn = new owned C(2);
    var b = returnOneOfThem(innerOwn, outerOwn);
    bb = b;
  }
}
bad3();

proc getGlobalHashtableElementGeneric (key) {
  return globalValue;
}

proc bad4() {
  var bb: borrowed C?;
  {
    var own = new owned C(2);
    var b = getGlobalHashtableElementGeneric(own.borrow());
    bb = b;
  }
}
bad4();

proc returnOneOfThemGeneric (a, b) {
  return b;
}

proc bad5() {
  var bb: borrowed C?;
  var outerOwn = new owned C(1);
  {
    var innerOwn = new owned C(2);
    var b = returnOneOfThemGeneric(innerOwn.borrow(), outerOwn.borrow());
    bb = b;
  }
}
bad5();
