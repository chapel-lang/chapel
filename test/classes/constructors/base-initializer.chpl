// Contributed by Rafael Larrosa Jimenez (rlarrosa@uma.es)

class Circle {
                var r: real;
                proc Circle(h)
                {
                        r=h;
                }
}

proc Circle.area() return 3.14159*r**2;





class Oval: Circle{
                var r2: real;
                proc Oval(h1,h2) {
                        r=h1;
                        r2=h2;
                }
}

override proc Oval.area() return 3.14159*r*r2;



var c = new owned Circle(4.0);

writeln("circle area:",c.area());

var o = new owned Oval(1.0,2.0);

writeln(o.area()," r:",o.r," r2:",o.r2);
