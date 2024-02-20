use IO;

var f = openMemFile();

f.writer(locking=false).write(new ioLiteral("asdf"));
f.reader(locking=false).read(new ioLiteral("asdf"));

f.writer(locking=false).readWriteLiteral("jkl;");
f.reader(locking=false).readWriteLiteral("jkl;");

f.writer(locking=false).write(new ioNewline());
f.reader(locking=false).read(new ioNewline());

f.writer(locking=false).readWriteNewline();
f.reader(locking=false).readWriteNewline();
