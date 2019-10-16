class Parent { }
class Child : Parent { }

var x:Parent = new owned Child();
writeln(x.type:string);
