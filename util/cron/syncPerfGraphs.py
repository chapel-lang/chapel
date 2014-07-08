#!/usr/bin/env python

# This script is intended to sync the performance graphs for the specified host
# to sourceforge. It is essentially a large wrapper for rsync. The reason it is
# separated, rather than just have an rsync command in place is so that we make
# sure the SF dest is the same, and that we can check for errors consistently
# before syncing.

# rsync over ssh is used to transfer the files to SourceForge. The user running
# this script needs to have configured access to web.sourceforge.net.

import os
import sys
import subprocess
import shlex
import time
import argparse

def main():
    parser = argparse.ArgumentParser(description='Syncs chapel performance'
        'graphs to sourceforge. Assumes user has configured access to'
        'web.sourceforge.net. Checks for a SUCCESS file in the directory that'
        'will be synced to ensure the graphs were successfully created.')
    parser.add_argument('baseDir', metavar='BASEDIR', help='the base directory'
    ' that contains the folder HOST/html/ to be synced')
    parser.add_argument('host', metavar='HOST', help='the host name')
    parser.add_argument('--logFile','-l', metavar='FILE', default=sys.stdout,
        type=argparse.FileType('w'), help='log file (default is stdout)')
    args = parser.parse_args()

    baseDir = args.baseDir
    host = args.host
    logFile = args.logFile

    sync = syncToSourceForge(baseDir, host, logFile)
    if sync != 0:
        exit(sync)

# Send the performance graphs for the host to sourceforge
# Returns the status of the rsync command (0 on success)
# or 124 (value that doesn't conflict with rsync exit codes) if the SUCCESS
# file wasn't found in the directory that is being synced
def syncToSourceForge(baseDir, host, logFile):

    logFile.write('SF sync log for: ' + time.strftime("%m/%d/%Y") + '\n\n')

    localPerfSrc = baseDir + '/' +host + '/html/'

    if not  os.path.isfile(localPerfSrc + '/SUCCESS'):
        logFile.write('SUCCESS file did not exist in %s. Assuming genGraph was'
          'unsuccessful. Graphs will NOT be synced.'%(localPerfSrc))
        logFile.close()
        return 124

    # Assumes correct username and authentication for web.sourceforge.net is
    # configured for the current system.
    sfPerfDest = 'web.sourceforge.net:/home/project-web/chapel/htdocs/perf/'+host

    logFile.write('Attempting to rsync %s to %s\n'%(localPerfSrc, sfPerfDest))

    # The rsync command that we will execute -- authenticates over ssh
    # --del to remove any old data (graphs merged, changed names, removed etc)
    rsyncCommand = 'rsync -avz --del -e ssh ' + localPerfSrc + ' ' + sfPerfDest
    logFile.write('Sync to sourceforge command was: ' + rsyncCommand + '\n\n')
    logFile.write('Rsync output is: \n')
    logFile.flush()

    # Actually send graphs to sourceforge and pipe output to the logfile
    rsync = subprocess.call(shlex.split(rsyncCommand), stdout=logFile)

    if rsync == 0:
        logFile.write('\n\nSuccessfully sent performance graphs to sourceforge')
    else:
        logFile.write('\n\nFailure sending performance graphs to sourceforge')

    logFile.close()

    return rsync


if __name__ == "__main__":
    main()
