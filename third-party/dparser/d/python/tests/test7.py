# the 'nodes' argument to actions is useful for obtaining line number information and such

from dparser import Parser

def d_start(t, nodes, this):      # in this example, nodes[0] is the D_ParseNode for 'noun'
    'start : noun verb'
    
    # the 'buf' member contains the entire string that is being parsed
    buf = nodes[0].buf
    noun = nodes[0]
    
    # the 'end', 'end_skip', and 'start_loc.s' members of nodes[0] are indices into relevant parts of buf:
    buf[noun.start_loc.s:noun.end]  # 'cat'
    buf[noun.start_loc.s:]          # 'cat flies'
    buf[noun.end:]                  # ' flies'
    buf[noun.end_skip:]             # 'flies'

    # line numbers and columns:
    noun.start_loc.line
    noun.start_loc.col
    
    # the 'this' argument is the D_ParseNode for this action:
    buf[this.start_loc.s:this.end] # 'cat flies'

    # children of a node can also be obtained with the 'c' member:
    # this.c[0] is the same as nodes[0]

def d_noun(t, this):
    "noun : 'cat'"
    return t[0]

def d_verb(t, this):
    "verb : 'flies'"
    return t[0]

Parser().parse('cat flies')
