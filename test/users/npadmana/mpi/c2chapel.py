""" A C to Chapel parser to help automate writing bindings.

This is built on top of pycparser :
  https://github.com/eliben/pycparser
(pycparser is also part of the Anaconda distribution)

Some parts of this code may need to be special cased for particular bindings,
and the resulting bindings may require some human intervention.
"""
from pycparser import c_parser, c_ast, parse_file
import sys

## Global dictionary of C->Chapel mappings. New typedefs can
## be registered here.
## TODO : Currently incomplete
c2chapel = {}
c2chapel['double'] = 'c_double'
c2chapel['float'] = 'c_float'
c2chapel['int'] = 'c_int'
c2chapel['long'] = 'c_long'
c2chapel['char'] = 'c_char'
c2chapel['void'] = None

# Convert a type declaration into Chapel
# returns a " : " + declaration
# If varname is passed in, use that (easy to build function prototypes)
def typeDecl2Chapel(decl, varname=None, isArray=False, isPtr=False):
    if not isinstance(decl, c_ast.TypeDecl):
        decl.show()
        raise c_parser.ParseError("Unable to parse type declaration at %s"%decl.coord)
    tmp = decl.type;
    if varname is None :
        varname = decl.declname;
    if not isinstance(tmp, c_ast.IdentifierType):
        decl.show()
        raise c_parser.ParseError("Unable to parse type declaration at %s"%decl.coord)
    name = tmp.names[0]
    if name.startswith("MPI_") :
        chapeltype=name
    else :
        if name not in c2chapel.keys() :
            raise c_parser.ParseError("Unknown type % at %s"%(name,decl.coord))
        chapeltype = c2chapel[name]
    if isArray:
        chapeltype = "[]"+chapeltype
    if chapeltype is not None :
        varname += ": "+chapeltype
    if isPtr :
        varname = "ref "+varname
    return varname

def params2Chapel(p):
    if isinstance(p.type, c_ast.ArrayDecl):
        return typeDecl2Chapel(p.type.type,isArray=True)
    elif isinstance(p.type, c_ast.PtrDecl):
        return typeDecl2Chapel(p.type.type,isPtr=True)
    # TODO : need to careful about pointer and arrays
    return typeDecl2Chapel(p.type)


# Convert function declarations into Chapel
def funcDecl2Chapel(decl):
    fname = decl.type.declname
    # Build prototype
    ss = "extern proc %s (%%s)"%fname
    ss = typeDecl2Chapel(decl.type, varname=ss)
    if decl.args is not None :
        parlist = [params2Chapel(p1) for p1 in decl.args.params]
        ss = ss%(", ".join(parlist))
    else :
        ss = ss%("",)
    return ss


# Simple visitor to all function declarations
class FuncDeclVisitor(c_ast.NodeVisitor):
    def visit_FuncDecl(self, node):
        print funcDecl2Chapel(node) + ";"

if __name__=="__main__":
    ast = parse_file(sys.argv[1], use_cpp=True)
    v = FuncDeclVisitor()
    v.visit(ast)
