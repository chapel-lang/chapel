module mymod {
  class MyClass { var x:int; }

  proc main() throws {
    var nilable:unmanaged MyClass?;
    nilable = new unmanaged MyClass(1);
    writeln(nilable);

    var notNilable = nilable:unmanaged MyClass;
    writeln(notNilable);

    var notNilable2 = nilable:borrowed MyClass;
    writeln(notNilable2);

    var notNilable3 = (nilable:borrowed MyClass?):unmanaged MyClass;
    writeln(notNilable3);

    delete nilable;
    nilable = nil;
    notNilable = nilable:unmanaged MyClass;
    // Expecting an error thrown
    writeln("unexpected ", notNilable);
  }
}
