class C: Writer {
  var data: string;
  def writeIt(s: string) {
    data += s.substring(1);
  }
  def writeThis(x: Writer) {
    x.write(data);
  }
}

var c = new C();

c.write(41, 32, 23, 14);

writeln(c);
