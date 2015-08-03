extern record R {
  // WORKAROUND (uncomment:)
  //  var dummy: int;
}

var myR: R;

extern proc foo(const in myR:R);

foo(myR);
