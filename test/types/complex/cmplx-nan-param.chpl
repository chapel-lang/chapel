use Math;

// check what happens when converting a param "nan" string to complex
{
  var nan = "nan": complex;
  writeln(nan);
}
{
  var nan = "nani": complex;
  writeln(nan);
} 

// check for non-param string as well in case the behavior differs (at
// present it doesn't)

config var str = "nan";
config var str2 = "nani";

{
  var nan = str: complex;
  writeln(nan);
}
{
  var nan = str2: complex;
  writeln(nan);
}

// check that we can create a proper nan param string
{
  param nancmplx = nan + nan*1.0i;
  param str2 = nancmplx: string;
  writeln(str2);
}
{
  param nancmplx = nan + 3.4i;
  param str2 = nancmplx: string;
  writeln(str2);
}
{
  param nancmplx = 1.2 + nan*1.0i;
  param str2 = nancmplx: string;
  writeln(str2);
}
