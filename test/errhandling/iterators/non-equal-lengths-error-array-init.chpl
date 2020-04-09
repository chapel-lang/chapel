config const useInternal = false;

proc willthrow() throws {
  throw new owned Error();
  return 777;
}

var RRR = 0..3;

proc test(expr) {
  var array1 = for i in {RRR} do willthrow();
}

test( for RRR do willthrow() );
