use Math;

class MyCircleImpl {
  var radius:real;
  proc area() {
    return pi*radius*radius;
  }
  proc circumference() {
    return 2.0*pi*radius;
  }
}
  
record MyCircle1 {
  var impl: MyCircleImpl;

  proc area() {
    return impl.area();
  }
  proc circumference() {
    return impl.circumference();
  }
}

record MyCircle2 {
  forwarding var impl: MyCircleImpl;
}

record MyCircle3 {
  var impl: MyCircleImpl;

  proc getImplOrFail() {
    if impl == nil then
      halt("impl is nil");
    else
      return impl;
  }

  forwarding getImplOrFail();
}

record MyCircle4 {
  var impl: MyCircleImpl;

  forwarding impl only area;
}

record MyCircle5 {
  var impl: MyCircleImpl;

  forwarding impl except circumference;
}


var r = sqrt(1.0/pi);

var x1 = new MyCircle1(new MyCircleImpl(r));
writeln(x1.area());

var x2 = new MyCircle2(new MyCircleImpl(r));
writeln(x2.area());

var x3 = new MyCircle3(new MyCircleImpl(r));
writeln(x3.area());

var x4 = new MyCircle4(new MyCircleImpl(r));
writeln(x4.area());

var x5 = new MyCircle5(new MyCircleImpl(r));
writeln(x5.area());

delete x1.impl;
delete x2.impl;
delete x3.impl;
delete x4.impl;
delete x5.impl;
