function zero(A : [?D] float) {
  forall i in D {
    A(i) = 0;
  }
}

var a1 : [1..10] float;
var a2 : [1..10, 1..10] float;

zero(a1);
zero(a2);
write(a1(1));
write(a2(1,1));
