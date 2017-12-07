// Modified from
// test/classes/constructors/dflt-ctor-generic-uninit-field-var-err-1.chpl
pragma "use default init"
class A {
  var v;
}

var x = new A();
