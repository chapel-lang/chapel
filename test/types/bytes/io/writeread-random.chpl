use CTypes;
use Random, IO;

config const nBytes = 1024;

// create bytes with random bytes
var rs = new randomStream(eltType=uint(8));
var buf = allocate(uint(8), (nBytes+1).safeCast(c_size_t));
for i in 0..#nBytes {
  buf[i] = rs.next();
}
buf[nBytes] = 0;

const randomBytes = bytes.createAdoptingBuffer(buf, length=nBytes,
                                                    size=nBytes+1);

if randomBytes.size != nBytes {
  halt("Error creating bytes object with correct length");
}

var bytesChannel = openTempFile();

{
  // write them to a channel
  var bytesWriter = bytesChannel.writer();
  bytesWriter.write(randomBytes);
  bytesWriter.close();
}

{
  // read them into a different object
  var bytesReader = bytesChannel.reader();
  var readBytes = bytesReader.readAll(bytes);
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
