use Types;

use Copyable;

record GenericRecord {
  type t;
  var x : t;

  type e;
  var y : e;
}

record GenericRecordWithSomeDefaults {
  type t = int;
  var x : t;

  type e;
  var y : e;
}

record GenericRecordWithDefaults {
  type t = int;
  var x : t;

  type e = real;
  var y : e;
}

proc main() {
  var grSomeDefaultsFilledIn = new GenericRecordWithSomeDefaults(int, real)(5, 4.2);
  var grAllDefaults = new GenericRecordWithDefaults()(5, 4.2);
  var grAllDefaultsFilledIn = new GenericRecordWithDefaults(int, int)(5, 4);

  // generic non-composite ok
  checkNormal(integral, nil, onlyType = true);

  // generic composite not ok
  // using nil value and not checking it, as these can't be instantiated
  checkNo(GenericRecord(?), nil, onlyType = true);
  checkNo(GenericRecordWithSomeDefaults(?), nil, onlyType = true);

  // generic with all defaults ok
  checkNormal(GenericRecordWithSomeDefaults(int, real), grSomeDefaultsFilledIn);
  checkNormal(GenericRecordWithDefaults, grAllDefaults);
  checkNormal(GenericRecordWithDefaults(int, int), grAllDefaultsFilledIn);
}
