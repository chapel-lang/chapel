
use TOML;

config const str: string = """[owner]
                            name = "Foo Bar"
                            timestamp = 06:30:30
                            """;

proc main() {
  var TomlData = parseToml(str);
  var timestamp = TomlData["owner"]["timestamp"];
  writeln(timestamp.toString());

  delete TomlData;
}

