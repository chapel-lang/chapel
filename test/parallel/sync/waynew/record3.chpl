// We used to think sync should be on the record, not the fields.
// Now we think sync should only be applied to single-value types.

record R {
  var x: int;
  var y: int;
  var z: int;
}

var s: sync R;

s.x = 1;
s.x = 2;
s.y = 3;
s.y = 4;
s.z = 5;
s.z = 6;
writeln( (s.x, s.y, s.z));


