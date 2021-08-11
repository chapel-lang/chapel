use IO;

var f = openmem();

f.writer().write("1 2 3 error");

var r = f.reader();

r.read(new ioLiteral("1"));
r.read(new ioLiteral("2"));
r.read(new ioLiteral("3"));
r.read(new ioLiteral("4"));
