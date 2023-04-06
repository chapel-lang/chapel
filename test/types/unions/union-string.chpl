import IO.FormattedIO.format;

union U {
  var s: string;
}

var u: U;
u.s = "Cogito, ergo sum";

var t = "%t".format(u);
writeln(t);
