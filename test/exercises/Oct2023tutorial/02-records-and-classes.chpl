use Math;

{
  record circle {
    var radius: real;
    proc area() {
      return pi*radius**2;
    }
  }
  var c1: circle;      // default-initialized
  c1 = new circle(radius=1.0);
  var c2 = c1;         // copies c1
  c1.radius = 5.0;
  writeln(c2.radius);  // prints 1.0
}


{
  class Circle {
    var radius: real;
    proc area() {
      return pi*radius**2;
    }
  }
  // c1 is a nilable class
  var c1: Circle? = new shared Circle(radius=1.0);
  var c2 = c1;         // aliases c1’s circle
  c1!.radius = 5.0;
  writeln(c2!.radius);  // prints 5.0
}
