use IO;

class A : writeSerializable {
  var x:int;
  override proc serialize(writer, ref serializer) throws {
    var loc = writer.readWriteThisFromLocale();
    writeln("in A.serialize loc= ", loc.id);
    writer.writeln(x);
  }
}

class B : readDeserializable, initDeserializable {
  var x:int;
  proc init(x: int = 0) { this.x = x; }
  proc init(reader: fileReader(?), ref deserializer) {
    var loc = reader.readWriteThisFromLocale();
    writeln("in B.init loc= ", loc.id);
    this.x = reader.readln(int);
  }
  override proc deserialize(reader, ref deserializer) throws {
    var loc = reader.readWriteThisFromLocale();
    writeln("in B.deserialize loc= ", loc.id);
    reader.readln(x);
  }
}

class C : serializable {
  var x:int;
  proc init(x: int = 0) { this.x = x; }
  proc init(reader: fileReader(?), ref deserializer) {
    var loc = reader.readWriteThisFromLocale();
    writeln("in C.init loc= ", loc.id);
    this.x = reader.read(int);
  }

  override proc deserialize(reader, ref deserializer) throws {
    var loc = reader.readWriteThisFromLocale();
    writeln("in C.deserialize loc= ", loc.id);
    reader.read(x);
  }

  override proc serialize(writer, ref serializer) throws {
    var loc = writer.readWriteThisFromLocale();
    writeln("in C.serialize loc= ", loc.id);
    writer.write(x);
  }
}


var a = new A(1);
var b = new B(1);
var c = new C(1);

var f = openMemFile();
var w = f.writer(locking=false);

writeln("Writes from Locale 1");
on Locales[1] {
  w.writeln(a);
  // w.writeln(b); B is not writeable
  w.writeln(c);
}

writeln("Writes from Locale 2");
on Locales[2] {
  w.writeln(a);
  // w.writeln(b); B is not writeable
  w.writeln(c);
}


var r = f.reader(locking=false);

writeln("Reads from Locale 1");
on Locales[1] {
  //r.readln(a); A is not readable
  r.readln(b);
  r.readln(c);
}

writeln("Reads from Locale 2");
on Locales[2] {
  //r.readln(a); A is not readable
  r.readln(b);
  r.readln(c);
}
