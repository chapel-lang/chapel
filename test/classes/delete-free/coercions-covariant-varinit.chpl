use OwnedObject;

class MyClass {
  var x:int;
}

class SubClass : MyClass {
  var y:int;
}


proc test6() {
  var oChild = new Owned(new SubClass(1,2));
  var oParent: Owned(MyClass) = oChild;

  writeln("expect nil, oChild = ", oChild);
  writeln("oParent = ", oParent);
}

test6();
