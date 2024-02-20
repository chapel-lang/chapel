use IO;
use IO.FormattedIO;

config const useRegex=false;

var ff=openReader("bug-16891.txt", locking=false);
var got=true;
var ss : string;
while (got) {
  got = ff.readf("""%/([^\n]+)\n/""",ss);
  if (got) then writeln(ss);
}
