config var x: uint(32) = 15;

param one = 1;

var y = x ^ one;

writeln("y is: ", y);

writeln("the size of y is: ", numBits(y.type));
writeln("it ranges from: ", min(y.type), " to ", max(y.type));

writeln("the size of one is: ", numBits(one.type));
writeln("it ranges from: ", min(one.type), " to ", max(one.type));


const onePrime = 1;

var yPrime = x ^ onePrime;

writeln("yPrime is: ", yPrime);

writeln("the size of yPrime is: ", numBits(yPrime.type));
writeln("it ranges from: ", min(yPrime.type), " to ", max(yPrime.type));

writeln("the size of onePrime is: ", numBits(onePrime.type));
writeln("it ranges from: ", min(onePrime.type), " to ", max(onePrime.type));
