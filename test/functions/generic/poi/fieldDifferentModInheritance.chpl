// Tests calling a method on a field when the field's type and inherited type is
// not visible from the place where the method was called.
module One {
  class OuterClass {
    var field;

    proc one(x) {
      field.two(x);
      field.three(x);
    }
  }
}

module Two {
  private use TwoPoint1;

  class Stored : StoredParent {

    proc two(x) {
      writeln("In Stored.two with value ", x);
    }
  }
}

module TwoPoint1 {
  class StoredParent {
    proc three(x) {
      writeln("In StoredParent.three with value ", x);
    }
  }
}

module Three {
  proc blah() {
    use One;
    var fieldVal = foo();
    return new unmanaged OuterClass(fieldVal);
  }

  proc foo() {
    use Two;
    return new owned Stored();
  }
}

module User {
  use Three;

  proc main() {
    var x = blah();
    x.one(2);
    delete x;
  }
}
