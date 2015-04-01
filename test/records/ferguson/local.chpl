use myrecord;

proc myfunction() {

  // Declare some local variables.
  var local1: R; // default initialized

  // This version does not work because the constructor
  // is returning by value (rather than being a method).
  //var local2: R = new R(x=20); // constructor initialized

  var local2: R;
  local2.init(x = 20);

  local1.verify();

  local2.verify();
  assert(local2.x == 20);

}

myfunction();

