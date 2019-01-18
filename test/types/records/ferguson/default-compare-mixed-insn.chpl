record R {
  type t = int;
  var x:t;
}

config param case = 1;

proc main() {
  var r64 = new R(int(64), 64);
  var r32 = new R(int(32), 32);
  
  if case == 1 then
    writeln(r32 == r64);
  
  if case == 2 then
    writeln(r64 == r32);

  if case == 3 then
    r64 = r32;

  if case == 4 then
    r32 = r64;
  
  if case == 5 then
    writeln(r32 != r64);
  
  if case == 6 then
    writeln(r64 != r32);
}
