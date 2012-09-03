
class A {
  proc noargs()         { writeln("A.noargs()");     }
  proc generic(argA)    { writeln("A.generic()");    }
  proc nongeneric(argA) { writeln("A.nongeneric()"); }
}
class B: A {
  proc noargs()         { writeln("B.noargs()");     }
  proc generic(argB)    { writeln("B.generic()");    }
  proc nongeneric(argB) { writeln("B.nongeneric()"); }
}

config const sel = false;
var c: A = if sel then new A() else new B();
c.noargs();
c.generic(1);
c.nongeneric(2);
