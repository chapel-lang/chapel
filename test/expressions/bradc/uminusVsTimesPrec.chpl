//
// This is the only case I could come up with for why unary minus
// should bind more tightly than division.  Are there other, more
// subtle issues?
//

var m = min(int(32));
writeln("m = ", m);
writeln("-m/2 = ", -m/2);
writeln("-(m/2) = ", -(m/2));
writeln("(-m)/2 = ", (-m)/2);
