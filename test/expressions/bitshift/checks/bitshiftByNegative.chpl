config const op = 0;

var shift: int(64) = 128;
var unsignedShift: uint(64) = 128;
var shiftBy = -8;

select op {
  when 0 do const foo = shift << shiftBy;
  when 1 do const foo = shift >> shiftBy; 
  when 2 do const foo = unsignedShift << shiftBy;
  when 3 do const foo = unsignedShift >> shiftBy;
}

