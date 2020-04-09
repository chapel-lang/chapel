// Test unordered copy for various src/dst remote/local combinations (both
// known at compile time and some only known at runtime.) This is meant to
// test codegen and that the runtime getput function works as expected.

use UnorderedCopy;

config param unordered=true;
record R { var a, b: int; }
type TupInt = 2*int, RecInt = R;
config type copyType = int;

inline proc iniDstValue() param where isPrimitive(copyType) { return 0:copyType; }
inline proc iniSrcValue() param where isPrimitive(copyType) { return 1:copyType; }

inline proc iniDstValue() where copyType == TupInt { return (0, 0); }
inline proc iniSrcValue() where copyType == TupInt { return (1, 1); }

inline proc iniDstValue() where copyType == RecInt { return new R(0, 0); }
inline proc iniSrcValue() where copyType == RecInt { return new R(1, 1); }

config const printResults = false;
proc printThem(ref dst, const ref src) {
  if unordered then unorderedCopyTaskFence();
  assert(src == dst && src == iniSrcValue());
  if printResults then
    write("dst=", dst, ", src=", src, " -- ");
}

proc printThem(ref dst, param src) {
  if unordered then unorderedCopyTaskFence();
  assert(src == dst && src == iniSrcValue());
  if printResults then
    write("dst=", dst, ", src=", src, " -- ");
}

inline proc assign(ref dst, const ref src) {
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
  writeln("bothLocalAssign(compileTimeKnown=", compileTimeKnown, ")");
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
  writeln("dstLocalAssign(compileTimeKnown=", compileTimeKnown, ", srcLocal=", srcLocal, ")");
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
  writeln("srcLocalAssign(compileTimeKnown=", compileTimeKnown, ", dstLocal=", dstLocal, ")");
}

proc srcLocalConstAssign(param compileTimeKnown: bool, dstLocal: bool) {
  var dst = iniDstValue();
  if compileTimeKnown {
    on Locales[getLocaleID(curLocale=dstLocal)] {
      const src = iniSrcValue();
      assign(dst, src);
      printThem(dst, src);
    }
  } else {
    on Locales[getLocaleID(curLocale=dstLocal)] {
      const src = iniSrcValue();
      on Locales[getLocaleID(curLocale=true)] do assign(dst, src);
      printThem(dst, src);
    }
  }
  writeln("srcLocalConstAssign(compileTimeKnown=", compileTimeKnown, ", dstLocal=", dstLocal, ")");
}

proc srcParamAssign(dstLocal: bool) {
  var dst = iniDstValue();
  if isParam(iniSrcValue()) {
    on Locales[getLocaleID(curLocale=dstLocal)] {
      param src = iniSrcValue();
      if unordered then unorderedCopy(dst, src);
                   else dst = src;
      printThem(dst, src);
    }
  }
  writeln("srcParamAssign(dstLocal=", dstLocal, ")");
}

proc neitherLocalAssign() {
  var dst = iniDstValue(), src = iniSrcValue();
  on Locales[getLocaleID(curLocale=false)] {
    assign(dst, src);
    printThem(dst, src);
  }
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

// PUT where src is const
for param compileTimeKnown in false..true do
  for dstLocal in false..true do
    srcLocalConstAssign(compileTimeKnown, dstLocal);

// PUT where src is param
for dstLocal in false..true do
  srcParamAssign(dstLocal);

// GET PUT
neitherLocalAssign();
