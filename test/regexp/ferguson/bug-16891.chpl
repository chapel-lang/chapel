use IO;
use IO.FormattedIO;

config const useRegexp=false;

var ff=openreader("bug-16891.txt");
var got=true;
var ss : string;
while (got) {
  got = ff.readf("""%/([^\n]+)\n/""",ss);
  if (got) then writeln(ss);
}
