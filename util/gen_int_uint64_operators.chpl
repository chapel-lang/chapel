// This file was used to generate operators for addition into the ChapelBase
// standard module. The operators are designed to cause compiler errors when
// a uint(64) and signed int are used together unless the signed int is a
// param and it is non-negative. The operators to generate are listed in
// infilename which is terminated with the word "end_end_end"

config const infilename = "operator_defs";
def main() {
  var infile = new file(infilename, FileAccessMode.read, ".");
  var done = false;
  infile.open();
  while (!done) {
    var operator: string;
    infile.read(operator);
    if (operator == "end_end_end") {
      done = true;
      continue;
    }
    write_functions(operator);
  }
  infile.close();
}

def write_functions(operator: string) {
  writeln("// non-param/non-param");
  writeln("pragma \"inline\" def " + operator + "(a: uint(64), b: int(64)) { _throwOpError(\"" + operator + "\"); }");
  writeln("pragma \"inline\" def " + operator + "(a: int(64), b: uint(64)) { _throwOpError(\"" + operator + "\"); }");
  writeln();
  writeln("// param/param");
  writeln("pragma \"inline\" def " + operator + "(param a: uint(64), param b: int(64)) param {");
  writeln("  if b < 0 then _throwOpError(\"" + operator + "\"); else return __primitive(\"" + operator + "\", a, b: uint(64));");
  writeln("}");
  writeln("pragma \"inline\" def " + operator + "(param a: int(64), param b: uint(64)) param {");
  writeln("  if a < 0 then _throwOpError(\"" + operator + "\"); else return __primitive(\"" + operator + "\", a:uint(64), b);");
  writeln("}");
  writeln();
  writeln("// non-param/param and param/non-param");
  writeln("pragma \"inline\" def " + operator + "(a: uint(64), param b: int(64)) {");
  if operator == "<"  then
    writeln("  if b < 0 then _throwOpError(\"" + operator + "\"); else if b == 0 then return false; else return __primitive(\"" + operator + "\", a, b:uint(64));");
  else if operator == ">=" then
    writeln("  if b < 0 then _throwOpError(\"" + operator + "\"); else if b == 0 then return true; else return __primitive(\"" + operator + "\", a, b:uint(64));");
  else
    writeln("  if b < 0 then _throwOpError(\"" + operator + "\"); else return __primitive(\"" + operator + "\", a, b:uint(64));");
  writeln("}");
  writeln("pragma \"inline\" def " + operator + "(param a: int(64), b: uint(64)) {");
  if operator == ">" then
    writeln("  if a < 0 then _throwOpError(\"" + operator + "\"); else if a == 0 then return false; else return __primitive(\"" + operator + "\", a:uint(64), b);");
  else if operator == "<=" then
    writeln("  if a < 0 then _throwOpError(\"" + operator + "\"); else if a == 0 then return true; else return __primitive(\"" + operator + "\", a:uint(64), b);");
  else
    writeln("  if a < 0 then _throwOpError(\"" + operator + "\"); else return __primitive(\"" + operator + "\", a:uint(64), b);");
  writeln("}");
  writeln(); writeln();
}

