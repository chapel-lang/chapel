#!/usr/bin/env python

import sys, os, shutil, subprocess, string, signal
import re

varList = [
  ("b0",  "bool"),
  ("b8",  "bool(8)"),
  ("b16", "bool(16)"),
  ("b32", "bool(32)"),
  ("b64", "bool(64)"),

  ("i8",  "int(8)"),
  ("i16", "int(16)"),
  ("i32", "int(32)"),
  ("i64", "int(64)"),

  ("u8",  "uint(8)"),
  ("u16", "uint(16)"),
  ("u32", "uint(32)"),
  ("u64", "uint(64)"),

  ("r32",  "real(32)"),
  ("r64",  "real(64)"),
  ("m32",  "imag(32)"),
  ("m64",  "imag(64)"),
  ("z64",  "complex(64)"),
  ("z128", "complex(128)"),

  ("str",    "string"),
  ("enm",    "EnumType"),
  ("homtup", "HomTupType"),
  ("hettup", "HetTupType"),

  ("cls",  "ClassType"),
  ("rec1", "RecordSmall"),
  ("unn",  "UnionType"),

  ("rng1", "range"),
  ("rng2", "range(uint(8), BoundedRangeType.boundedNone, true)"),

  ("dmp",  "new dmap(new DefaultDist())"),
  ("dom1", "DomType1"),
  ("dom2", "DomType2"),
  ("arr1", "ArrType1"),
  ("arr2", "ArrType2"),

  ("syInt",  "sync int"),
  ("syReal", "sync real"),
  ("siInt",  "single int"),
  ("siReal", "single real"),
  ("aInt",   "atomic int"),
  ("aReal",  "atomic real"),
]

predList = [
  "isBool",
  "isInt",
  "isUint",
  "isReal",
  "isImag",
  "isComplex",
  "isString",
  "isEnum",
  "isTuple",
  "isClass",
  "isRecord",
  "isUnion",
  "isRange",
  "isDmap",
  "isDomain",
  "isArray",
  "isSync",
  "isSingle",
  "isAtomic",

  "isIntegral",
  "isFloat",
  "isNumeric",
  "isPrimitive",
]

def say(*args):
  sys.stdout.write("".join(args))
  sys.stdout.write("\n")

def compilerWarning(*args):
  say("compilerWarning(\"", "".join(args), "\");")


# declare test()

say("proc test(arg) {")
say("  compilerWarning(typeToString(arg.type));")

for pred in predList:
  say("  report(", pred,      "(arg), \"",      pred,      "\");")
  say("  report(", pred, "Value(arg), \"",      pred, "Value\");")
  say("  report(", pred,  "Type(arg.type), \"", pred,  "Type\");")

say("  if isTuple(arg) {");
say("    report(isHomogeneousTuple(arg),\"isHomogeneousTuple\");")
say("    report(isHomogeneousTupleValue(arg),\"isHomogeneousTupleValue\");")
say("    report(isHomogeneousTupleType(arg.type),\"isHomogeneousTupleType\");")
say("  }")

say("  compilerWarning(\"\");")
say("}")
say("")


# invoke test()

for (varname, vartype) in varList:
  say("test(", varname, ");")


# wrap up

say("")
say("compilerError(\"done\"); // no need to execute")
