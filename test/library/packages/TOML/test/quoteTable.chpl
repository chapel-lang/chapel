
use TOML;

config const str: string = """
[LocalAtomics."0.1.0"]
score="3"

[DataStructures."0.1.0"]
score ="1"

[GPUIterator."0.0.1"]
score= "4"

# key = "value"
[NumpyLike."0.1.0"]
score = "5"

[csm."0.1.0"]
score = "4"
""";

proc main() {
  var TomlData = parseToml(str);
  const score1 = TomlData['LocalAtomics."0.1.0"']!['score']!.s;
  const score2 = TomlData['DataStructures."0.1.0"']!['score']!.s;
  const score3 = TomlData['GPUIterator."0.0.1"']!['score']!.s;
  const score4 = TomlData['NumpyLike."0.1.0"']!['score']!.s;
  const score5 = TomlData['csm."0.1.0"']!['score']!.s;
  writeln(score1);
  writeln(score2);
  writeln(score3);
  writeln(score4);
  writeln(score5);
  delete TomlData;
}


