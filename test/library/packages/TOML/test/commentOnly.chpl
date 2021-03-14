use TOML;

config const str: string = """# Comment""";

proc main() {
  var TomlData = parseToml(str);
  writeln(TomlData);
  delete TomlData;
}


