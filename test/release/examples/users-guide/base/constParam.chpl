use IO;

config param testErrors = 0;

const two: int = 2;

if testErrors then
  two = 3;

const meaning = 42;  // inferred to be an 'int'
const zero: int;     // default initialized to '0'

writeln(two);
writeln(meaning);
writeln(zero);

const three = 3,
      four = 4,
      Zero, ZERO: int,         // 'Zero' and 'ZERO' are both 'int's default initialized to '0'
      language = "Chapel",     // 'language' is inferred to be a 'string'
      debug, verbose = false;  // 'debug' and 'verbose' are both 'bool's initialized to 'false'

const n = read(int),     // initialize 'n' by reading an 'int' from the console
      piOverN = pi / n;  // initialize 'piOverN' by dividing 'pi' by 'n'

writeln("n = ", n);
writeln("pi/n = ", piOverN);

{
  param zero, Zero, ZERO: int,
        two: int = 2,
        three = 3,
        four = 4,
        meaning = 42,
        language = "Chapel",
        debug, verbose = false;

  if testErrors {
    param n = read(int),
          piOverN = pi / n;
  }
}


param tenSquared = 10**2,
      bitMask = 1 << 8,
      bitsPerByte = 8,
      size = numBits(int),
      sizeInBytes = size / bitsPerByte,
      bigger = max(bitMask, size);

writeln(tenSquared);
writeln(bitMask);
writeln(size);
writeln(sizeInBytes);
writeln(bigger);
