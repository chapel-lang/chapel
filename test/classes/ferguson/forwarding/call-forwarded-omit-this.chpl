class MyCircleImpl {
  var radius:real;


  proc area() {
    return pi*radius*radius;
  }
  proc circumference() {
    return 2.0*pi*radius;
  }
}

record MyCircle {
  forwarding var impl: unmanaged MyCircleImpl;
  // above declaration requests forwarding

  // compiler creates area() method
  // to call impl.area()

  // compiler creates circumference() method
  // to call impl.circumference()

 
  proc good() {
    writeln("I'm a circle with area ", this.area());
  }
  proc bad() {
    writeln("I'm a circle with area ", area());
  }
  proc ugly() {
    good();
  }


}


var r = new MyCircle(new unmanaged MyCircleImpl(1.0));
r.good();
r.bad();
r.ugly();
