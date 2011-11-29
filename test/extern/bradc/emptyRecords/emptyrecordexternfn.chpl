record R {
  // WORKAROUND (uncomment):
  //  var dummy: int;
}

var myR: R;

extern proc foo(inout myR);

foo(myR);
