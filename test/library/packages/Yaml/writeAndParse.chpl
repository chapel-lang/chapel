use Yaml;

var y = parseYamlFile("test.yaml");
// writeln(y[0]);
// writeln((y[0]:YamlMapping)._map.keys());
writeYamlFile("test.out.yaml", y[0]["items"]);
