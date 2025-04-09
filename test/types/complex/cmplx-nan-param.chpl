// check what happens when converting a param "nan" string to complex
var nan = "nan": complex;
writeln(nan);

// check for non-param string as well in case the behavior differs (at
// present it doesn't)
config var str = "nan";
var nan2 = str: complex;
writeln(nan2);

// check that we can create a proper nan param string
{
  param nancmplx = 0.0/0.0 + 0.0/0.0*1.0i;
  param str2 = nancmplx: string;
  writeln(str2);
}
{
  param nancmplx = 0.0/0.0 + 3.4i;
  param str2 = nancmplx: string;
  writeln(str2);
}
{
  param nancmplx = 1.2 + 0.0/0.0*1.0i;
  param str2 = nancmplx: string;
  writeln(str2);
}
