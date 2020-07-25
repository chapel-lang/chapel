
use TOML;

config const str: string = """
[LocalAtomics."0.1.0"]
score = "3"

[DataStructures."0.1.0"]
score = "1"

[GPUIterator."0.0.1"]
score = "4"
""";

proc main() {
  var TomlData = parseToml(str);
  const score1 = TomlData['LocalAtomics."0.1.0"']!['score']!.s;
  const score2 = TomlData['DataStructures."0.1.0"']!['score']!.s;
  const score3 = TomlData['GPUIterator."0.0.1"']!['score']!.s;
  writeln(TomlData);
  delete TomlData;
}


