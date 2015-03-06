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
]

groupList = [
  "isIntegral",
  "isFloat",
  "isNumeric",
  "isPrimitive",
]

def say(what):
  sys.stdout.write(what)


# predList: group by variable

def predShow(varname, pred):
  say("report(%s(%s), \"%s\");\n" % (pred, varname, pred))

for (varname, vartype) in varList:
  say("\n");
  say("write(\"%s (%s)\");\n" % (vartype, varname));

  for pred in predList:
    predShow(varname,           pred)
    predShow(varname,           pred + "Value")
    predShow(varname + ".type", pred + "Type")

  if vartype == "HomTupType" or vartype == "HetTupType":
    predShow(varname,           "isHomogeneousTuple")
    predShow(varname,           "isHomogeneousTupleValue")
    predShow(varname + ".type", "isHomogeneousTupleType")

  say("writeln(\"  .\");\n")


# separator

say("\n")
say("writeln();\n")


# groupList: group by predicate, here "group"

def groupShow(group, repArg, varname):
  say("report(%s(%s), \"%s\");\n" % (group, repArg, varname))

def doGroup(group, varX):
  say("\n")
  say("write(\"%s: \");\n" % group)

  for (varname, vartype) in varList:
    groupShow(group, varname + varX, varname)

  say("writeln(\"  .\");\n")

for group in groupList:
  doGroup(group,           "")
  doGroup(group + "Value", "")
  doGroup(group + "Type",  ".type")
