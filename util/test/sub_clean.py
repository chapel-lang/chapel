#!/usr/bin/env python3
#
# Clean a directory or the files associated with a particular test
#

import os, sys, time, glob, shutil


# return True if f has .chpl or test.c extension
def getExecname(f):
    if (
        (f.count(".") != 0)
        and (len(f) >= 6)
        and (len(f) - f.find(".chpl") == 5)
    ):
        return f[: len(f) - 5]
    elif (
        (f.count(".") > 1)
        and (len(f) >= 8)
        and (len(f) - f.find(".test.c") == 7)
    ):
        return f[: len(f) - 7]
    elif (
        (f.count(".") > 1)
        and (len(f) >= 11)
        and (len(f) - f.find(".ml-test.c") == 10)
    ):
        return f[: len(f) - 10]
    return None


# read a cleanfiles file (with comments)
def ReadCleanfiles(f, ignoreLeadingSpace=True):
    myfile = open(f, "r")
    mylines = myfile.readlines()
    myfile.close()
    mylist = list()
    for line in mylines:
        line = line.rstrip()
        # ignore blank lines
        if not line.strip():
            continue
        # ignore comments
        if ignoreLeadingSpace:
            if line.lstrip()[0] == "#":
                continue
        else:
            if line[0] == "#":
                continue
        tlist = line.split()
        for t in tlist:
            if t[0] == "#":
                break
            mylist.append(t)
    return mylist


def unlink(f, removeDirs=False):
    try:
        if removeDirs and os.path.isdir(f) and not os.path.islink(f):
            shutil.rmtree(f)
        else:
            os.unlink(f)
    except:
        pass


# Clean the files associated with a Chapel test program
def _cleanChapelTest(f):
    execname = getExecname(f)
    lines = []
    if execname:
        lines.append("Cleaning test: " + execname + "\n")
        globfiles = glob.glob(execname + ".*.out.tmp")
        for g in globfiles:
            unlink(g)
        unlink(execname)
        unlink(execname + "_real")
        unlink(execname + ".dSYM", removeDirs=True)
        unlink(execname + "_real.dSYM", removeDirs=True)
        lines.extend(_cleanCleanfiles(None, execname + ".cleanfiles", False))
    return lines


# Clean files listed in the file c, in the directory d (if given)
def _cleanCleanfiles(d, c, rmCore):
    if d:
        cleanfile = os.path.join(d, c)
    else:
        cleanfile = c
    lines = []
    if os.access(cleanfile, os.R_OK):
        cleanfiles = ReadCleanfiles(cleanfile)
        if rmCore:
            cleanfiles += ["core"]
        for f in cleanfiles:
            if d:
                globfiles = glob.glob(os.path.join(d, f))
            else:
                globfiles = glob.glob(f)
            for g in globfiles:
                lines.append("Removing " + g + "\n")
                unlink(g, removeDirs=True)
    return lines


def clean(to_clean=["."]):
    #
    # Cleaning..
    #
    lines = [
        "[Starting sub_clean - %s]\n"
        % (time.strftime("%a %b %d %H:%M:%S %Z %Y", time.localtime())),
        "[pwd: " + os.getcwd() + "]\n",
    ]

    for f in to_clean:
        if os.path.isdir(f):
            lines.append("Cleaning directory: " + f + "\n")
            dirlist = glob.glob(f + "/*.chpl")
            dirlist += glob.glob(f + "/*.test.c")
            dirlist += glob.glob(f + "/*.ml-test.c")
            dirlist.sort()
            for file in dirlist:
                lines.extend(_cleanChapelTest(file))
            lines.extend(_cleanCleanfiles(f, "CLEANFILES", True))
        else:
            lines.extend(_cleanChapelTest(f))
            lines.extend(
                _cleanCleanfiles(os.path.dirname(f), "CLEANFILES", True)
            )

    return "".join(lines)


if __name__ == "__main__":
    if len(sys.argv) == 1:
        to_clean = ["."]
    else:
        to_clean = sys.argv[1:]
    sys.stdout.write(clean(to_clean))
