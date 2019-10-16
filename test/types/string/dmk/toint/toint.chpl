for c in "AEIOU" {
  if c.toByte() != c.byte(1) then
    writeln("mismatch: '", c, "' ", c.toByte(), " ", c.byte(1));
  if c.toCodepoint() != c.codepoint(1) then
    writeln("mismatch: '", c, "' ", c.toCodepoint(), " ", c.codepoint(1));
}

for c in "ÀÉÌÓÙ" {
  if c.toCodepoint() != c.codepoint(1) then
    writeln("mismatch: '", c, "' ", c.toCodepoint(), " ", c.codepoint(1));
}
