config var w="chapel",L=w.length,D={w};
proc f(s,b,l){
  if!l{D+=s;}else[i in 1..L]if!(b&2**i){f(s+w[i],b|2**i,l-1);}
}
f("",0,L);
write(D);