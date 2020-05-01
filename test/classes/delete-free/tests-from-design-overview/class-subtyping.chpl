class ParentClass {
  var x:int;
  proc parentMethod() {
    writeln("in parentMethod, this.type is ", this.type:string);
  }
  proc deinit() {
    writeln("in ParentClass.deinit");
  }
}
class ChildClass: ParentClass {
  proc method() {
    writeln("in method, this.type is ", this.type:string);
  }
}

proc consumeParent(in arg: owned ParentClass) {
  writeln("in consumeParent");
}

proc test1() {
  writeln("in test1");
  var x = new owned ChildClass();

  consumeParent(x); // coerces ‘owned ChildClass’ to ‘owned ParentClass’
		    // and consumes x, leaving it 'nil'
  writeln("back in test1");
}
test1();

proc borrowParent(arg: borrowed ParentClass) {
  writeln("in borrowParent");
}

proc test2() {
  writeln("in test2");
  var y = new owned ChildClass();
  borrowParent(y); // coerces ‘owned ChildClass’ to ‘borrowed ParentClass’
		   // y still stores an object
  writeln("back in test2 ", y);
}
test2();

proc test3() {
  writeln("in test3");
  var z = new owned ChildClass();
  z.method();
  z.parentMethod();
  writeln("back in test3");
}
test3();
