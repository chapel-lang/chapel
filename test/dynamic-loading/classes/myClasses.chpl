
class A {
  proc init() {
    writeln("A init");
  }
  proc deinit() {
    writeln("A deinit");
  }
  proc method1() {
    writeln("method1 called");
  }
  proc method2() {
    writeln("method2 called");
  }
}

class B: A {
  proc init() {
    writeln("B init");
  }
  proc deinit() {
    writeln("B deinit");
  }
  override proc method1() {
    writeln("method1 called (overridden in B)");
  }
  override proc method2() {
    writeln("method2 called (overridden in B)");
  }
  proc method3() {
    writeln("method3 called");
  }
  proc method4() {
    writeln("method4 called");
  }
}

class C: B {
  proc init() {
    writeln("C init");
  }
  proc deinit() {
    writeln("C deinit");
  }
  override proc method1() {
    writeln("method1 called (overridden in C)");
  }
  override proc method2() {
    writeln("method2 called (overridden in C)");
  }
  override proc method3() {
    writeln("method3 called (overridden in C)");
  }
  override proc method4() {
    writeln("method4 called (overridden in C)");
  }
  proc method5() {
    writeln("method5 called");
  }
}

proc callAllMethods(c: borrowed C, b: borrowed B, a: borrowed A) {
  writeln("callAllMethods");
  c.method1();
  c.method2();
  c.method3();
  c.method4();
  c.method5();
  var cb = c : borrowed B;
  cb.method1();
  cb.method2();
  cb.method3();
  cb.method4();
  var ca = c : borrowed A;
  ca.method1();
  ca.method2();

  b.method1();
  b.method2();
  b.method3();
  b.method4();
  var ba = b : borrowed A;
  ba.method1();
  ba.method2();

  a.method1();
  a.method2();
  writeln("="*80);
}

proc doDynamicCasts(baseClass: borrowed C?) {
  writeln("doDynamicCasts");
  if baseClass != nil {
    writeln("Successfully casted to C");
    var cObj = baseClass!;
    cObj.method1();
    cObj.method2();
    cObj.method3();
    cObj.method4();
    cObj.method5();
  }
  if (baseClass : unmanaged B?) != nil {
    writeln("Successfully casted to B");
    var bObj = (baseClass : unmanaged B?)!;
    bObj.method1();
    bObj.method2();
    bObj.method3();
    bObj.method4();
  }
  if (baseClass : unmanaged A?) != nil {
    writeln("Successfully casted to A");
    var aObj = (baseClass : unmanaged A?)!;
    aObj.method1();
    aObj.method2();
  }
  writeln("="*80);
}
