use IO, IO.FormattedIO;

record R {
  var x: int;

  proc writeThis(fw: fileWriter) throws {
    fw.write("<", x, ">");
  }

  proc readThis(fr: fileReader) throws {
    fr.readLiteral("<");
    this.x = fr.read(int);
    fr.readLiteral(">");
  }
}

var f = open("test.txt", ioMode.cwr),
    r = new R(1);

f.writer().writef("an R: %?\n", r);

var r2 = new R(2);
f.reader().readf("an R: %?\n", r2);

writeln(r == r2);
