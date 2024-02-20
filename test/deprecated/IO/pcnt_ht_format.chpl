use IO, IO.FormattedIO;

record R {
  var x: int;
  proc writeThis(fw: fileWriter(?)) throws do fw.write(x);
  proc ref readThis(fr: fileReader(?)) throws do this.x = fr.read(int);
}

writeln("%ht \t %ht \t %ht".format("a string", 1, new R(1)));

var f = openTempFile();
f.writer(locking=false).writef("%ht \t %ht \t %ht", "a string", 2, new R(2));

var s: string,
    i: int,
    r: R;

f.reader(locking=false).readf("%ht \t %ht \t %ht", s, i, r);
writeln((s, i, r));
