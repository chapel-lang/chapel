config param op = 0;

param shift: int(64) = 128;
param unsignedShift: uint(64) = 128;
param shiftBy: int = 64;

select op {
  when 0 do param foo = shift << shiftBy;
  when 1 do param foo = shift >> shiftBy;
  when 2 do param foo = unsignedShift << shiftBy;
  when 3 do param foo = unsignedShift >> shiftBy;
}

