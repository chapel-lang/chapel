class MyClass {
  var x:int;
  proc init(arg:int) {
    this.x = arg;
    writeln("in MyClass.init ", x);
  }
  proc deinit() {
    writeln("in MyClass.deinit ", x);
  }
}

proc test1() {
  var a = new MyClass(1);
  var myClass2 = new MyClass(2);
  var b: borrowed MyClass = myClass2;
  var myClass3 = new MyClass(3);
  var c: borrowed MyClass = myClass3;
  var dOwn = new owned MyClass(4);
  var d = dOwn: borrowed MyClass; 
  var eOwn = new owned MyClass(5);
  var e = eOwn: borrowed MyClass; 
  var fOwn = new owned MyClass(6);
  var f = fOwn.borrow();
  writeln(a.type:string);
  writeln(b.type:string);
  writeln(c.type:string);
  writeln(d.type:string);
  writeln(e.type:string);
  writeln(f.type:string);
}
test1();
