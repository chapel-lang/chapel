
use TOML;

config const str: string = """[owner]
                            name = "Foo Bar" # a name
                            timestamp1 = 06:30:30
                            timestamp2 = 06:30:30.123456
                            timestamp3 = 06:30:30.1234567 # rounding up
                            timestamp4 = 06:30:30.1234564 # rounding down
                            timestamp5 = 06:30:30.9999999 # edge case
                            """;

proc main() {
  var TomlData = parseToml(str);
  var ts1 = TomlData["owner"]!["timestamp1"];
  var ts2 = TomlData["owner"]!["timestamp2"];
  var ts3 = TomlData["owner"]!["timestamp3"];
  var ts4 = TomlData["owner"]!["timestamp4"];
  var ts5 = TomlData["owner"]!["timestamp5"];

  writeln(ts1!.toString());
  writeln(ts2!.toString());
  writeln(ts3!.toString());
  writeln(ts4!.toString());
  writeln(ts5!.toString());

  delete TomlData;
}

