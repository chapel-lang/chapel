#!/usr/bin/env python3
#
# Utility routines to help with reading files
#
def IsCommentOrBlank(line):
    l=line.strip()
    if l == '':
        return True
    if l[0] == '#':
        return True
    return False

# Ignore blank lines and commented line
# Strip trailing white space and new line
def ReadNextNonCommentLine(f):
    l = f.readline()
    if l == '':
        return None
    while IsCommentOrBlank(l):
        l = f.readline()
        if l == '':
            return None
    return l.rstrip()

def ReadFileWithComments(filename):
    try:
        f= open(filename, 'r')
    except IOError:
        return None

    lines = list()
    l = ReadNextNonCommentLine(f)
    while l != None:
        lines.append(l)
        l = ReadNextNonCommentLine(f)

    f.close()
    return lines
