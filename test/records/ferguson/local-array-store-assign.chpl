use myrecord;

proc myfunction() {

  var A:[1..10] R;

  for i in 1..10 {
    var local2: R;
    local2.init(x = 20);
    local2.verify();
    assert(local2.x == 20);
    A[i] = local2;
    A[i].verify();
    assert(A[i].x == 20);
  }
}

myfunction();

