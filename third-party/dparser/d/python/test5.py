import dparser

def d_h(t):
    'h : h1 | h2'     # uh oh, h1 and h2 are both matched by the letter a
    return t[0]

def d_h1(t, spec_only):  # h1 will be called for speculative parses only
    "h1 : 'a'"
    return 1         # This parse will be accepted since dparser.Reject is not returned.  Its return value for the final parse will also be 1.

def d_h2(t, spec):
    "h2 : 'a'"
    if spec:  
        return dparser.Reject     # don't let h2 match.  If this were not here, a dparser.AmbiguityException exception would result
    return 2

parser = dparser.Parser()
if parser.parse('a') != 1:
    print 'error'
