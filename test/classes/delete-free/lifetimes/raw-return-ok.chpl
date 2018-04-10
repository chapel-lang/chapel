pragma "safe"
module rawOK {

  use OwnedObject;

  class MyClass {
    var x:int;
  }

  record R {
    pragma "owned"
    var tmp:MyClass;

    proc makeClass() {
      var c = new unmanaged MyClass();
      return c;
    }
  }

  proc callMakeClassOnLocal() {
    var r:R;
    var c = r.makeClass();
    return c;
  }

  proc test() {
    var tmp = callMakeClassOnLocal();
    writeln(tmp);
    delete tmp;
  }

  test();

}
