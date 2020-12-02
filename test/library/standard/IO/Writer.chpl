class C: Writer {
  var data: string;
  proc writeIt(s: string) {
    data += s[1];
  }
  proc writeThis(x: Writer) throws {
    x.write(data);
  }
}

var c = new C();
c.write(41, 32, 23, 14);
writeln(c);
