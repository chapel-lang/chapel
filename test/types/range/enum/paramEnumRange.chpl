enum color { red, orange, yellow, green, blue, purple };

for param c in color.red..color.blue {
  compilerWarning(c:string);
  writeln(c);
}
compilerWarning("---");
writeln("---");

for param c in color.red..color.blue by 2{
  compilerWarning(c:string);
  writeln(c);
}
compilerWarning("---");
writeln("---");

for param c in color.orange..color.blue by 2 {
  compilerWarning(c:string);
  writeln(c);
}
compilerWarning("---");
writeln("---");

for param c in color.orange..color.purple by 2 {
  compilerWarning(c:string);
  writeln(c);
}
compilerWarning("---");
writeln("---");

for param c in color.blue..color.red {
  compilerWarning(c:string);
  writeln(c);
}
compilerWarning("---");
writeln("---");

for param c in color.blue..color.red by -1 {
  compilerWarning(c:string);
  writeln(c);
}
compilerWarning("---");
writeln("---");

for param c in color.blue..color.red by -2 {
  compilerWarning(c:string);
  writeln(c);
}
compilerWarning("---");
writeln("---");

for param c in color.purple..color.yellow by -2 {
  compilerWarning(c:string);
  writeln(c);
}
compilerWarning("---");
writeln("---");

for param c in color.blue..color.red by -2 {
  compilerWarning(c:string);
  writeln(c);
}
