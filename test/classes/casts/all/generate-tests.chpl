use Set;
use IO;
use List;

proc getType(managment, cls) {
  var isNilable = managment.endsWith("?");
  var m = managment.strip("?");
  return "%s %s%s".format(m, cls, if isNilable then "?" else "");
}

proc getAllocationType(managment, cls) {
  var allocation = managment.strip("?");
  if allocation == "borrowed" then allocation = "owned";

  return getType(allocation, cls);
}

var D = {1..4};
var managmentTypes = ["owned", "shared", "unmanaged", "borrowed"];
var managmentTypesAll: [D.low..D.high*2] string;
managmentTypesAll[D.low..D.high] = managmentTypes;
managmentTypesAll[D.high+1..D.high*2] = ([x in managmentTypes] x + "?");

// generate generic stuff all tests need
proc generateFiles(modName: string,
                   funcName: string,
                   funcIdxs: range,
                   ref chplLines: list(string),
                   ref goodLines: list(string)) {
  {
    var w = openWriter(modName+".compopts");
    w.writeln("--ignore-errors-for-pass --print-callstack-on-error");
  }
  {
    var w = openWriter(modName+".noexec");
  }

  chplLines.append("class A {}");
  chplLines.append("class Parent {}");
  chplLines.append("class Child : Parent {}");
  chplLines.append("proc myError(param a: string) { compilerError(a); }");


  chplLines.append("proc main() {");
  for i in funcIdxs {
    chplLines.append("  T%n.%s();".format(i, funcName));
  }
  chplLines.append("}");

  {
    var w = openWriter(modName+".chpl");
    for l in chplLines do w.writeln(l);
  }
  {
    var w = openWriter(modName+".good");
    for l in goodLines do w.writeln(l);
  }
}

proc generateExplicitCasts() {
  const modName = "explicitCasts";
  const funcName = "foo";
  
  var chplLines: list(string);
  var goodLines: list(string);

  var allowed: set(2*string);
  // cast owned to ...
  for x in ["owned", "shared", "unmanaged", "borrowed", "owned?", "unmanaged?", "borrowed?"] do
    allowed.add(("owned", x));

  // cast shared to ...
  for x in ["shared", "unmanaged", "borrowed", "shared?", "unmanaged?", "borrowed?"] do
    allowed.add(("shared", x));

  // cast unmanaged to ...
  for x in ["unmanaged", "borrowed", "unmanaged?", "borrowed?"] do
    allowed.add(("unmanaged", x));
  
  // cast borrowed to ...
  for x in ["unmanaged", "borrowed", "unmanaged?", "borrowed?"] do
    allowed.add(("borrowed", x));

  // cast owned? to ...
  for x in ["owned", "unmanaged", "borrowed", "owned?", "shared?", "unmanaged?", "borrowed?"] do
    allowed.add(("owned?", x));

  // cast shared? to ...
  for x in ["shared", "unmanaged", "borrowed", "shared?", "unmanaged?", "borrowed?"] do
    allowed.add(("shared?", x));

  // cast unmanaged? to ...
  for x in ["unmanaged", "borrowed", "unmanaged?", "borrowed?"] do
    allowed.add(("unmanaged?", x));
  
  // cast borrowed? to ...
  for x in ["unmanaged", "borrowed", "unmanaged?", "borrowed?"] do
    allowed.add(("borrowed?", x));
  
  // upcast is close to the same
  var allowedUpcast: set(2*string);
  for x in allowed do allowedUpcast.add(x);
  // upcast does not support owned->shared
  allowedUpcast.remove(("owned", "shared"));
  allowedUpcast.remove(("owned?", "shared"));
  allowedUpcast.remove(("owned", "shared?"));
  allowedUpcast.remove(("owned?", "shared?"));

  // downcast is the same as upcast
  var allowedDowncast: set(2*string);
  for x in allowedUpcast do allowedDowncast.add(x);


  proc writeEachTestCase(ref chplLines: list(string),
                     ref goodLines: list(string),
                     idx: int,
                     isLegal: bool,
                     fromType: string,
                     allocType: string,
                     toType: string) {
    chplLines.append("module T%n {".format(idx));
    chplLines.append("  use %s;".format(modName));
    chplLines.append("  proc %s() {".format(funcName));
    chplLines.append("    // casting from %s to %s".format(fromType, toType));
    chplLines.append("    var alloc = new %s();".format(allocType));
    chplLines.append("    var a:%s = alloc;".format(fromType));
    chplLines.append("    var a_ = a:%s;".format(toType));
    if isLegal {
      const errStr = "%s to %s is legal".format(fromType, toType);
      chplLines.append("    myError(\"KEEP ME: %s\");".format(errStr));
      const lineNum = chplLines.size;
      goodLines.append("%n - %s".format(lineNum, errStr));
    }
    else {
      const lineNum = chplLines.size;
      goodLines.append("%n - %s".format(lineNum, "compiler error"));
    }
    
    chplLines.append("  }");
    chplLines.append("}");
  }
  proc writeTestCase(allowList: set(2*string),
                     from: string,
                     fromClass: string,
                     to: string,
                     toClass: string,
                     ref chplLines: list(string),
                     ref goodLines: list(string),
                     idx: int) {
    const isLegal = allowList.contains((from, to));
    const fromType = getType(from, fromClass);
    const allocType = getAllocationType(from, fromClass);
    const toType = getType(to, toClass);
    writeEachTestCase(chplLines, goodLines, idx, isLegal, fromType, allocType, toType);
  }

  const startIdx = 1;
  var idx = startIdx;
  for from in managmentTypesAll {
    for to in managmentTypesAll {
      writeTestCase(allowed, from, "A", to, "A", chplLines, goodLines, idx);
      writeTestCase(allowedUpcast, from, "Child", to, "Parent", chplLines, goodLines, idx+1);
      writeTestCase(allowedDowncast, from, "Parent", to, "Child", chplLines, goodLines, idx+2);
      idx += 3;
    }
  }

  generateFiles(modName, funcName, startIdx..<idx, chplLines, goodLines);
}



proc main() {
  generateExplicitCasts();
}
