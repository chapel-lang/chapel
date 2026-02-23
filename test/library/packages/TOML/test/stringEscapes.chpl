
use TOML, IO;

const str1 = '[table]\nkey = %sFoo, Bar%s';
const str2 = '[table]\nkey = %sFoo \t\\ Bar%s';
const str3 = """
[table]
key = %sFoo \ Bar%s
""";
const str4 = """
[table]
key = %sFoo \'\"\\ Bar%s
""";
const str4_raw = """
[table]
key = %sFoo \"\\ Bar%s
""";
const str5 = '[table]\nkey = "Foo, \' Bar"';
const str6 = "[table]\nkey = 'Foo, \" Bar'";

proc test(s, quoteType="") {
  var str:string;
  if quoteType == ""
    then str = s;
    else str = s.format(quoteType, quoteType);
  var TomlData = parseToml(str);
  var value = TomlData["table.key"]!;
  writef("toString  : %s\nraw string: '%s'\n=====\n",
          value.toString(), value.s);
}

proc main() {
  writeln("Test with single quotes (raw strings):");
  test(str1, "'");
  test(str2, "'");
  test(str3, "'");
  test(str4_raw, "'");

  writeln("Test with double quotes:");
  test(str1, '"');
  test(str2, '"');
  test(str3, '"');
  test(str4, '"');

  writeln("Test with escaped quotes:");
  test(str5);
  test(str6);
}

