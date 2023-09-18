record A : writeSerializable {
  var x:int;
  proc serialize(writer, ref serializer) throws {
    var loc = writer.readWriteThisFromLocale();
    writer.writeln("in A.serialize loc=", loc.id);
    writer.writeln(x);
  }
}
record B : writeSerializable {
  var a:A;
  proc serialize(writer, ref serializer) throws {
    var loc = writer.readWriteThisFromLocale();
    writer.writeln("in B.serialize loc=", loc.id);
    on loc {
      writer.writeln("in B.serialize on loc");
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

