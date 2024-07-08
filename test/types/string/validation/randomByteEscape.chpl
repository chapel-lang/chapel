use CTypes;
use Random;

config const nBytes = 100000;
config const nIterations = 25;
config const useFactory = false;

var rs = new randomStream(eltType=uint(8));
for i in 1..nIterations {
  // create bytes with random bytes
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

  var randomEscapedString: string;
  try {
     randomEscapedString =
        if useFactory then
          string.createCopyingBuffer(buf, length=nBytes, size=nBytes+1,
                                    policy=decodePolicy.escape)
        else
          randomBytes.decode(policy=decodePolicy.escape);
  }
  catch e: DecodeError {
    halt("Unexpected decode error");
  }
  catch {
    halt("Unexpected error");
  }

  // unescaped string must be equal to the initial `bytes`
  if randomEscapedString.encode(policy=encodePolicy.unescape) != randomBytes {
    halt("Failed. Seed:", rs.seed, " Iteration:" , i);
  }
}
writeln("Success");
