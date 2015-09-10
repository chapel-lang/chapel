var i   : int =  0;
var n   : int = 10;

var res : int =  1;

while (i < n)
{
  res = res * 2;
  i   = i   + 1;
}

writeln('i:   ', i);
writeln('n:   ', n);
writeln('res: ', res);
quit();
