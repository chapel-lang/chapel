class MyClass {
  var x:int;
  var y:int;
}

var x: borrowed = new owned MyClass(1,2);
writeln(x.type:string);
writeln(x);

var u = new unmanaged MyClass(3,4);
var y: borrowed = u;
writeln(y.type:string);
writeln(y);

delete u; 
