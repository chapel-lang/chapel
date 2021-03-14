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
  writeln('// non-param/non-param');
  writeln('pragma "inline" def ' + operator + '(a: uint(64), b: int(64)) { _throwOpError("' + operator + '"); }');
  writeln('pragma "inline" def ' + operator + '(a: int(64), b: uint(64)) { _throwOpError("' + operator + '"); }');
  writeln();
  writeln('// param/param');
  writeln('pragma "inline" def ' + operator + '(param a: uint(64), param b: int(64)) param {');
  writeln('  if b < 0 then _throwOpError("' + operator + '"); else return a ', operator, ' b:uint(64);');
  writeln('}');
  writeln('pragma "inline" def ' + operator + '(param a: int(64), param b: uint(64)) param {');
  writeln('  if a < 0 then _throwOpError("' + operator + '"); else return a:uint(64) ', operator, ' b;');
  writeln('}');
  writeln();
  writeln('// non-param/param and param/non-param');
  writeln('pragma "inline" def ' + operator + '(a: uint(64), param b: int(64)) {');
  // The extra check against zero for < and >= is required because some
  // compilers warn about comparing uint values with zero.
  // uint < 0 is always false. uint >= 0 is always true.
  if operator == "<" then
    writeln('  if b < 0 then _throwOpError("' + operator + '"); else return if b == 0 then false else a ' + operator + ' b:uint(64);');
  else if operator == ">=" then
    writeln('  if b < 0 then _throwOpError("' + operator + '"); else return if b == 0 then true else a ' + operator + ' b:uint(64);');
  else
    writeln('  if b < 0 then _throwOpError("' + operator + '"); else return a ' + operator + ' b:uint(64);');
  writeln('}');
  writeln('pragma "inline" def ' + operator + '(param a: int(64), b: uint(64)) {');
  // The extra check against zero for > and <= is required because some
  // compilers warn about comparing uint values with zero.
  // 0 > uint is always false. 0 <= uint is always true.
  if operator == ">" then
    writeln('  if a < 0 then _throwOpError("' + operator + '"); else return if a == 0 then false else a:uint(64) ' + operator + ' b;');
  else if operator == "<=" then
    writeln('  if a < 0 then _throwOpError("' + operator + '"); else return if a == 0 then true else a:uint(64) ' + operator + ' b;');
  else
    writeln('  if a < 0 then _throwOpError("' + operator + '"); else return a:uint(64) ' + operator + ' b;');
  writeln('}');
  writeln(); writeln();
}

