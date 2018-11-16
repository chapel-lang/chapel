class C { var x: int; } 


var globalValue = new borrowed C(1);
proc getGlobalHashtableElement (key: C) lifetime return globalValue {
  return globalValue;
}

proc ok1() {
  var bb: borrowed C;
  {
    var own = new owned C(2);
    var b = getGlobalHashtableElement(own.borrow());
    bb = b;
  }
  writeln(bb.x);
}
ok1();

var globalInt = 0;
proc getGlobalHashtableElementRef (const ref key: int) const ref lifetime return globalInt {
  return globalInt;
}

proc ok2 () const ref {
  var key = 0;
  return getGlobalHashtableElementRef(key);
}

proc callOk2() {
  const ref x = ok2();
  writeln(x);
}
callOk2();
