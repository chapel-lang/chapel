from dparser import Parser

def d_S(t):
    '''S : d '+' d'''
    return t[0] + t[2]

def d_number(t):
    '''d : "[0-9]+" '''
    return int(t[0])

def skip_space(loc):
    while loc.s < len(loc.buf) and loc.buf[loc.s:loc.s+len('hello')] == 'hello':
        loc.s = loc.s + len('hello')

parser = Parser(make_grammar_file=1)

buf = 'hi10hello+3hellohi'

if parser.parse(buf, buf_offset=2, partial_parses=1, initial_skip_space_fn = skip_space) != 13:
    print 'fail'

buf = '87+5'
if parser.parse(buf, initial_skip_space_fn = skip_space) != 92:
    print 'fail'
