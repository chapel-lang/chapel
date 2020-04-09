// This tests the behavior of the case-changing routines in the
// presence of an anomalous character.  The lower-case German sharp S
// was encoded in 1985 and takes two UTF-8 bytes, but the upper-case
// version was encoded in 2008 and takes three bytes.

var lo = "ßß";
var up = "ẞẞ";

writeln(lo.toLower(), ", ", lo.toUpper(), ", ", lo.toTitle(), ", ",
	lo.capitalize());
writeln(up.toLower(), ", ", up.toUpper(), ", ", up.toTitle(), ", ",
	up.capitalize());

