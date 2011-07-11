/**
 * RCR Daemon implementation.
 * By Min Yeol Lim
 *
 * Includes some code based on the UDP server example from the Linux Gazette
 *   (http://www.linuxgazette.com/node/8758)
 * Uses some code based on the libpfm4 examples from Perfmon2
 *   (http://perfmon2.sourceforge.net/)
 */

#include <stdio.h>

#include "common_rcrtool.h"
#include "perf_util.h"
#include "bcGen.h"

#define TITLE "RCR Daemon (RCRdaemon) 1.00"

/*!
 * Emit some help info to the console.  The program will then exit.
 *
 * \param progName Name of this program, i.e. argv[0]
 */
void usage(char* progName) {
	printf(
		TITLE "\n"
		"  By Min Yeol Lim\n"
		"\n"
		"Syntax:\n"
		"  %s [-d [-O]] [-v]\n"
		"\n"
		"Options:\n"
		"  -d  Daemon mode; fork a daemon process to run in background\n"
		"  -O  When daemonizing, don't redirect stdout/stderr to null\n"
		"  -v  Verbose; output every step of the way.\n"
		"\n"
		"Parameters:\n",
		progName);
	exit(0);
}

/**
 * Daemon entry point.
 *  
 * @param argc
 * 
 * @param argv
 * 
 * @return
 * 
 */
int main(int argc, char** argv) {

	// Program name (i.e. argv[0])
	char* progName;

	progName = argv[0]; // Note our program name
	argc--; argv++; // ...and get rid of it

	// Get all cmdline args
	while (argc >= 1 && argv[0][0]=='-') {
		switch (argv[0][1]) {
			case 'v': bVerbose=1; break;
			case 'O': bOutputToNull=0; break;
			case 'd': bDaemon=1; break;
			default: usage(progName); // Invalid argument, give the help message and exit
		}
		
		argc--; argv++; // Shift cmdline arguments to get rid of this option we just processed
	}

	// Check for port number on command line
	if (argc >= 1) {
		usage(progName);
	}

	// Emit banner with options listing
	printf(
			TITLE "\n"
			"\n"
			"Options:\n"
			"  Verbose?      : %s\n"
			"  Daemon?       : %s\n"
			"  OutputToNull? : %s  (daemon mode only)\n"
			"\n",
	tf(bVerbose),
	tf(bDaemon),
	tf(bOutputToNull));

	// Become a daemon if we should
	if (bDaemon) {
		printf("Forking daemon...\n");
		daemonize(bOutputToNull);
	}

        if (pfm_initialize() != PFM_SUCCESS)
                errx(1, "libpfm initialization failed");

	printf("Up and running!\n");
	// Build the triggerMap for breadcrumbs insertion
	// FIXME: hard-coded fileName for trigger info
	buildTriggerMap("triggers.config");

        doWork();

	// We never get here, but who cares
	return EXIT_SUCCESS;
}


