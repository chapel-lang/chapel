class A {

  var root : B;
  proc A(){ root = new B(); }

  class B {
    var data : int;
    proc B(){ data = 1; }
    // proc initialize() { data = 1; } // was work around
  }

}

proc main(){
  var a = new A();
  writeln(a);
}
