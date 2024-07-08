module my_freader {
use IO;
    class P {
            var size: int = 0;
            var buffer : [] bytes;

            proc init() {
                    var f = open("freader.chpl",ioMode.r);
                    var ch = f.reader();

                    size = f.size;
                    buffer = new bytes[size];

                    ch.read( buffer, size );

                    writeln( buffer:string, size );
            }

    }

    proc main() {
            var p: P;
    }
}
