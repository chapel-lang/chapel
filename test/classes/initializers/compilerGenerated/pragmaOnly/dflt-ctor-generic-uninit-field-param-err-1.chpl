// Modified from
// test/classes/constructors/dflt-ctor-generic-uninit-field-param-err-1.chpl
pragma "use default init"
class A {
  param p;
}

var x = new A();
