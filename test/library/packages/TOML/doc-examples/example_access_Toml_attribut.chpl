//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
/* START_EXAMPLE */
use IO, TOML;

const tomlFile = open("example.toml", ioMode.r);
const toml = parseToml(tomlFile);
const projectName = toml["root"]!["name"]; // returns a Toml object
writeln(projectName!.toString());     // to turn Toml object into string representation
/* STOP_EXAMPLE */
