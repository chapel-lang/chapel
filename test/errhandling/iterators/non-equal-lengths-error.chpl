config const useInternal = false;

proc willthrow() throws {
  throw new Error();
  return 777;
}

var RRR = 0..3;

proc test(expr) {
  // Put both errors on the same line, for .bad convenience.
  // When 'useInternal", it calls chpl__initCopy(ir: _iteratorRecord),
  // which runs the same zippered loop as in the the !useInternal case here.
  if useInternal { var array1 = for i in {RRR} do willthrow(); } else { for zip(RRR, expr) {
      if numLocales < 0 then writeln("hi"); // no-op
    }
  }
}

test( for RRR do willthrow() );
