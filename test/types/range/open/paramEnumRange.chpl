enum color { red, green, blue };

for param c in color.red..color.blue {
  compilerWarning(c:string);
  writeln(c);
}

for param c in color.red..<color.blue {
  compilerWarning(c:string);
  writeln(c);
}
