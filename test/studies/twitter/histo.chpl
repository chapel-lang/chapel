use IO;
config var b=4,n=-1;
var A:[0..#2**b] int,x=0;
while stdin.readBits(x,b) && n!=0 {
  A[x]+=1;
  n-=1;}
for a in A do writeln("#"*a);
