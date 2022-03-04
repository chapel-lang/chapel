use IO;
use Time;
var f=open("moby.txt",iomode.r);
var filereader=f.reader(kind=iokind.native,locking=false,
                        hints=IOHINT_SEQUENTIAL|IOHINT_CACHED);
var x:bytes;

filereader.readbytes(x);

write(x);

filereader.close();
f.close();
