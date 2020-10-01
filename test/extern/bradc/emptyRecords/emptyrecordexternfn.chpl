record R {
  // WORKAROUND (uncomment):
  //  var dummy: int;
}

var myR: R;

extern proc foo(ref myR);

foo(myR);
