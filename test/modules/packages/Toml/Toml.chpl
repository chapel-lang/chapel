/*
Chapel's Library for `Tom's Obvious, Minimal Language (TOML)
              <https://github.com/toml-lang/toml>`_.
This module provides support for parsing and writing toml files.
*/

use parser;
use reader;

/*
Receives a channel to a TOML file as a parameter and outputs an associative
array Node.
*/
proc parseToml(input: file) : Node {
  const source = new Source(input);
  const parser = new Parser(source);
  return parser.parseLoop();
}

/*
Receives a string of TOML format as a parameter and outputs an associative
array Node.
*/
proc parseToml(input: string) : Node {
  const source = new Source(input);
  const parser = new Parser(source);
  return parser.parseLoop();
}