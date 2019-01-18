
// Contributed by Rafael Larrosa Jimenez (rlarrosa@uma.es)

class Circle {
  param rank: int = 1;
  type idxType = int;
  const r : rank*idxType;
  proc init(h,param myRank=h.size)
  {
    this.rank = myRank;
    this.r=h;
  }
}

proc Circle.area() return 3.14159*2**2;



class Oval: Circle{
  var r2: real;
  proc init(h1,h2,param myRank=h1.size) {
    super.init(h1, myRank);
    r2=h2;
  }
}

override proc Oval.area() return 3.14159*r2;



var c = new unmanaged Circle(h=(4,));

writeln("circle area:",c.area());

delete c;



var o = new unmanaged Oval(h1=(1,),h2=2.0);

writeln(o.area()," r:",o.r," r2:",o.r2);

delete o;
