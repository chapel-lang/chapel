use IO;

extern record R {
  var a,b,c,d,e,f,g,h,i,j,k: int(64);
}

proc testSyserr() {
  var x:syserr;
  writeln(x:int);
}
testSyserr();

proc testMystruct() {
  var st: R;
  writeln(st.a, st.b, st.c, st.d, st.e, st.f, st.g, st.h, st.i, st.j, st.k);
}
testMystruct();

var globSysErr: syserr;
writeln(globSysErr:int);

var g: R;
writeln(g.a, g.b, g.c, g.d, g.e, g.f, g.g, g.h, g.i, g.j, g.k);
