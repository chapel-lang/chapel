from dparser import Parser

def d_s(t):
    "s : a"

def d_a(t, spec):
    "a ::= 'a'"

parser = Parser()

parser.parse('a')
