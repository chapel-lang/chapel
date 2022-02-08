use IO;

iter file.split(hints:iohints = IOHINT_NONE) {
  open(this.path, iomode.r, hints);
  yield 1;
}

var fl = open("open_inside_file_iter.good", iomode.r);
for t in fl.split() do
 writeln(t);
