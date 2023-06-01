use Yaml;

var y = parseYamlFile("test.yaml");
writeln(y);
writeYamlFile("test.out.yaml", y[0]);
