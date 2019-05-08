// Test unordered copy for various src/dst remote/local combinations (both
// known at compiler time and some only known at runtime.) This is meant to
// test codegen and that the runtime getput function works as expected.

use UnorderedCopy;

// TODO test with different types (and sizes?)
config param unordered=true;

inline proc iniDstValue() { return 0; }
inline proc iniSrcValue() { return 1; }

proc printThem(ref dst, ref src) {
  if unordered then unorderedCopyTaskFence();
  write("dst=" + dst + ", src=" + src + " -- ");
}

inline proc assign(ref dst, ref src) {
  if unordered then
    unorderedCopy(dst, src);
  else
    dst = src;
}

inline proc getLocaleID(curLocale: bool) {
  return if curLocale then chpl_nodeID else chpl_nodeID+1 % numLocales;
}

proc bothLocalAssign(param compileTimeKnown: bool) {
  var dst = iniDstValue(), src = iniSrcValue();
  if compileTimeKnown {
    assign(dst, src);
  } else {
    on Locales[getLocaleID(curLocale=true)] do assign(dst, src);
  }
  printThem(dst, src);
  writeln("bothLocalAssign(compileTimeKnown="+compileTimeKnown+")");
}

proc dstLocalAssign(param compileTimeKnown: bool, srcLocal: bool) {
  var src = iniSrcValue();
  if compileTimeKnown {
    on Locales[getLocaleID(curLocale=srcLocal)] {
      var dst = iniDstValue();
      assign(dst, src);
      printThem(dst, src);
    }
  } else {
    on Locales[getLocaleID(curLocale=srcLocal)] {
      var dst = iniDstValue();
      on Locales[getLocaleID(curLocale=true)] do assign(dst, src);
      printThem(dst, src);
    }
  }
  writeln("dstLocalAssign(compileTimeKnown="+compileTimeKnown+", srcLocal="+srcLocal+")");
}

proc srcLocalAssign(param compileTimeKnown: bool, dstLocal: bool) {
  var dst = iniDstValue();
  if compileTimeKnown {
    on Locales[getLocaleID(curLocale=dstLocal)] {
      var src = iniSrcValue();
      assign(dst, src);
      printThem(dst, src);
    }
  } else {
    on Locales[getLocaleID(curLocale=dstLocal)] {
      var src = iniSrcValue();
      on Locales[getLocaleID(curLocale=true)] do assign(dst, src);
      printThem(dst, src);
    }
  }
  writeln("srcLocalAssign(compileTimeKnown="+compileTimeKnown+", dstLocal="+dstLocal+")");
}

proc neitherLocalAssign() {
  var dst = iniDstValue(), src = iniSrcValue();
  on Locales[getLocaleID(curLocale=false)] {
    assign(dst, src);
  }
  printThem(dst, src);
  writeln("neitherLocalAssign()");
}


// local copy
for param compileTimeKnown in false..true do
  bothLocalAssign(compileTimeKnown);

// GET
for param compileTimeKnown in false..true do
  for srcLocal in false..true do
    dstLocalAssign(compileTimeKnown, srcLocal);

// PUT
for param compileTimeKnown in false..true do
  for dstLocal in false..true do
    srcLocalAssign(compileTimeKnown, dstLocal);

// GET PUT
neitherLocalAssign();
