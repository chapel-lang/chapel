use Set;
use IO;
use List;
import FileSystem as FS;
import OS.POSIX as OS;

// controls whether this generates error test cases or no error test cases
config var generateErrorCases: bool = true;
// controls which suite gets generated
config var generateSuite: string = "explicitCasts";

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

proc generateDirectoryFiles(errorFiles: bool = true) {
  { openWriter("NOEXEC"); }
  {
    var w = openWriter("all.good");
    if errorFiles
      then w.writeln("Compiler correctly threw an error");
  }
  {
    var w = openWriter("COMPOPTS");
    w.writeln("--stop-after-pass callDestructors # all.good");
  }
  if errorFiles {
    const prediff = "PREDIFF";
    var w = openWriter(prediff);
    w.writeln("#!/bin/sh");
    w.writeln("if [ -s $2 ]; then");
    w.writeln("echo 'Compiler correctly threw an error' >$2");
    w.writeln("fi");
    w.close();
    OS.chmod(prediff.c_str(), 0o755:OS.mode_t);
  }
}



proc generate(allowed: set(2*string),
              allowedUpcast: set(2*string),
              allowedDowncast: set(2*string),
              writeEachTestCase) {

  generateDirectoryFiles(errorFiles = generateErrorCases);

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
    // only generate files when requested to
    if (isLegal && ! generateErrorCases) || (! isLegal && generateErrorCases)
      then generateFile(filename, chplLines);
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
  generate(allowed, allowedUpcast, allowedDowncast, writeEachTestCase);

}






proc generateCoerceInitAndAssign(doInit: bool = true) {

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

  var writeFunc = if doInit then writeEachTestCaseInit else writeEachTestCaseAssign;
  generate(allowed, allowedUpcast, allowedDowncast, writeFunc);

}

proc generateArgumentConst() {

  var allowed: set(2*string);
  // coerce owned to ...
  for x in ["owned", "borrowed", "borrowed?"] do
    allowed.add(("owned", x));

  // coerce shared to ...
  for x in ["shared", "borrowed", "borrowed?"] do
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

  var allowedUpcast: set(2*string);

  // coerce owned to ...
  for x in ["borrowed", "borrowed?"] do
    allowedUpcast.add(("owned", x));
  allowedUpcast.add(("owned?", "borrowed?"));
  // coerce shared to ...
  for x in ["borrowed", "borrowed?"] do
    allowedUpcast.add(("shared", x));
  allowedUpcast.add(("shared?", "borrowed?"));

  // coerce unmanaged to ...
  for x in ["unmanaged", "borrowed", "unmanaged?", "borrowed?"] do
    allowedUpcast.add(("unmanaged", x));
  // coerce borrowed to ...
  for x in ["borrowed", "borrowed?"] do
    allowedUpcast.add(("borrowed", x));
  // coerce unmanaged? to ...
  for x in ["unmanaged?", "borrowed?"] do
    allowedUpcast.add(("unmanaged?", x));
  // coerce borrowed? to ...
  for x in ["borrowed?"] do
    allowedUpcast.add(("borrowed?", x));

  // downcast is not supported
  var allowedDowncast: set(2*string);


  proc writeEachTestCase(
                     isLegal: bool,
                     fromType: string,
                     allocFromType: string,
                     toType: string,
                     allocToType: string): list(string) {
    var chplLines: list(string);
    chplLines.append("// coercing from %s to %s".format(fromType, toType));
    chplLines.append("proc bar(const x: %s) {}".format(toType));
    chplLines.append("proc foo() {");
    if allocFromType != fromType {
      chplLines.append("  var alloc = new %s();".format(allocFromType));
      chplLines.append("  var a:%s = alloc;".format(fromType));
    }
    else {
      chplLines.append("  var a = new %s();".format(fromType));
    }
    chplLines.append("  bar(a);");
    chplLines.append("}");
    return chplLines;
  }

  generate(allowed, allowedUpcast, allowedDowncast, writeEachTestCase);

}


proc generateArgumentIn(doConst: bool = false) {

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

  proc writeEachTestCaseHelper(
                     intent: string,
                     isLegal: bool,
                     fromType: string,
                     allocFromType: string,
                     toType: string,
                     allocToType: string): list(string) {
      var chplLines: list(string);
      chplLines.append("// coercing from %s to %s".format(fromType, toType));
      chplLines.append("proc bar(%s x: %s) {}".format(intent, toType));
      chplLines.append("proc foo() {");
      if allocFromType != fromType {
        chplLines.append("  var alloc = new %s();".format(allocFromType));
        chplLines.append("  var a:%s = alloc;".format(fromType));
      }
      else {
        chplLines.append("  var a = new %s();".format(fromType));
      }
      chplLines.append("  bar(a);");
      chplLines.append("}");
      return chplLines;
  }


  proc writeEachTestCaseIn(
                     isLegal: bool,
                     fromType: string,
                     allocFromType: string,
                     toType: string,
                     allocToType: string): list(string)
    do return writeEachTestCaseHelper("in", isLegal, fromType, allocFromType, toType, allocToType);
  proc writeEachTestCaseConstIn(
                     isLegal: bool,
                     fromType: string,
                     allocFromType: string,
                     toType: string,
                     allocToType: string): list(string)
    do return writeEachTestCaseHelper("const in", isLegal, fromType, allocFromType, toType, allocToType);

  var writeFunc = if doConst then writeEachTestCaseConstIn else writeEachTestCaseIn;
  generate(allowed, allowedUpcast, allowedDowncast, writeFunc);

}

proc generateArgumentRef(doConst: bool = false) {

  var allowed: set(2*string);
  // no change of managment allowed
  for x in managmentTypesAll do allowed.add((x, x));

  // upcast and downcast are not supported
  var allowedUpcast: set(2*string);
  var allowedDowncast: set(2*string);

  proc writeEachTestCaseHelper(
                     intent: string,
                     isLegal: bool,
                     fromType: string,
                     allocFromType: string,
                     toType: string,
                     allocToType: string): list(string) {
      var chplLines: list(string);
      chplLines.append("// coercing from %s to %s".format(fromType, toType));
      chplLines.append("proc bar(%s x: %s) {}".format(intent, toType));
      chplLines.append("proc foo() {");
      if allocFromType != fromType {
        chplLines.append("  var alloc = new %s();".format(allocFromType));
        chplLines.append("  var a:%s = alloc;".format(fromType));
      }
      else {
        chplLines.append("  var a = new %s();".format(fromType));
      }
      chplLines.append("  bar(a);");
      chplLines.append("}");
      return chplLines;
  }


  proc writeEachTestCaseRef(
                     isLegal: bool,
                     fromType: string,
                     allocFromType: string,
                     toType: string,
                     allocToType: string): list(string)
    do return writeEachTestCaseHelper("ref", isLegal, fromType, allocFromType, toType, allocToType);
  proc writeEachTestCaseConstRef(
                     isLegal: bool,
                     fromType: string,
                     allocFromType: string,
                     toType: string,
                     allocToType: string): list(string)
    do return writeEachTestCaseHelper("const ref", isLegal, fromType, allocFromType, toType, allocToType);

  var writeFunc = if doConst then writeEachTestCaseConstRef else writeEachTestCaseRef;
  generate(allowed, allowedUpcast, allowedDowncast, writeFunc);

}


proc generateArgumentOut() {

  var allowed: set(2*string);
  allowed.add(("owned", "owned"));
  allowed.add(("owned?", "owned"));
  allowed.add(("owned?", "owned?"));
  allowed.add(("shared", "shared"));
  allowed.add(("shared?", "shared"));
  allowed.add(("shared?", "shared?"));
  allowed.add(("unmanaged", "unmanaged"));
  allowed.add(("unmanaged?", "unmanaged"));
  allowed.add(("unmanaged?", "unmanaged?"));
  allowed.add(("borrowed", "borrowed"));
  allowed.add(("borrowed?", "borrowed"));
  allowed.add(("borrowed?", "borrowed?"));

  allowed.add(("borrowed", "owned"));
  allowed.add(("borrowed", "shared"));
  allowed.add(("borrowed", "unmanaged"));

  allowed.add(("borrowed?", "owned"));
  allowed.add(("borrowed?", "owned?"));
  allowed.add(("borrowed?", "shared"));
  allowed.add(("borrowed?", "shared?"));
  allowed.add(("borrowed?", "unmanaged"));
  allowed.add(("borrowed?", "unmanaged?"));

  // no "upcast", although for out it is backward
  var allowedUpcast: set(2*string);

  // same as allowed
  ref allowedDowncast = allowed;


  proc writeEachTestCase(
                     isLegal: bool,
                     fromType: string,
                     allocFromType: string,
                     toType: string,
                     allocToType: string): list(string) {
    var chplLines: list(string);
    chplLines.append("// coercing from %s to %s".format(fromType, toType));

    // doing a global alloc to avoid lifetime issues
    if allocToType != toType {
      chplLines.append("var globalAlloc = new %s();".format(allocToType));
    }
    chplLines.append("proc bar(out x: %s) {".format(toType));
    if allocToType != toType {
      chplLines.append("  x = globalAlloc;");
    }
    else {
      chplLines.append("  x = new %s();".format(toType));
    }
    chplLines.append("}");

    chplLines.append("proc foo() {");
    // alloc bere is just a dummy for the non nilable case
    if allocFromType != fromType {
      chplLines.append("  var alloc = new %s();".format(allocFromType));
      chplLines.append("  var a:%s = alloc;".format(fromType));
    }
    else {
      chplLines.append("  var a = new %s();".format(fromType));
    }
    chplLines.append("  bar(a);");
    chplLines.append("}");
    return chplLines;
  }

  generate(allowed, allowedUpcast, allowedDowncast, writeEachTestCase);

}

proc generateArgumentInout() {

  var allowed: set(2*string);
  allowed.add(("owned?", "owned?"));
  allowed.add(("shared", "shared"));
  allowed.add(("shared?", "shared?"));
  allowed.add(("unmanaged?", "unmanaged?"));
  allowed.add(("unmanaged", "unmanaged"));
  allowed.add(("borrowed", "borrowed"));
  allowed.add(("borrowed?", "borrowed?"));
  // upcast and downcast are not supported
  var allowedUpcast: set(2*string);
  var allowedDowncast: set(2*string);


  proc writeEachTestCase(
                     isLegal: bool,
                     fromType: string,
                     allocFromType: string,
                     toType: string,
                     allocToType: string): list(string) {
    var chplLines: list(string);
    chplLines.append("// coercing from %s to %s".format(fromType, toType));

    // doing a global alloc to avoid lifetime issues
    if allocToType != toType {
      chplLines.append("var globalAlloc = new %s();".format(allocToType));
    }
    chplLines.append("proc bar(inout x: %s) {".format(toType));
    if allocToType != toType {
      chplLines.append("  x = globalAlloc;");
    }
    else {
      chplLines.append("  x = new %s();".format(toType));
    }
    chplLines.append("}");

    chplLines.append("proc foo() {");
    if allocFromType != fromType {
      chplLines.append("  var alloc = new %s();".format(allocFromType));
      chplLines.append("  var a:%s = alloc;".format(fromType));
    }
    else {
      chplLines.append("  var a = new %s();".format(fromType));
    }
    chplLines.append("  bar(a);");
    chplLines.append("}");
    return chplLines;
  }

  generate(allowed, allowedUpcast, allowedDowncast, writeEachTestCase);

}


proc main() {

  select(generateSuite) {
    when "explicitCasts" do generateExplicitCasts();
    when "coerceInit" do generateCoerceInitAndAssign(doInit=true);
    when "coerceAssign" do generateCoerceInitAndAssign(doInit=false);
    when "argumentConst" do generateArgumentConst();
    when "argumentConstIn" do generateArgumentIn(doConst=true);
    when "argumentIn" do generateArgumentIn(doConst=false);
    when "argumentConstRef" do generateArgumentRef(doConst=true);
    when "argumentRef" do generateArgumentRef(doConst=false);
    when "argumentOut" do generateArgumentOut();
    when "argumentInout" do generateArgumentInout();
    otherwise do writeln("Unknown suite: ", generateSuite);
  }
}
