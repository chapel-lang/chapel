### PYTHON FRAGMENT

# suggestions for code improvement are highly welcome ;-)

def syntax_error(loc):
    contextwidth_left = 30
    contextwidth_right = 30
    ellipsis = '[...]'
    locbuf_flattened = loc.buf.replace("\n","\254") # \254 is 'not sign'
    
    spanstart = loc.s - contextwidth_left
    mn = max(spanstart, 0)
    if spanstart > 0:
        before = ellipsis + locbuf_flattened[mn + len(ellipsis):loc.s]
    else:
        before = locbuf_flattened[mn:loc.s]
        space = ' '*(contextwidth_left - len(before))
        before = space + before

    spanend = loc.s + contextwidth_right
    mx = min(spanend, len(locbuf_flattened))
    after = locbuf_flattened[loc.s:mx]
    if len(after) > (contextwidth_right - len(ellipsis)):
        after = locbuf_flattened[loc.s:(mx - len(ellipsis))] + ellipsis

    print "-"*79
    print "File %s:\n" % inputfilename
    print before +\
          " ____________\n" +\
          " "*contextwidth_left +\
          "/syntax error at line " + str(loc.line) +\
          " (buffer pos. " + str(loc.s) + ")"
    print " "*contextwidth_left + after
    print "-"*79
