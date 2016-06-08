var iTen : int = 0;

writeln(c'iTen (expect  0) ', iTen);

if true then
  iTen = 6;

writeln(c'iTen (expect  6) ', iTen);

if false then
  iTen = 2;

writeln(c'iTen (expect  6) ', iTen);

if iTen > 9 then
  iTen = 2;
else
  iTen = 10;

writeln(c'iTen (expect 10) ', iTen);
quit();
