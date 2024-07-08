use IO;

var f = openMemFile();

f.writer(locking=false).write("1 2 3 error");

var r = f.reader(locking=false);

r.readLiteral("1");
r.readLiteral("2");
r.readLiteral("3");
r.readLiteral("4");
