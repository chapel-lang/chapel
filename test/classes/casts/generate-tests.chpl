use Set;
use IO;
use List;
import FileSystem as FS;
import OS.POSIX as OS;

proc getType(managment, cls) {
  var isNilable = managment.endsWith("?");
  var m = managment.strip("?");
  return "%s %s%s".format(m, cls, if isNilable then "?" else "");
}

proc getAllocationType(managment, cls) {
  var allocation = managment;
  if allocation == "borrowed" then allocation = "owned";
  if allocation == "borrowed?" then allocation = "owned?";

  return getType(allocation, cls);
}

var D = {1..4};
var managmentTypes = ["owned", "shared", "unmanaged", "borrowed"];
var managmentTypesAll: [D.low..D.high*2] string;
managmentTypesAll[D.low..D.high] = managmentTypes;
managmentTypesAll[D.high+1..D.high*2] = ([x in managmentTypes] x + "?");

// generate generic stuff all tests need
proc generateFile(filename: string,
                  ref chplLines: list(string)) {
  chplLines.insert(0, "// AUTO-GENERATED: Do not edit");
  chplLines.insert(1, "class A {}");
  chplLines.insert(2, "class Parent {}");
  chplLines.insert(3, "class Child : Parent {}");


  chplLines.append("proc main() {");
  chplLines.append("  foo();");
  chplLines.append("}");

  {
    var w = openWriter(filename+".chpl");
    for l in chplLines do w.writeln(l);
  }
}

proc generateFilename(in fromType: string, in toType:string) {
  var fromNilable = fromType.endsWith("?");
  fromType = fromType.strip("?").replace(" ", "-");
  if fromNilable then fromType += "-nilable";
  var toNilable = toType.endsWith("?");
  toType = toType.strip("?").replace(" ", "-");
  if toNilable then toType += "-nilable";

  return "from-%s-to-%s".format(fromType, toType);
}

// returns (noerrorPath, errorPath)
proc generateDirectoryStructure(modName: string): 2*string {
  const noerrorPath = "%s/noerror".format(modName);
  const errorPath = "%s/error".format(modName);
  FS.mkdir(noerrorPath, parents=true);
  FS.mkdir(errorPath, parents=true);
  { var w = openWriter("%s/NOEXEC".format(noerrorPath)); }
  { var w = openWriter("%s/NOEXEC".format(errorPath)); }

  { var w = openWriter("%s/noerror.good".format(noerrorPath)); }
  {
    var w = openWriter("%s/COMPOPTS".format(noerrorPath));
    w.writeln("--stop-after-pass callDestructors # noerror.good");
  }
  {
    var w = openWriter("%s/error.good".format(errorPath));
    w.writeln("Compiler correctly threw an error");
  }
  {
    var w = openWriter("%s/COMPOPTS".format(errorPath));
    w.writeln("--stop-after-pass callDestructors # error.good");
  }
  {
    var prediff = "%s/PREDIFF".format(errorPath);
    var w = openWriter(prediff);
    w.writeln("#!/bin/sh");
    w.writeln("if [ -s $2 ]; then");
    w.writeln("echo 'Compiler correctly threw an error' >$2");
    w.writeln("fi");
    w.close();
    OS.chmod(prediff.c_str(), 0o755:OS.mode_t);
  }

  return (noerrorPath, errorPath);
}

proc generate(modName: string,
              allowed: set(2*string),
              allowedUpcast: set(2*string),
              allowedDowncast: set(2*string),
              writeEachTestCase) {
  
  if FS.exists(modName) then FS.rmTree(modName);
  const (noerrorPath, errorPath) = generateDirectoryStructure(modName);

  proc writeTestCase(const ref allowList: set(2*string),
                     from: string,
                     fromClass: string,
                     to: string,
                     toClass: string) {
    const isLegal = allowList.contains((from, to));
    const fromType = getType(from, fromClass);
    const allocFromType = getAllocationType(from, fromClass);
    const toType = getType(to, toClass);
    const allocToType = getAllocationType(to, toClass);
    
    var chplLines = writeEachTestCase(isLegal, fromType, allocFromType, toType, allocToType);
    
    var filename = generateFilename(fromType, toType);
    if isLegal
      then filename = "%s/%s".format(noerrorPath, filename);
      else filename = "%s/%s".format(errorPath, filename);
    generateFile(filename, chplLines);
  }

  for from in managmentTypesAll {
    for to in managmentTypesAll {
      writeTestCase(allowed, from, "A", to, "A");
      writeTestCase(allowedUpcast, from, "Child", to, "Parent");
      writeTestCase(allowedDowncast, from, "Parent", to, "Child");
    }
  }

}




proc generateExplicitCasts() {
  const modName = "explicitCasts";

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


  proc writeEachTestCase(isLegal: bool,
                         fromType: string,
                         allocFromType: string,
                         toType: string,
                         allocToType: string): list(string) {
    var chplLines: list(string);

    chplLines.append("proc foo() {");
    chplLines.append("  // casting from %s to %s".format(fromType, toType));
    if allocFromType != fromType {
      chplLines.append("  var alloc = new %s();".format(allocFromType));
      chplLines.append("  var a:%s = alloc;".format(fromType));
    }
    else {
      chplLines.append("  var a = new %s();".format(fromType));
    }
    chplLines.append("  var a_ = a:%s;".format(toType));
    chplLines.append("}");
    return chplLines;
  }
  generate(modName, allowed, allowedUpcast, allowedDowncast, writeEachTestCase);
  
}






proc generateCoerceInitAndAssign() {

  var allowed: set(2*string);
  // coerce owned to ...
  for x in ["owned", "borrowed", "owned?", "borrowed?"] do
    allowed.add(("owned", x));

  // coerce shared to ...
  for x in ["shared", "borrowed", "shared?", "borrowed?"] do
    allowed.add(("shared", x));

  // coerce unmanaged to ...
  for x in ["unmanaged", "borrowed", "unmanaged?", "borrowed?"] do
    allowed.add(("unmanaged", x));
  
  // coerce borrowed to ...
  for x in ["borrowed", "borrowed?"] do
    allowed.add(("borrowed", x));

  // coerce owned? to ...
  for x in ["owned?", "borrowed?"] do
    allowed.add(("owned?", x));

  // coerce shared? to ...
  for x in ["shared?", "borrowed?"] do
    allowed.add(("shared?", x));

  // coerce unmanaged? to ...
  for x in ["unmanaged?", "borrowed?"] do
    allowed.add(("unmanaged?", x));
  
  // coerce borrowed? to ...
  for x in ["borrowed?"] do
    allowed.add(("borrowed?", x));
  
  // upcast is the same
  var allowedUpcast: set(2*string);
  for x in allowed do allowedUpcast.add(x);

  // downcast is not supported
  var allowedDowncast: set(2*string);


  proc writeEachTestCaseInit(
                     isLegal: bool,
                     fromType: string,
                     allocFromType: string,
                     toType: string,
                     allocToType: string): list(string) {
    var chplLines: list(string);
    chplLines.append("proc foo() {");
    chplLines.append("  // coercing from %s to %s".format(fromType, toType));
    if allocFromType != fromType {
      chplLines.append("  var alloc = new %s();".format(allocFromType));
      chplLines.append("  var a:%s = alloc;".format(fromType));
    }
    else {
      chplLines.append("  var a = new %s();".format(fromType));
    }
    chplLines.append("  var a_:%s = a;".format(toType));
    chplLines.append("}");
    return chplLines;
  }

  proc writeEachTestCaseAssign(
                     isLegal: bool,
                     fromType: string,
                     allocFromType: string,
                     toType: string,
                     allocToType:string): list(string) {
    var chplLines: list(string);
    chplLines.append("proc foo() {");
    chplLines.append("  // coercing from %s to %s".format(fromType, toType));
    if allocFromType != fromType {
      chplLines.append("  var allocFrom = new %s();".format(allocFromType));
      chplLines.append("  var a:%s = allocFrom;".format(fromType));
    }
    else {
      chplLines.append("  var a = new %s();".format(fromType));
    }
    if allocToType != toType {
      chplLines.append("  var allocTo = new %s();".format(allocToType));
      chplLines.append("  var a_:%s = allocTo;".format(toType));
    }
    else {
      chplLines.append("  var a_ = new %s();".format(toType));
    }
    chplLines.append("  a_ = a;");
    chplLines.append("}");
    return chplLines;
  }

  generate("coerceInitAssign", allowed, allowedUpcast, allowedDowncast, writeEachTestCaseInit);
  generate("coerceAssign", allowed, allowedUpcast, allowedDowncast, writeEachTestCaseAssign);

}



proc main() {
  generateExplicitCasts();
  generateCoerceInitAndAssign();
}
