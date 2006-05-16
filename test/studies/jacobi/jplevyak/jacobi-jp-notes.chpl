var m = 3, n = 4;
var D = [1..m, 1..n];
// would have liked to use: var a[D] = 0.0; or perhaps var a = [D] 0.0; or even var a = [i in D] 0.0;
var a : [D] float = 0.0;
var epsilon = 0.01;

writeln("initial = ", a);

var X = [0..m+1, 0..n+1];
var t : [X] float = 0.0;
// would have like to use: t(m+1, ..) = 1.0; // bottom
// also failed: t(m+1, 0..n+1) = 1.0; // bottom
[i in 0..n+1] t(m+1, i) = 1.0; // bottom
var stencil = (/ (-1,-1), (1, -1), (-1, 1), (1, 1) /);
label again;
[i in D] t(i) = a(i);
// does this just make a referece? : var tt = t;
// would have liked to use: [i in D] t(i) = t(i + stencil)/+;
// also failed: [i in D] t(i) = sum(float) (t(i + stencil));
writeln("t = ", t);
[i in D] a(i) = 0.0;
for i in D do for c in stencil do a(i) += t(i(1) + c(1), i(2) + c(2))/4.0;
// would have liked to use: if (fabs(t-tt)/max < epsilon) break;
// tried but failed: if ((max (float) fabs(t-tt)) < epsilon) break; 
// tried but failed: if ((max (float) ([i in X] fabs(t(i)-tt(i)))) < epsilon) break;
// tried but failed: tt = tt - t; var m = max (float) fabs(tt);
writeln("a = ", a);
// works: var d : [D] float;
//        [i in D] d(i) = fabs(a(i) - t(i));
// fails: var d = max(float) ([i in D] fabs(a(i) - t(i)));
// fails: var d = max(float) reduce ([i in D] fabs(a(i) - t(i)));
// fails: if ((max(float) reduce ([i in D] fabs(a(i) - t(i)))) > epsilon) goto again;

var d : [D] float;
[i in D] d(i) = fabs(a(i) - t(i));
if ((max(float) reduce d) > epsilon) goto again;

// worked:
//for i in D {
//  if ((a(i) - t(i)) > epsilon) goto again;
//  if ((t(i) - a(i)) > epsilon) goto again;
//}

writeln("final = ", a);
