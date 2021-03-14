use BigInteger;

proc castAndPrintType(a: bigint, type t) {
  writeln((a:t).type:string);
}

proc castAndPrintValue(a: bigint, type t) {
  writeln(a:t);
}

var a = new bigint(1);
var negA = new bigint(-1);

castAndPrintType(a, uint(8));
castAndPrintType(a, uint(16));
castAndPrintType(a, uint(32));
castAndPrintType(a, uint(64));
castAndPrintType(a, uint);

castAndPrintType(a, int(8));
castAndPrintType(a, int(16));
castAndPrintType(a, int(32));
castAndPrintType(a, int(64));
castAndPrintType(a, int);

castAndPrintType(a, real(32));
castAndPrintType(a, real(64));
castAndPrintType(a, real);


castAndPrintValue(a,    uint);  // 1
castAndPrintValue(negA, uint);  // 1 (cast to uint is defined as abs value)

castAndPrintValue(a,    int);   // 1
castAndPrintValue(negA, int);   // -1

castAndPrintValue(a,    real);  // 1.0
castAndPrintValue(negA, real);  // -1.0

