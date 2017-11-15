enum Identifier {
  Constant1,
  Constant2,
  Constant3
};

// Just to make it not a param string
proc id(s:string) {
  return s;
}

writeln("Constant1":Identifier);
writeln("Identifier.Constant1":Identifier);
writeln("Constant2":Identifier);
writeln("Identifier.Constant2":Identifier);
writeln("Constant3":Identifier);
writeln("Identifier.Constant3":Identifier);

writeln((id("Constant1")):Identifier);
writeln((id("Identifier.Constant1")):Identifier);
writeln((id("Constant2")):Identifier);
writeln((id("Identifier.Constant2")):Identifier);
writeln((id("Constant3")):Identifier);
writeln((id("Identifier.Constant3")):Identifier);
