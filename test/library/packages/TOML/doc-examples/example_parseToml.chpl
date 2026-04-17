//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
/* START_EXAMPLE */
use IO, TOML;

const tomlFile = open("example.toml", ioMode.r);
const toml = parseToml(tomlFile);
/* STOP_EXAMPLE */

writeln(toml);
