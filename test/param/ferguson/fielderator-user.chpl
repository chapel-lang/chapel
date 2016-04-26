use Reflection;

record R {
  var x: int;
  var y: int;
}


assert(numFields(R) == 2);
assert(getFieldName(R, 1) == "x");

assert(getFieldIndex(R, "x") == 1);
assert(getFieldIndex(R, "y") == 2);
assert(getFieldIndex(R, "z") == 0); // no field z.
assert(hasField(R, "x") == true);
assert(hasField(R, "y") == true);
assert(hasField(R, "z") == false);


var r = new R(10,20);

assert(getField(r, 1) == 10);
assert(getField(r, 2) == 20);

assert(getField(r, "x") == 10);
assert(getField(r, "y") == 20);


getFieldRef(r, 1) = 100;
getFieldRef(r, "y") = 200;

assert(getFieldRef(r, "x") == 100);
assert(getFieldRef(r, "y") == 200);

