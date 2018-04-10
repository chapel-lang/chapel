class A {
  forwarding var driver: B;
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
  proc print(){
    writeln("Hallo from C");
  }

  proc printv(x...?n){
    writeln("Hallo from varidic method in C");
  }
}

proc main() {
  var obj = new A(new C() );
  obj.print();
  obj.printv(1);
}
