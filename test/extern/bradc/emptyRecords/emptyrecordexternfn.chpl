record R {
  // WORKAROUND (uncomment):
  //  var dummy: int;
}

var myR: R;

_extern def foo(inout myR);

foo(myR);
