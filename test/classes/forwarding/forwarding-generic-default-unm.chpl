// This test case represents a bug originally reported in issue #7783

class A{
  forwarding var driver: unmanaged B;
}

class B{
  type MyType = 3*string; //This makes the compiler not find the overridden methods.

  var bfield:MyType; 

  proc foo1(){
     return this;
  }

  proc foo2(args2:string){
      return this;
  }

  proc foo3(args3, args4:string){
      return this;
  }
  proc foo4(arg){
     return this;
  }

}

class C:B{

  var cfield:int;
}


var c1 = new unmanaged A(new unmanaged C());
writeln(c1.foo1());

var c2 = new unmanaged A(new unmanaged C());
writeln(c2.foo2("Test"));

var c3 = new unmanaged A(new unmanaged C());
writeln(c3.foo3("Test", "Test"));

var c4 = new unmanaged A(new unmanaged C());
writeln(c4.foo4("Test"));

delete c4.driver;
delete c4;
delete c3.driver;
delete c3;
delete c2.driver;
delete c2;
delete c1.driver;
delete c1;
