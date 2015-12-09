proc Power(n : int) : int
{
  var i   : int =  0;
  var res : int =  1;

  while (i < n)
  {
    res = res * 2;
    i   = i   + 1;
  }

  return res;
}

var n   : int = 10;
var pow : int = Power(n);

writeln(c'Pow: ', pow);
quit();
