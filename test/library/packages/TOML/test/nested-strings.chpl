use TOML, IO;

const s = """
myStr1 = ""
myStr2 = ''
myStr3 = "'"
myStr4 = '"'

[myTbl]
myQuotedStr1 = "I have a 'quoted string'"
myQuotedStr2 = 'I have a "quoted string"'
myQuotedStr3 = "'quotes at the beginning' for fun"
myQuotedStr4 = '"quotes at the beginning" for fun'

myArr = [
  "isn't this fun",
  '"quotes galore"',
  "''",
  '""',
  '',
  ""
]

[someOtherTable]
key = "value"
""";

var toml = parseToml(s);
writeln("toml output");
writeln(toml);
writeln("json output");
toml.writeJSON(stdout);


var mem = openMemFile();
var w = mem.writer();
w.writeln(toml);
w.close();

var roundTrippedToml = parseToml(mem.reader());
writeln("round-tripped toml output");
writeln(roundTrippedToml);

