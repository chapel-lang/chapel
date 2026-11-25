use IO;

var outfile = open("out.txt", ioMode.cw);
var myWriter = outfile.writer(locking = false);
for i in 0..9 {
    myWriter.write(i, " ");
}

myWriter.flush();
outfile.fsync();
on Locales[Locales.size - 1]{
    write(outfile.size);
}
