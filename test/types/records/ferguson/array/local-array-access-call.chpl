use myrecord;

proc test(x: R)
{
  x.verify();
}

proc myfunction() {

  var A:[1..10] R;

  for i in 1..10 {
    A[i].setup(x=i);
    A[i].verify();
    assert(A[i].x == i);
    test(A[i]);
  }
}

myfunction();

verify();

