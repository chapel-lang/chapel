// Contributed by Rafael Larrosa Jimenez (rlarrosa@uma.es)

class Circle {
                param rank: int = 1;
                type idxType = int;
                const r : rank*idxType;
                proc Circle(h,param rank=h.size)
                {
                        this.r=h;
                }
}

proc Circle.area()
return 3.14159*2**2;

class Oval: Circle{
                var r2: real;
                proc Oval(h1,h2,param rank=h1.size) {
                        this.r=h1;
                        r2=h2;
                }
}
proc Oval.area()
        return 3.14159*r2;

var c = new Circle(h=(4,));
writeln("circle area:",c.area());

delete c;
var o = new Oval(h1=(1,),h2=2.0);
writeln(o.area()," r:",o.r," r2:",o.r2);

