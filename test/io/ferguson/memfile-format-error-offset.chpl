use IO;

var f = openMemFile();

f.writer().write("1 2 3 error");

var r = f.reader();

r.readLiteral("1");
r.readLiteral("2");
r.readLiteral("3");
r.readLiteral("4");
