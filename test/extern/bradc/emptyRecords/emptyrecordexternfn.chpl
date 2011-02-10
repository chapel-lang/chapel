record R {
  // WORKAROUND (uncomment):
  //  var dummy: int;
}

var myR: R;

_extern proc foo(inout myR);

foo(myR);
