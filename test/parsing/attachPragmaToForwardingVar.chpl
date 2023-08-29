record A {
  var x: int;

  proc ref assignX(){
    this.x = 1;
    writeln(this.x);
  }
}

record B {
  @chpldoc.nodoc
  forwarding var instance: A;
}

var B_test = new B();
B_test.assignX();
