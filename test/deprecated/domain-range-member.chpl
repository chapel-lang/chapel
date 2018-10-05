// This test is to remind us to remove domain/range .member
// It's deprecated starting in Chapel 1.19 and should be
// present but deprecated for one release after that
// (and then removed in the next release).

var d = {1..10};
var r = 1..10;

writeln(d.member(1));
writeln(r.member(1));
