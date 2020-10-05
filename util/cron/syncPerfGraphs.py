#!/usr/bin/env python3

# This script is intended to sync the performance graphs to dreamhost. It is
# essentially a large wrapper for rsync. The reason it is separated, rather
# than just have an rsync command in place is so that we make sure the dreamhost
# dest is the same, and that we can check for errors consistently before syncing.

# rsync over ssh is used to transfer the files to Dreamhost. The user running
# this script needs to have configured access to tower.dreamhost.com

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
        'graphs to dreamhost. Assumes user has configured access to '
        'tower.dreamhost.com. Checks for a SUCCESS file in the directory that '
        'will be synced to ensure the graphs were successfully created.')
    parser.add_argument('dirToSync', metavar='DIR', help='perf directory to '
        'sync that contains the SUCCESS file')
    parser.add_argument('destDir', metavar='DIR', help='directory on dreamhost to '
        'sync the graphs to.')
    parser.add_argument('--logFile','-l', metavar='FILE', default=sys.stdout,
        type=argparse.FileType('w'), help='log file (default is stdout)')
    parser.add_argument('--numRetries','-r',  metavar='N', default=5,
        type=int, help='max number of retries for sync command')

    args = parser.parse_args()

    dirToSync = args.dirToSync
    destDir = args.destDir
    logFile = args.logFile
    numRetries = args.numRetries

    with contextlib.closing(logFile):
        sync = syncToDreamhost(dirToSync, destDir, logFile, numRetries)
    exit(sync)

# Send the performance graphs to dreamhost
# Returns the status of the rsync command (0 on success)
# or 124 (value that doesn't conflict with rsync exit codes) if the SUCCESS
# file wasn't found in the directory that is being synced
def syncToDreamhost(dirToSync, destDir, logFile, numRetries):

    logFile.write('Dreamhost sync log for: {0} \n\n'.format(time.strftime("%m/%d/%Y")))

    successFile = os.path.join(dirToSync, 'SUCCESS')

    if not os.path.isfile(successFile):
        logFile.write('SUCCESS file did not exist in {0}. Assuming genGraph was '
          'unsuccessful. Graphs will NOT be synced.\n'.format(dirToSync))
        return 124

    # Assumes correct username and authentication for tower.dreamhost.com is
    # configured for the current system.
    webHost = 'tower.dreamhost.com'
    perfBaseDir = '/home/chapeljenkins/chapel-lang.org/perf'
    perfDir = posixpath.join(perfBaseDir, destDir)

    # Delete files older than 100 days. Don't just use `rsync --del` because
    # there might be subdirectories we don't want to delete, ignore errors
    delOldCommand = 'ssh {0} "find {1} -ctime +100 | xargs rm -rf "'.format(webHost, perfDir)
    delOldDesc = 'delete old files'
    executeCommand(delOldCommand, delOldDesc, logFile)

    # rsync, authenticating with ssh
    rsyncDest = '{0}:{1}'.format(webHost, perfDir)
    rsyncCommand = 'rsync -avz -e ssh {0} {1}'.format(dirToSync, rsyncDest)
    rsyncDesc = 'rsync perf graphs to dreamhost'
    rsyncRet = 0
    for _ in range(numRetries):
        rsyncRet = executeCommand(rsyncCommand, rsyncDesc, logFile)
        if rsyncRet == 0:
            break
        else:
            delay = 30
            logFile.write('Waiting {0} seconds before retrying sync...\n\n'.format(delay))
            time.sleep(delay)
    return rsyncRet


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
