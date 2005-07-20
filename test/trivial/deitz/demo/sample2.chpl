var original = "shuffle me please";

writeln("original: ", original);

var shuffled = original(1..17 by 2) + original(2..17 by 2);

writeln("shuffled: ", shuffled);

var decoder : seq of integer;

for i in 1..9 do
  decoder = decoder # (/ i, 9 + i /);

writeln("decoder:  ", decoder);

write("decoded:  ");
for i in decoder do
  write(shuffled(i));
writeln();
