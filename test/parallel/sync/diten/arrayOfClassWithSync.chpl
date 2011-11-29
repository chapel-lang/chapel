class C {
  var s: sync bool;
}

var a: [1..3] C;

[i in 1..3] a(i) = new C();

a(2).s = true;
a(2).s;
if (a(2).s.isFull) then halt("It should be empty");
[i in 1..3] delete a(i);
