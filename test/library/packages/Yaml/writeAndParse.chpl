use Yaml;

var y = parseYamlFile("test.yaml");
writeYamlFile("test.out.yaml", y[0]);
