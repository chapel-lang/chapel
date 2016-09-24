#!/usr/bin/env python

# This script is intended to sync the performance graphs to sourceforge. It is
# essentially a large wrapper for rsync. The reason it is separated, rather
# than just have an rsync command in place is so that we make sure the SF dest
# is the same, and that we can check for errors consistently before syncing.

# rsync over ssh is used to transfer the files to SourceForge. The user running
# this script needs to have configured access to web.sourceforge.net.

import contextlib
import os
import posixpath
import sys
import subprocess
import shlex
import time
import argparse

def main():
    parser = argparse.ArgumentParser(description='Syncs chapel performance '
        'graphs to sourceforge. Assumes user has configured access to '
        'web.sourceforge.net. Checks for a SUCCESS file in the directory that '
        'will be synced to ensure the graphs were successfully created.')
    parser.add_argument('dirToSync', metavar='DIR', help='perf directory to '
        'sync that contains the SUCCESS file')
    parser.add_argument('destDir', metavar='DIR', help='directory on sf to '
        'sync the graphs to.')
    parser.add_argument('--logFile','-l', metavar='FILE', default=sys.stdout,
        type=argparse.FileType('w'), help='log file (default is stdout)')
    args = parser.parse_args()

    dirToSync = args.dirToSync
    destDir = args.destDir
    logFile = args.logFile

    with contextlib.closing(logFile):
        sync = syncToSourceForge(dirToSync, destDir, logFile)
    exit(sync)

# Send the performance graphs to sourceforge
# Returns the status of the rsync command (0 on success)
# or 124 (value that doesn't conflict with rsync exit codes) if the SUCCESS
# file wasn't found in the directory that is being synced
def syncToSourceForge(dirToSync, destDir, logFile):

    logFile.write('SF sync log for: {0} \n\n'.format(time.strftime("%m/%d/%Y")))

    successFile = os.path.join(dirToSync, 'SUCCESS')

    if not os.path.isfile(successFile):
        logFile.write('SUCCESS file did not exist in {0}. Assuming genGraph was '
          'unsuccessful. Graphs will NOT be synced.\n'.format(dirToSync))
        return 124

    # Assumes correct username and authentication for web.sourceforge.net is
    # configured for the current system.
    sfWebHost = 'web.sourceforge.net'
    sfShellHost = 'shell.sourceforge.net'
    sfPerfBaseDir = '/home/project-web/chapel/htdocs/perf/'
    sfPerfDir = posixpath.join(sfPerfBaseDir, destDir)

    # create an interactive shell on sourceforge and immediately exit, which
    # allows us to do regular ssh commands (SF security thing)
    getShellCommand = 'ssh chapeladmin,chapel@{0} create '.format(sfShellHost)
    getShellDesc = 'create interactive sourceforge shell'
    executeCommand(getShellCommand, getShellDesc, logFile)

    # Delete files older than 100 days. Don't just use `rsync --del` because
    # there might be subdirectories we don't want to delete, ignore errors
    delOldCommand = 'ssh {0} "find {1} -ctime +100 | xargs rm -rf "'.format(sfShellHost, sfPerfDir)
    delOldDesc = 'delete old files'
    executeCommand(delOldCommand, delOldDesc, logFile)

    # rsync, authenticating with ssh
    rsyncDest = '{0}:{1}'.format(sfWebHost, sfPerfDir)
    rsyncCommand = 'rsync -avz -e ssh {0} {1}'.format(dirToSync, rsyncDest)
    rsyncDesc = 'rsync perf graphs to sourceforge'
    return executeCommand(rsyncCommand, rsyncDesc, logFile)


# Helper function for execute a command and log results/progress
def executeCommand(command, commandDesc, logFile):
    logFile.write('Attempting to {0} with `{1}`...\n'.format(commandDesc, command))
    logFile.flush()

    commandRet = subprocess.call(shlex.split(command), stdout=logFile, stderr=subprocess.STDOUT)

    if commandRet == 0:
        logFile.write('\nCommand to {0} succeeded\n\n'.format(commandDesc))
    else:
        logFile.write('\nCommand to {0} failed with error code {1}\n\n'.format(commandDesc, commandRet))

    logFile.flush()

    return commandRet


if __name__ == "__main__":
    main()
