use IO;

iter file.split(hints = ioHintSet.empty) {
  open(this.path, ioMode.r, hints);
  yield 1;
}

var fl = open("open_inside_file_iter.good", ioMode.r);
for t in fl.split() do
 writeln(t);
