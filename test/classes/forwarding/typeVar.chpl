module Main{
  class A{
    type selfType;
    proc print(){
      writeln("hi, from A");
    }
  }
  class B{
    forwarding var a: A;
  }
  proc main(){
    var b = new B(new A(B));
    b.print();
    writeln("End");
  }
}
