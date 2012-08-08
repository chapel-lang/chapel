class C {
  var val: int;
}

const D = {1..10};

var A: [D] C;

for i in 1..5 {
  A(i) = new C(i);
}

write("A is: ");
for i in 1..10 {
  write(A(i).val, " ");
  stdout.flush();
}
