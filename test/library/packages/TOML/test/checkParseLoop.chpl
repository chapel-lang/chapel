import TOML;
use TOML.TomlParser;

var tomlStr = "t = true\nf = false";
var D: domain(string);
var table: [D] shared Toml?;
var rootTable = new shared Toml(table);
const source = new shared TOML.TomlReader.Source(tomlStr);
const parser = new shared Parser(source, rootTable);
const tomlData = parser.parseLoop();
writeln(tomlData);
