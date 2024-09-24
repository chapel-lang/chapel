// Note: the following tests have similar characteristics:
// - types/tuple/diten/tupleOfArray.chpl
// - types/tuple/diten/tupleOfArray2.chpl
// - types/tuple/sungeun/hetTupleWithArray.chpl
// - types/tuple/sungeun/homTupleOfArrays.chpl

type t = [1..3] int;
var a: (t, t);

writeln(a);
