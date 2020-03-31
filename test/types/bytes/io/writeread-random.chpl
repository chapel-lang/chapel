use Random, IO;

config const nBytes = 1024;

// create bytes with random bytes
var randomStream = createRandomStream(eltType=uint(8));
var buf = c_malloc(uint(8), nBytes+1);
for i in 0..#nBytes {
  buf[i] = randomStream.getNext();
}
buf[nBytes] = 0;

const randomBytes = createBytesWithOwnedBuffer(buf, length=nBytes,
                                                    size=nBytes+1);

if randomBytes.size != nBytes {
  halt("Error creating bytes object with correct length");
}

var bytesChannel = opentmp();

{
  // write them to a channel
  var bytesWriter = bytesChannel.writer();
  bytesWriter.writef("%ht", randomBytes);
  bytesWriter.close();
}

{
  // read them into a different object
  var bytesReader = bytesChannel.reader();
  var readBytes = b"";
  bytesReader.readf("%ht", readBytes);
  bytesReader.close();
  // compare
  if readBytes == randomBytes {
    writeln("Success");
  }
  else {
    writeln("Failed: Bytes generated and read differ!");
  }
}

{
  // write them to a channel
  var bytesWriter = bytesChannel.writer();
  bytesWriter.writef("%|*s", randomBytes.size, randomBytes);
  bytesWriter.close();
}

{
  // read them into a different object
  var bytesReader = bytesChannel.reader();
  var readBytes = b"";
  var readLen = randomBytes.size;
  bytesReader.readf("%|*s", readLen, readBytes);
  bytesReader.close();
  // compare
  if readBytes == randomBytes {
    writeln("Success");
  }
  else {
    writeln("Failed: Bytes generated and read differ!");
  }
}
