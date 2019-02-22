# Python (3.7.2) code with proper output for os.path.normpath(). Use this to
# generate the output for "testcases_normPath.good".


import os.path


collapsedA = 'foo/bar'
collapseToA0 = 'foo//bar'
collapseToA1 = 'foo/bar/'
collapseToA2 = 'foo/./bar'
collapseToA3 = 'foo/baz/../bar'

multiCurDir = '././.'
multiParentDir = '../../..'
multiMixedParentCurDir = './.././.././..'
multiOnlyPathSep = '/////'
curDir = '.'
parentDir = '..'
pathSep = '/'
pathSep2 = '//'


def collapse(path):
    print(path, 'becomes', os.path.normpath(path))


def main():
    # No change when given a valid path.
    collapse(collapsedA)
    # Collapse redundant separators.
    collapse(collapseToA0)
    # Eliminate trailing path separators.
    collapse(collapseToA1)
    # Eliminate current directory symbols.
    collapse(collapseToA2)
    # Properly collapse up-level references.
    collapse(collapseToA3)
    # Empty string yields single current dir.
    collapse('')
    # Single current dir yields single current dir.
    collapse(curDir)
    # Single parent dir yields single parent dir.
    collapse(parentDir)
    # Only current dirs yields single current dir.
    collapse(multiCurDir)
    # No change when given only parent dirs.
    collapse(multiParentDir)
    # Mix of current/parent dirs yields only parent dirs.
    collapse(multiMixedParentCurDir)
    # More than two path separators yields single path separator.
    collapse(multiOnlyPathSep)
    # No change when given single path separator.
    collapse(pathSep)
    # No change when given two path separators.
    collapse(pathSep2)
    # Path separators followed by parent dirs yields one path separator.
    collapse(multiOnlyPathSep + multiParentDir)
    # Path separators followed by current dirs yields one path separator.
    collapse(multiOnlyPathSep + multiCurDir)


if __name__ == '__main__':
    main()
