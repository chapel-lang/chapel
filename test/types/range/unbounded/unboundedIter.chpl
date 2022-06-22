config const case = 1;

select case {
when 1 do
  for i in 120:int(8).. do
    writeln(i);

when 2 do
  for i in 120:int(8).. by 2 do
    writeln(i);

when 3 do
  for i in 121:int(8).. by 2 do
    writeln(i);

when 4 do
  for i in ..-120:int(8) by -1 do
    writeln(i);

when 5 do
  for i in ..-120:int(8) by -2 do
    writeln(i);

when 6 do
  for i in ..-121:int(8) by -2 do
    writeln(i);
}
