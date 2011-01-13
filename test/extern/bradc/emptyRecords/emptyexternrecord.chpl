_extern record R {
  // WORKAROUND (uncomment:)
  //  var dummy: int;
}

var myR: R;

_extern def foo(myR:R);

foo(myR);
