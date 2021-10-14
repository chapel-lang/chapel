use BigInteger, Random;

var randNums: [0..#5] uint;
fillRandom(randNums);

var str: string;

for val in randNums do
  str += val:string;

var myBigInt = str: bigint;

assert(myBigInt.hash() == (str: bigint).hash());

var negativeBigInt = ("-" + str): bigint;

assert(myBigInt.hash() != negativeBigInt.hash());
