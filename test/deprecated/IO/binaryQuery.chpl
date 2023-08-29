use IO;

var f = open("binaryQuery.chpl", ioMode.rw),
    w = f.writer(kind=iokind.native),
    r = f.reader(kind=iokind.native);

writeln(w.binary());
writeln(r.binary());
