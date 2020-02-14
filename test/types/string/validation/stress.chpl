/*
  we can always check this against the corresponding python3 script:

    with open('UTF8-test.txt', 'rb') as f:
        for l in f:
            b = bytes(l).decode(errors='replace')
            print(b, end='')
    print()
*/


use IO;

var f = open("UTF8-test.txt", mode=iomode.rw);
var chan = f.reader();

var b: bytes;
while chan.readbytes(b) {
  writeln(b.decode(errors=decodePolicy.replace));
}
chan.close();
f.close();
