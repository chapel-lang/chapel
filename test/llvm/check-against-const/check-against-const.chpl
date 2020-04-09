
config const signedValue : int = 100;
config const unsignedValue : uint = 100;

proc test()
{
  // CHECK: icmp eq
  if signedValue == 1 then return 1;
  // CHECK: icmp sle
  if signedValue <= 2 then return 2;
  // CHECK: icmp slt
  if signedValue < 3 then return 3;
  // CHECK: icmp sge
  if signedValue >= 4 then return 4;
  // CHECK: icmp sgt
  if signedValue > 5 then return 5;
  // CHECK: icmp ne
  if signedValue != 6 then return 6;

  // CHECK: icmp ne
  if unsignedValue == 7 then return 7;
  // CHECK: icmp ule
  if unsignedValue <= 8 then return 8;
  // CHECK: icmp ult
  if unsignedValue < 9 then return 9;
  // CHECK: icmp uge
  if unsignedValue >= 10 then return 10;
  // CHECK: icmp ugt
  if unsignedValue > 11 then return 11;
  // CHECK: icmp ne
  if unsignedValue != 11 then return 11;

  return 0;
}

writeln(test());
