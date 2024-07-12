module Main{
  class A{
    type selfType;
    proc print(){
      writeln("hi, from A");
    }
  }
  class B{
    forwarding var a: owned A(owned B);// Chenged here
  }
  proc main(){
    var b = new B(new A(owned B));
    b.print();
    writeln("End");
  }
}
