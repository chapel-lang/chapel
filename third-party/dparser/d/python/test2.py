from dparser import Parser

def d_add(t):
    '''add : add '+' mul
           | mul'''
    if(len(t) == 1):
        return t[0]
    return t[0] + t[2]

def d_mul(t):
    '''mul : mul '*' exp
           | exp'''
    if(len(t) == 1):
        return t[0]
    return t[0]*t[2]

def d_exp(t):
    '''exp : number1
           | number2
           | '(' add ')' '''
    if(len(t) == 1):
        return int(t[0])
    return t[1]

def d_number1(t):
    '''number1 : number'''
    return t[0]

def d_number2(t):
    '''number2 : number'''
    return t[0]

def d_number(t):
    '''number : "[0-9]+"'''
    return t[0]

def ambiguity_func(v):
    return v[0]

def d_whitespace(t, spec):
    "whitespace : ' '*"
    
if Parser().parse('1  +2* (3+ 4+5)', ambiguity_fn = ambiguity_func) != 25:
    print 'fail'
