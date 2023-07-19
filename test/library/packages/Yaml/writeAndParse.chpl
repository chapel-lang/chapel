use YAML;

var y = parseYamlFile("test.yaml");
writeln(y[0]["customer"]["first_name"]);
writeYamlFile("test.out.yaml", y[0]["items"]);
