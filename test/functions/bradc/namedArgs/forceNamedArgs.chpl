class dummyarg {
  proc init() {
    compilerError("can't create an object of type 'dummyarg' -- use named argument passing to call this routine");
  }
}

proc momentum(dummy: borrowed dummyarg? = nil, mass: real, velocity: real) {
  return mass*velocity;
}

var m1 = momentum(mass=1.2, velocity=2.3);
var m2 = momentum(velocity=2.3, mass=1.2);

var m3 = momentum2(1.2, 2.3);  // generates an unresolved call error
