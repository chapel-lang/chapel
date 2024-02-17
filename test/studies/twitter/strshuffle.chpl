config var w="";var L=w.size,D={w};
proc f(s,b,l){if!l{D+=s;}else{for i in 1..L{if!(b&2**i){f(s+w[i-1],b|2**i,l-1);}}}} f("",0,L);write(D);
