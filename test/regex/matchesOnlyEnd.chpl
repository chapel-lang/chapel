// from https://github.com/chapel-lang/chapel/issues/20431
module PlayingAround {
  use Regex;
  use IO.FormattedIO;

  proc main () throws {
    var s = 'i string i';
    var pattern = '$';
    var myRegex = new regex(pattern);
    writeln("matches('%s')".format(pattern));
    writeln(myRegex.matches(s));
    writeln();
    writeln("search('%s')".format(pattern));
    writeln(myRegex.search(s));
  }
}
