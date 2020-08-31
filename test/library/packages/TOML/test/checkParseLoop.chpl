import TOML;
use TOML.TomlParser;

var tomlStr = "t = true\nf = false";
var D: domain(string);
var table: [D] unmanaged Toml?;
var rootTable = new unmanaged Toml(table);
const source = new unmanaged TOML.TomlReader.Source(tomlStr);
const parser = new unmanaged Parser(source, rootTable);
const tomlData = parser.parseLoop();
writeln(tomlData);
delete parser;
delete source;
delete tomlData;
