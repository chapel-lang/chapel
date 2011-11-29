extern record R {
  // WORKAROUND (uncomment:)
  //  var dummy: int;
}

var myR: R;

extern proc foo(myR:R);

foo(myR);
