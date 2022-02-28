use IO;
use Time;
var f=open("/home/sayan/chapel-1.25.0/examples/f2.txt",
            iomode.r);
var filereader=f.reader(kind=iokind.native,locking=false,
                        hints=IOHINT_SEQUENTIAL|IOHINT_CACHED);
var x:bytes;

filereader.readbytes(x);

write(x);

filereader.close();
f.close();
