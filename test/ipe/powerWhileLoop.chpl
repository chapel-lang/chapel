var i   : int =  0;
var n   : int = 10;

var res : int =  1;

while (i < n)
{
  res = res * 2;
  i   = i   + 1;
}

writeln(c'i:   ', i);
writeln(c'n:   ', n);
writeln(c'res: ', res);
quit();
