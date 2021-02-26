// Tests calling a method on a field when the field's type pretends to be the
// parent type.
module One {
  class OuterClass {
    var field;

    proc one(x) {
      field.two(x);
    }
  }
}

module Two {
  private use TwoPoint1;

  class Stored : StoredParent {

    override proc two(x) {
      writeln("In Stored.two with value ", x);
    }
  }
}

module TwoPoint1 {
  class StoredParent {
    proc two(x) {
      writeln("In StoredParent.two with value ", x);
    }
  }
}

module Three {
  proc blah() {
    use One, TwoPoint1;
    var fieldVal = foo();
    var retVar: unmanaged OuterClass(owned StoredParent) =
      new unmanaged OuterClass(fieldVal: owned StoredParent);
    return retVar;
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
