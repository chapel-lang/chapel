proc foo(type t) {
  var x: t;
  writeln(x);
}

{
  foo([1..3] int);
  foo([1..3] [1..3] int);
  foo([1..3] [1..3] [1..3] int);
}

{
  record R { var x = 10; }
  foo([1..3] R);
  foo([1..3] [1..3] R);
  foo([1..3] [1..3] [1..3] R);
}

{
  class C { var x = 10; }
  foo([1..3] owned C?);
  foo([1..3] [1..3] owned C?);
  foo([1..3] [1..3] [1..3] owned C?);
}

{
  class C { var x = 10; }
  foo([1..3] shared C?);
  foo([1..3] [1..3] shared C?);
  foo([1..3] [1..3] [1..3] shared C?);
}


var r = 1..3;
{
  foo([r] int);
  foo([r] [r] int);
  foo([r] [r] [r] int);
}

{
  record R { var x = 10; }
  foo([r] R);
  foo([r] [r] R);
  foo([r] [r] [r] R);
}

{
  class C { var x = 10; }
  foo([r] owned C?);
  foo([r] [r] owned C?);
  foo([r] [r] [r] owned C?);
}

{
  class C { var x = 10; }
  foo([r] shared C?);
  foo([r] [r] shared C?);
  foo([r] [r] [r] shared C?);
}
