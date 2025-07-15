#!/usr/bin/env python3

""" Runs a command (or series of commands) on all the images.
    Prints a summary of the output at the end.
    Saves all of the command output in a file called 'log'. """

import argparse, glob, os, subprocess, shutil, sys

def gatherDirs(skip_nollvm):
    dirs = [ ]
    for d in glob.glob('current/*'):
        if os.path.exists(os.path.join(d, 'image.def')):
            if skip_nollvm and 'nollvm' in d:
                pass # skip -nollvm versions if requested
            else:
                dirs.append(d)
    dirs.sort()
    return dirs

def dirNameToConfigName(d):
    return os.path.basename(d.rstrip('/'))

def printAndLog(log, s):
    print(s)
    print(s, file=log)

# Returns a tuple consisting of (exit code, last line)
def runAndLog(log, command):
    printAndLog(log, "Running command: {}\n".format(" ".join(command)))
    try:
        with subprocess.Popen(command,
                              stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                              text=True) as p:
            for line in p.stdout:
                line = line.rstrip('\n')
                printAndLog(log, line)
                lastline = line
            p.wait()
            return (p.returncode, lastline)
    except OSError as e:
        return (-1, str(e))

def main():
    parser = argparse.ArgumentParser(
            description='Run a command on all the images.')
    parser.add_argument('--cleanup', dest='cleanup', action='store_true',
                        help='delete each image and checkout after using it')
    parser.add_argument('--rebuild', dest='rebuild', action='store_true',
                        help='rebuild each image before running it')
    parser.add_argument('--start', dest='start', action='store',
                        help='run configurations starting with the passed one')
    parser.add_argument('--skip-nollvm', dest='skip_nollvm', action='store_true',
                        help='skip nollvm configurations')
    parser.add_argument('command', nargs='+', help='command to run')

    args = parser.parse_args()

    logpath = 'log'

    startDir = os.getcwd()

    status = { }

    apptainer_image = os.environ.get('APPTAINER_IMAGE')
    if not apptainer_image:
        printAndLog(sys.stderr,
                    "APPTAINER_IMAGE environment variable not set.")
        sys.exit(1)
    elif apptainer_image == "all":
        printAndLog(sys.stderr, "Running on all images.")
        only = None
    else:
        printAndLog(sys.stderr, "Running on image: " + apptainer_image)
        only = apptainer_image

    with open(logpath, 'w', encoding="utf-8") as log:
        dirs = gatherDirs(args.skip_nollvm)
        if only != None:
            hadDirs = dirs
            dirs = [ ]
            for d in hadDirs:
                if dirNameToConfigName(d) == dirNameToConfigName(only):
                    dirs.append(d)
        if len(dirs) == 0:
            printAndLog(log, "No test configs to run found, aborting.")
            sys.exit(1)

        if args.start != None:
            hadDirs = dirs
            dirs = [ ]
            started = False
            for d in hadDirs:
                if dirNameToConfigName(d) == dirNameToConfigName(args.start):
                    started = True
                if started:
                    dirs.append(d)

        # compute the longest length for padding
        maxNameLen = 0
        for d in dirs:
            name = dirNameToConfigName(d)
            if len(name) > maxNameLen:
                maxNameLen = len(name)

        ok = True
        for d in dirs:
            name = dirNameToConfigName(d)

            os.chdir(d)
            printAndLog(log,
                        "     ---- " + name + " ---- ")

            statusline = ""
            # Build/rebuild image.sif if needed
            if os.path.exists("image.sif") and args.rebuild:
                printAndLog(log, "Removing {}/image.sif".format(d))
                os.remove("image.sif")
            if not os.path.exists("image.sif"):
                e,r = runAndLog(log, ["apptainer", "build", "--fakeroot",
                                "image.sif", "image.def"])
                if e != 0 or not os.path.exists("image.sif"):
                    statusline = "IMAGE BUILD FAILURE: " + r
                    ok = False
                    print("    Image build failure: you might want to run\n" +
                          "        cd " + d + "\n" +
                          "        apptainer build --fakeroot image.sif image.def\n")


            # Run the command in the image
            if ok:
                e,r = runAndLog(log,
                                ["apptainer", "run", "image.sif"] +
                                args.command)
                if e == 0:
                    statusline = "OK: " + r
                else:
                    statusline = "FAIL: " + r
                    ok = False
                    print("    Failure: you might want to run\n" +
                          "        cd " + d + "\n" +
                          "        apptainer shell image.sif\n" +
                          "        " + " ".join(args.command))

            status[name] = statusline

            if args.cleanup:
                if os.path.exists("image.sif"):
                    printAndLog(log, "Removing {}/image.sif".format(d))
                    os.remove("image.sif")
                if os.path.exists("chapel"):
                    printAndLog(log, "Removing {}/chapel".format(d))
                    shutil.rmtree("chapel")

            printAndLog(log, "")
            os.chdir(startDir)

        # Print out the status summary.
        printAndLog(log, "     -------- SUMMARY --------")

        for d in dirs:
            name = dirNameToConfigName(d)
            paddedName = name.rjust(maxNameLen)
            printAndLog(log, paddedName + ": " + status[name])

        if not ok:
            sys.exit(1)


if __name__ == '__main__':
    main()
