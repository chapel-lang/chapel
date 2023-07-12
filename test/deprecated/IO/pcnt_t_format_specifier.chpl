use IO, IO.FormattedIO;

record R {
  var x: int;
  proc writeThis(fw: fileWriter) throws do fw.write(x);
  proc readThis(fr: fileReader) throws do this.x = fr.read(int);
}

writeln("%t \t %t \t %t".format("a string", 1, new R(1)));

var f = open("pcnt_t.txt", ioMode.cwr);
f.writer().writef("%t \t %t \t %t", "a string", 2, new R(2));

var s: string,
    i: int,
    r: R;

f.reader().readf("%t \t %t \t %t", s, i, r);
writeln((s, i, r));
