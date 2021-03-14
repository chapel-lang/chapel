
use TOML;

config const str: string = """[owner]
                            name = "Foo Bar"
                            dob = 2000-04-30
                            """;

proc main() {
  var TomlData = parseToml(str);
  var dob = TomlData["owner"]!["dob"];
  writeln(dob!.toString());

  delete TomlData;
}

