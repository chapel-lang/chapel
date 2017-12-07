// Modified from
// test/classes/constructors/dflt-ctor-generic-uninit-field-const-err-1.chpl
pragma "use default init"
class A {
  const c;
}

var x = new A();
