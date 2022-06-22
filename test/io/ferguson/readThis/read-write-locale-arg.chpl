use IO;

class A {
  var x:int;
  proc writeThis(writer) throws {
    var loc = writer.readWriteThisFromLocale();
    writeln("in A.writeThis loc= ", loc.id);
    writer.writeln(x);
  }
}

class B {
  var x:int;
  proc readThis(writer) throws {
    var loc = writer.readWriteThisFromLocale();
    writeln("in B.readThis loc= ", loc.id);
    writer.readln(x);
  }
}

class C {
  var x:int;

  proc readThis(r) throws {
    readWriteHelper(r);
  }

  proc writeThis(w) throws {
    readWriteHelper(w);
  }

  proc readWriteHelper(rw) throws {
    var loc = rw.readWriteThisFromLocale();
    writeln("in C.readWriteHelper loc= ", loc.id);
    if rw.writing then
      rw.write(x);
    else
      rw.read(x);
  }
}


var a = new unmanaged A(1);
var b = new unmanaged B(1);
var c = new unmanaged C(1);

var f = openmem();
var w = f.writer();

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


var r = f.reader();

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


delete a;
delete b;
delete c;
