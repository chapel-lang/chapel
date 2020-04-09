use Regexp;

var re = "foobar".match(compile(".*oob.*"));

writeln(re.length);
