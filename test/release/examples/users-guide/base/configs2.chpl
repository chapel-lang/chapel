use configs;

config param verbose = false,
             bitMask = 1 << 8,
             size = numBits(int);

config type age = uint(8);

type myInt = int(size);

param sizeOfAge = numBits(age);

writeln(verbose);
writeln(bitMask);
writeln(size);
writeln(age:string);
writeln(myInt:string);
writeln(sizeOfAge);
