// Modified from
// test/classes/constructors/dflt-ctor-generic-uninit-field-type-err-1.chpl
class A {
  type t;
}

var x = new A();
