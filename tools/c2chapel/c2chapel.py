#!/usr/bin/env python

#
# Copyright 2017 Cray Inc.
# Other additional copyright holders may be indicated within.
#
# The entirety of this work is licensed under the Apache License,
# Version 2.0 (the "License"); you may not use this file except
# in compliance with the License.
#
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

from __future__ import print_function

""" A C to Chapel parser to automatically generate extern bindings.

This tool is built on top of pycparser :
  https://github.com/eliben/pycparser

See README.rst for more information.
"""

__version__ = "0.1.0"

# TODO: look at .type.type and make sure we're not relying on a specific
# AST depth (e.g., watch out for pointers-to-pointers-to-pointers-to-...
#
# TODO: enums?

import sys

try:
    from pycparser import c_parser, c_ast, parse_file
except ImportError as e:
    sys.exit("Unable to import pycparser: " + str(e));

import argparse
import os.path
try:
    from StringIO import StringIO
except ImportError as e:
    from io import StringIO
import re

noComments = False
DEBUG      = False

## Global dictionary of C->Chapel mappings. New types can
## be registered here.
c2chapel = {}

c2chapel["double"] = "c_double"
c2chapel["float"]  = "c_float"
c2chapel["char"]   = "c_char"
c2chapel["void"]   = ""

# Based on SysCTypes.chpl
c2chapel["int"]                = "c_int"
c2chapel["unsigned int"]       = "c_uint"
c2chapel["long"]               = "c_long"
c2chapel["unsigned long"]      = "c_ulong"
c2chapel["long long"]          = "c_longlong"
c2chapel["unsigned long long"] = "c_ulonglong"
c2chapel["char"]               = "c_char"
c2chapel["signed char"]        = "c_schar"
c2chapel["unsigned char"]      = "c_uchar"
c2chapel["short"]              = "c_short"
c2chapel["unsigned short"]     = "c_ushort"
c2chapel["intptr_t"]           = "c_intptr"
c2chapel["uintptr_t"]          = "c_uintptr"
c2chapel["ptrdiff_t"]          = "c_ptrdiff"
c2chapel["ssize_t"]            = "ssize_t"
c2chapel["size_t"]             = "size_t"

__temp = [k for k in c2chapel.keys()]
for key in __temp:
    if key.endswith("long"):
        c2chapel[key + " int"] = c2chapel[key]

for i in range(3, 7):
    width = 2**i
    s  = "int" + str(width) + "_t"
    ch = "int(" + str(width) + ")"

    c2chapel[s] = ch
    c2chapel["u" + s] = "u" + ch

# Map of strings to c_ast.TypeDef
typeDefs = {}

# set of types c2chapel is aware of
foundTypes = set()

def getArgs():
    parser = argparse.ArgumentParser(description="Generate C bindings for Chapel", prog="c2chapel")
    parser.add_argument("file", help="C99 file for which to generate bindings")
    parser.add_argument("--no-typedefs",
                        help="do not generate extern types for C typedefs",
                        action="store_true")
    parser.add_argument("--debug",
                        help="enable debugging output",
                        action="store_true")
    parser.add_argument("--no-fake-headers",
                        help="do not use fake headers included with c2chapel",
                        action="store_true")
    parser.add_argument("--no-comments",
                        help="instruct c2chapel to not generate comments",
                        action="store_true")
    parser.add_argument("-V", "--version", action="version", version="%(prog)s " + __version__)
    parser.add_argument("cppFlags", nargs="*", help="flags forwarded to the C preprocessor (invoked with cc -E)")
    return parser.parse_known_args()

# s - string to print out.
# each line will be prepended with '// '
def genComment(s):
    if noComments:
        return
    for line in s.split("\n"):
        print("// " + line)

def commentNode(node):
    buf = StringIO()
    node.show(buf=buf)
    genComment(buf.getvalue())

def getDeclName(decl):
    inner = decl
    if type(inner) == c_ast.TypeDecl:
        inner = inner.type

    name = ""
    if type(inner) == c_ast.IdentifierType:
        name = " ".join(inner.names)
    elif type(inner) == c_ast.Struct:
        name = inner.name
    elif type(inner) == c_ast.Decl:
        name = inner.name
    else:
        raise Exception("Unhandled node type: " + str(type(inner)))
    return name

def computeArgName(decl):
    if type(decl) == c_ast.Typename:
        return ""
    elif type(decl) == c_ast.Decl:
        return decl.name
    else:
        decl.show()
        raise c_parser.ParseError("Unhandled Node type")

def getIntentInfo(ty):
    refIntent = ""
    retType   = ""
    curType   = ty

    if type(curType) == c_ast.PtrDecl and not (isPointerTo(curType, "char") or isPointerTo(curType, "void")):
        refIntent = "ref"
        curType = curType.type
    else:
        refIntent = ""

    retType = toChapelType(curType)

    return (refIntent, retType)

# pl - a c_ast.ParamList
def computeArgs(pl):
    formals = []
    if pl is None:
        return ""

    for (i, arg) in enumerate(pl.params):
        if type(arg) == c_ast.EllipsisParam:
            formals.append("c__varargs ...")
        else:
            (intent, typeName) = getIntentInfo(arg.type)
            argName = computeArgName(arg)
            if typeName != "":
                if intent != "":
                    intent += " "
                if argName == "":
                    argName = "arg" + str(i)
                formals.append(intent + argName + " : " + typeName)
    return ", ".join(formals)

def isPointerTo(ty, text):
    if type(ty) == c_ast.PtrDecl:
        if type(ty.type) == c_ast.TypeDecl:
            name = getDeclName(ty.type)
            if name == text:
                return True
    return False

def toChapelType(ty):
    if isPointerTo(ty, "char"):
        return "c_string"
    elif isPointerTo(ty, "void"):
        return "c_void_ptr"
    elif type(ty) == c_ast.ArrayDecl:
        return "c_ptr(" + toChapelType(ty.type) + ")"
    elif type(ty) == c_ast.PtrDecl:
        if type(ty.type) == c_ast.FuncDecl:
            return "c_fn_ptr"
        else:
            return "c_ptr(" + toChapelType(ty.type) + ")"
    elif type(ty) == c_ast.TypeDecl:
        inner = ty.type
        name = ""
        try:
            name = getDeclName(ty)
        except Exception as e:
            raise Exception("toChapelType: " + str(e))

        if name in c2chapel:
            return c2chapel[name]

        return name
    elif type(ty) == c_ast.FuncDecl:
        return "c_fn_ptr"
    else:
        ty.show()
        raise c_parser.ParseError("Unable to translate to Chapel type")


def getFunctionName(ty):
    if type(ty) == c_ast.PtrDecl:
        return getFunctionName(ty.type)
    else:
        if type(ty) != c_ast.TypeDecl:
            ty.show()
            raise c_parser.ParseError("Expecting TypeDecl...")
        return ty.declname


# Convert function declarations into Chapel
def genFuncDecl(fn):
    retType = toChapelType(fn.type)
    fnName  = getFunctionName(fn.type)
    args    = computeArgs(fn.args)

    if retType == "":
        retType = "void"

    return "extern proc " + fnName + "(" + args + ") : " + retType

def isStructDef(decl):
    if type(decl) == c_ast.Struct:
        if decl.decls is not None:
            return decl
        else:
            return None
    elif type(decl) == c_ast.Decl or type(decl) == c_ast.TypeDecl or type(decl) == c_ast.PtrDecl:
        return isStructDef(decl.type)
    else:
        return None


def genStruct(struct, name=""):
    if name == "":
        name = struct.name
    ret = "extern record " + name + " {"
    foundTypes.add(name)

    members = ""
    for decl in struct.decls:
        innerStruct = isStructDef(decl)
        if innerStruct is not None:
            genStruct(innerStruct)

        members += "  var " + getDeclName(decl) + " : " + toChapelType(decl.type) + ";\n"

    if members != "":
        members = "\n" + members
    ret += members + "}\n"
    print(ret)

def genVar(decl):
    name = decl.name
    ty   = toChapelType(decl.type)
    print("extern var " + name + " : " + ty + ";")
    print()

# Simple visitor to all function declarations
class ChapelVisitor(c_ast.NodeVisitor):
    def visit_Struct(self, node):
        typeDefs[node.name] = None
        genStruct(node)

    def visit_Typedef(self, node):
        if node.name not in typeDefs:
            typeDefs[node.name] = node

    def visit_FuncDecl(self, node):
        print(genFuncDecl(node) + ";")
        print()

    def visit_FuncDef(self, node):
        if DEBUG:
            genComment("[debug]: Skipping FuncDef body")
            commentNode(node)
        self.visit_Decl(node.decl)

    def visit_Decl(self, node):
        if DEBUG:
            genComment("Evaluating node:")
            commentNode(node)
        for c_name, c in node.children():
            if type(c) == c_ast.Struct:
                self.visit_Struct(c)
            elif type(c) == c_ast.FuncDecl:
                self.visit_FuncDecl(c)
            elif type(c) == c_ast.TypeDecl or type(c) == c_ast.PtrDecl or type(c) == c_ast.ArrayDecl:
                genVar(node)
            else:
                node.show()
                raise Exception("Unhandled declaration")


def genTypeAlias(node):
    alias = node.name
    if type(node.type) == c_ast.PtrDecl or type(node.type) == c_ast.TypeDecl:
        typeName = toChapelType(node.type)
        foundTypes.add(alias);
        print("extern type " + alias + " = " + typeName + ";")
        print()

def genTypedefs(defs):
    for name in sorted(defs):
        node = defs[name]
        if node is not None:
            if type(node.type.type) == c_ast.Struct:
                if node.type.type.decls is not None:
                    genStruct(node.type.type, name=node.name)
                elif node.type.type.name not in foundTypes:
                    genComment("Opaque struct?")
                    print("extern record " + node.name + " {};")
                    print()
                else:
                    genTypeAlias(node)
            else:
              genTypeAlias(node)

def handleTypedefs(defs, ignores):
    ignoreDefs = {}
    for i in ignores:
        if i in defs:
            ignoreDefs[i] = defs[i]
            defs[i] = None

    numDefs = sum([1 for x in defs if defs[x] is not None])
    if numDefs != 0:
        genComment("==== c2chapel typedefs ====")
        print()
        genTypedefs(defs)

    if len(ignoreDefs) != 0:
        genComment("c2chapel thinks these typedefs are from the fake headers:")
        print("/*")
        genTypedefs(ignoreDefs)
        print("*/")

# TODO: ignore those defined within an ifdef
# There's probably some macro magic we can play with such that we only generate
# the macro name if it is defined.
#
# TODO: handle other kinds of literals:
# - strings
# - hex, octal ints
# - floats/doubles?
# - bitshift constants (e.g. 1<<3)
def emit_defines(fname):
    with open(fname, "r") as f:
        pat = re.compile("^\s*#define\s+([_a-zA-Z0-9]+)\s+[0-9]+$")
        first = True
        for line in f:
            res = pat.match(line)
            if res is not None:
                if first:
                    genComment("#define'd integer literals:")
                    genComment("Note: some of these may have been defined with an ifdef")
                    first = False

                print("extern const " + res.group(1) + " : int;")

        if not first:
            print()
            genComment("End of #define'd integer literals")
            print()

def getFakeHeaderPath():
    script = os.path.realpath(__file__)
    ret    = ""
    if os.path.isfile(script):
        parent = os.path.dirname(script)
        fakes = parent + "/install/utils/fake_libc_include/"
        if os.path.isdir(fakes):
            ret = fakes

    return ret

def findIgnores():
    path = getFakeHeaderPath()
    ret = set()
    if path:
        defs = path + "_fake_typedefs.h"
        if os.path.isfile(defs):
            with open(defs, "r") as fi:
                for line in fi:
                    if line.startswith("typedef"):
                        rhs = line.replace(";", "")
                        rhs = rhs.replace("typedef int ", "")
                        rhs = rhs.replace("typedef _Bool ", "")

                        ret.add(rhs.strip())

    return ret

# Return a list of strings to be passed to the C preprocessor
def getFakeHeaders():
    ret  = []
    path = getFakeHeaderPath()
    if path:
        ret = ["-I", path]
    return ret

def preamble(args, fakes):
    genComment("Generated with c2chapel version " + __version__)
    print()

    fname = args.file
    if fname.endswith(".h"):
        genComment("Header given to c2chapel:")
        print("require \"" + fname + "\";\n")

    if len(fakes) != 0:
        genComment("Note: Generated with fake std headers")
        print()

# TODO: accept file from stdin?
if __name__=="__main__":
    (args, unknowns)  = getArgs()
    fname      = args.file
    noComments = args.no_comments
    DEBUG      = args.debug

    if not os.path.isfile(fname):
        sys.exit("No such file: '" + fname + "'")

    fakes = []
    ignores = set()
    if args.no_fake_headers is not True:
        fakes = getFakeHeaders()
        ignores = findIgnores()

    try:
        ast = parse_file(fname, use_cpp=True, cpp_path="cc", cpp_args=["-E"] + fakes + unknowns)
    except c_parser.ParseError as e:
        sys.exit("Unable to parse file: " + str(e))

    preamble(args, fakes)

    emit_defines(fname)

    v = ChapelVisitor()
    v.visit(ast)

    if args.no_typedefs is False:
        handleTypedefs(typeDefs, ignores)

