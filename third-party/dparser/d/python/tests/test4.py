import dparser

def d_S(t):
    "S: a | b"
    return 'S'

def d_a(t):
    "a : x1 x1 'y'"

def d_b(t):
    "b : x2 x2 'y'"

def d_x1(t, spec):
    "x1 : 'x'"
    
def d_x2(t, spec):
    "x2 : 'x'"
    if spec:
        return dparser.Reject

def syntax_error(t):
    print 'fail'
    
parser = dparser.Parser()
parser.parse('xxy', syntax_error_fn = syntax_error)
