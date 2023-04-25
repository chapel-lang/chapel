use IO, YamlParser, YamlClassHierarchy;

var x = parseYamlFile("./test.yaml");
writeln(x);

writeln("----------------------------------------");
var ym = x[0]: YamlMapping;
writeln(ym["date"]);
writeln(ym["customer"]["first_name"]);
