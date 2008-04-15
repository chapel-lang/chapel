class A {

  var root : B;
  def A(){ root = new B(); }

  class B {
    var data : int;
    def B(){ data = 1; }
    // def initialize() { data = 1; } // was work around
  }

}

def main(){
  var a = new A();
  writeln(a);
}
