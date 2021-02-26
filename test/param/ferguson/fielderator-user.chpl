use Reflection;

record R {
  var x: int;
  var y: int;
}


assert(numFields(R) == 2);
assert(getFieldName(R, 0) == "x");

assert(getFieldIndex(R, "x") == 0);
assert(getFieldIndex(R, "y") == 1);
assert(getFieldIndex(R, "z") == -1); // no field z.
assert(hasField(R, "x") == true);
assert(hasField(R, "y") == true);
assert(hasField(R, "z") == false);


var r = new R(10,20);

assert(getField(r, 0) == 10);
assert(getField(r, 1) == 20);

assert(getField(r, "x") == 10);
assert(getField(r, "y") == 20);


getFieldRef(r, 0) = 100;
getFieldRef(r, "y") = 200;

assert(getFieldRef(r, "x") == 100);
assert(getFieldRef(r, "y") == 200);

