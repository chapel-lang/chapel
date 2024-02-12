use IO;

var f = open("binaryQuery.chpl", ioMode.rw),
    w = f.writer(),
    r = f.reader();

var ws = w._styleInternal();
ws.binary = 1;
w._set_styleInternal(ws);

var rs = r._styleInternal();
rs.binary = 1;
r._set_styleInternal(rs);

writeln(w.binary());
writeln(r.binary());
