config const n=5;
h(n);
proc h(n,s=1,d=2,e=3) {
  proc p {writeln(n,": ",s," to ",d);}
  if n==1 {p;} else {h(n-1,s,e,d);p;h(n-1,e,d,s);}
}
