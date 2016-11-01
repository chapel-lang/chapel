// Modified from
// test/classes/constructors/base-initializer.chpl

// Contributed by Rafael Larrosa Jimenez (rlarrosa@uma.es)

class Circle {
  var r: real;
  proc init(h)
  {
    r=h;
  }
}

proc Circle.area() return 3.14159*r**2;





class Oval: Circle{
  var r2: real;
  proc init(h1,h2) {
    r2=h2;
    super.init(h1);
  }
}

proc Oval.area() return 3.14159*r*r2;



var c = new Circle(4.0);

writeln("circle area:",c.area());

delete c;



var o = new Oval(1.0,2.0);

writeln(o.area()," r:",o.r," r2:",o.r2);

delete o;


