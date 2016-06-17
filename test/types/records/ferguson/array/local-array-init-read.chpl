use myrecord;

proc myfunction() {

  var A:[1..10] R;

  for i in 1..10 {
    A[i].verify();
    A[i].setup(x=i);
    A[i].verify();
    assert(A[i].x == i);
  }

  for i in 1..10 {
    var localR = A[i];
    localR.verify();
    assert(localR.x == i);
  }
}

myfunction();

verify();

