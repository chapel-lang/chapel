use IO, IO.FormattedIO;

record R {
  var x: int;
  proc writeThis(fw: fileWriter) throws do fw.write(x);
  proc readThis(fr: fileReader) throws do this.x = fr.read(int);
}

writeln("%jt \t %jt \t %jt".format("a string", 1, new R(1)));

var f = openTempFile();
f.writer().writef("%jt \t %jt \t %jt", "a string", 2, new R(2));

var s: string,
    i: int,
    r: R;

f.reader().readf("%jt \t %jt \t %jt", s, i, r);
writeln((s, i, r));
