record A {
  var x:int;
  proc writeThis(writer) throws {
    var loc = writer.readWriteThisFromLocale();
    writer.writeln("in A.writeThis loc=", loc.id);
    writer.writeln(x);
  }
}
record B {
  var a:A;
  proc writeThis(writer) throws {
    var loc = writer.readWriteThisFromLocale();
    writer.writeln("in B.writeThis loc=", loc.id);
    on loc {
      writer.writeln("in B.writeThis on loc");
      writer.writeln(a);
      // Note, since stdout is locked at this point,
      // and since the recursive locking isn't multi-locale
      // aware, the program can hang if we do I/O
      // with stdout at this point.
    }
  }
}

on Locales[numLocales-1] {
  var r = new B(new A(1));
  write(r);
}

