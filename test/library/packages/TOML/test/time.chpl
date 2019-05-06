
use TOML;

config const str: string = """[owner]
                            name = "Foo Bar"
                            timestamp1 = 06:30:30
                            timestamp2 = 06:30:30.123456
                            """;

proc main() {
  var TomlData = parseToml(str);
  var ts1 = TomlData["owner"]["timestamp1"];
  var ts2 = TomlData["owner"]["timestamp2"];
  writeln(ts1.toString());
  writeln(ts2.toString());

  delete TomlData;
}

