config const useInternal = false;

proc willthrow(): bool throws {
  throw new owned Error();
  return 777; // note that this line is ignored
}

var RRR = 0..3;

proc test(expr) {
  var array1 = for i in {RRR} do willthrow();
}

test( for RRR do willthrow() );
