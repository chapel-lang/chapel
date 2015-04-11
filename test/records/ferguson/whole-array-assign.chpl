use myrecord;

proc myfunction() {

  var A:[1..10] R;

  for i in 1..10 {
    A[i].verify();
    A[i].init(x=i);
    A[i].verify();
    assert(A[i].x == i);
  }

  var B:[1..10] R;

  B = A;

  for i in 1..10 {
    B[i].verify();
    assert(B[i].x == i);
  }

}

myfunction();

