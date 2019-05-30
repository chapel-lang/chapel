
use TOML;

config const str: string = """[owner]
                            name = "Foo Bar"
                            timestamp1 = 06:30:30
                            timestamp2 = 06:30:30.123456
                            timestamp3 = 06:30:30.1234567
                            """;

proc main() {
  var TomlData = parseToml(str);
  var ts1 = TomlData["owner"]["timestamp1"];
  var ts2 = TomlData["owner"]["timestamp2"];
  var ts3 = TomlData["owner"]["timestamp3"];

  writeln(ts1.toString());
  writeln(ts2.toString());
  writeln(ts3.toString());

  delete TomlData;
}

