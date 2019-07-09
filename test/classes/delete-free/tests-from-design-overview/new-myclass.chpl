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
  var b: borrowed MyClass = new MyClass(2); 
  var c: borrowed MyClass = new owned MyClass(3); 
  var d = (new owned MyClass(4)): borrowed MyClass; 
  var e = (new owned MyClass(5)): borrowed MyClass; 
  var f = (new owned MyClass(6)).borrow();
  writeln(a.type:string);
  writeln(b.type:string);
  writeln(c.type:string);
  writeln(d.type:string);
  writeln(e.type:string);
  writeln(f.type:string);
}
test1();
