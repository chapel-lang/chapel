use IO;

var f = openMemFile();

f.writer().write(new ioLiteral("asdf"));
f.reader().read(new ioLiteral("asdf"));

f.writer().readWriteLiteral("jkl;");
f.reader().readWriteLiteral("jkl;");

f.writer().write(new ioNewline());
f.reader().read(new ioNewline());

f.writer().readWriteNewline();
f.reader().readWriteNewline();
