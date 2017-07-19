
use TOML;

config const file: string;

proc main() {
  var tomlFile = parseToml(file);
  writeln(tomlFile);
} 