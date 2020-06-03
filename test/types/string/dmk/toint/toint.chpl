for c in "AEIOU" {
  if c.toByte() != c.byte(0) then
    writeln("mismatch: '", c, "' ", c.toByte(), " ", c.byte(0));
  if c.toCodepoint() != c.codepoint(0) then
    writeln("mismatch: '", c, "' ", c.toCodepoint(), " ", c.codepoint(0));
}

for c in "ÀÉÌÓÙ" {
  if c.toCodepoint() != c.codepoint(0) then
    writeln("mismatch: '", c, "' ", c.toCodepoint(), " ", c.codepoint(0));
}
