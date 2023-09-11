
// only low and high default initialization should be unstable
var drlb: range(int, boundKind.low),
    drub: range(int, boundKind.high),
    drnb: range(int, boundKind.neither),
    drbb: range(int, boundKind.both);

// none of these should produce an unstable warning
var rlb = 1..,
    rub = ..0,
    rnb = ..,
    rbb = 1..0;

writeln(drlb);
writeln(drub);
