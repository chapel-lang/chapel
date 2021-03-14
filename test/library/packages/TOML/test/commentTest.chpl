
use TOML;

config const str: string = """[owner]
                            name = ["Foo Bar"] # something
                            # Another comment
                            dob = 2000-30-04-23
                            """;

proc main() {
  try! {
    var TomlData = parseToml(str);
    var dob = TomlData["owner"]!["name"];

    delete TomlData;
  } catch e: TomlError{
    writeln(e.message());
    exit(1);
  }
}
