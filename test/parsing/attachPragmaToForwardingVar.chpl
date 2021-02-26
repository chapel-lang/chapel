record A {
  var x: int;

  proc assignX(){
    this.x = 1;
    writeln(this.x);
  }
}

record B {
  pragma "no doc"
  forwarding var instance: A;
}

var B_test = new B();
B_test.assignX();
