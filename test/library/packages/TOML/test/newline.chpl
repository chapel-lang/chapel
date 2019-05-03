
use TOML;

config const str: string = """[owner]

                            name = "Foo Bar"
                            dob = 2000-04-30-234-42
                            field = "someValue"
                            """;

proc main() {
  var TomlData = parseToml(str);
  writeln(TomlData);

  delete TomlData;
}

