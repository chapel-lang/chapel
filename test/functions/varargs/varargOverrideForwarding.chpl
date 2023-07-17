class A {
  forwarding var driver: borrowed B;
}

class B {
  proc print() {
    writeln("Hallo from B");
  }

  proc printv(x...?n){
    writeln("Hallo from varidic method in B");
  }
}

class C: B {
  override proc print(){
    writeln("Hallo from C");
  }

  override proc printv(x...?n){
    writeln("Hallo from varidic method in C");
  }
}

proc main() {
  var ownInner = new owned C();
  var ownObj = new owned A(ownInner.borrow());
  var obj = ownInner.borrow();
  obj.print();
  obj.printv(1);
}
