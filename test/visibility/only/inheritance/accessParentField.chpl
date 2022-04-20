use BaseModule only BaseClass;

class ChildClass: BaseClass {
  var x = 10;
}

{ // always compiled
  var c: owned ChildClass? = new owned ChildClass();
  writeln(c!.baseField);
}

{ // always compiled
  var c: shared ChildClass? = new shared ChildClass();
  writeln(c!.baseField);
}

{ // didn't compile with `use BaseModule only BaseClass`
  var c: unmanaged ChildClass? = new unmanaged ChildClass();
  writeln(c!.baseField);  // could not resolve this
}

{ // always compiled
  var c: owned ChildClass = new owned ChildClass();
  writeln(c.baseField);
}

{ // always compiled
  var c: shared ChildClass = new shared ChildClass();
  writeln(c.baseField);
}

{ // didn't compile with `use BaseModule only BaseClass`
  var c: unmanaged ChildClass = new unmanaged ChildClass();
  writeln(c.baseField);   // could not resolve this
}
