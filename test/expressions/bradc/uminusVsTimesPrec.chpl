//
// This is the only case I could come up with for why unary minus
// should bind more tightly than division.  Are there other, more
// subtle issues?
//
// Note that this test relies on signed integer wraparound which
// isn't well-defined in Chapel, so could be retired or skipped
// if it's found to be non-portable.

var m = min(int(64));
writeln("m = ", m);
writeln("-m/2 = ", -m/2);
writeln("-(m/2) = ", -(m/2));
writeln("(-m)/2 = ", (-m)/2);
