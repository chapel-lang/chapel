/*
= StarPU-Top for StarPU =

Copyright (C) 2011
William Braik
Yann Courtois
Jean-Marie Couteyen
Anthony Roy

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include <QtGui/QApplication>
#include "mainwindow.h"
#include <string.h>
#include <config.h>

#define PROGNAME "starpu_top"

static void parse_args(int argc, char **argv)
{
	if (argc == 1)
		return;

	if (argc > 2 || /* Argc should be either 1 or 2 */
	    strncmp(argv[1], "--help", 6) == 0 ||
	    strncmp(argv[1], "-h", 2) == 0)
	{
		(void) fprintf(stderr, "\
starpu-top is an interface which remotely displays the        \n\
on-line state of a StarPU application and permits the user    \n\
to change parameters on the fly.                              \n\
                                                              \n\
Usage: %s [OPTION]                                            \n\
                                                              \n\
Options:                                                      \n\
	-h, --help       display this help and exit           \n\
	-v, --version    output version information and exit  \n\
                                                              \n\
Report bugs to <" PACKAGE_BUGREPORT ">.",
PROGNAME);
	}
	else if (strncmp(argv[1], "--version", 9) == 0 ||
		 strncmp(argv[1], "-v", 2) == 0)
	{
		(void) fprintf(stderr, "%s %d.%d\n",
			PROGNAME, STARPU_MAJOR_VERSION, STARPU_MINOR_VERSION);
	}
	else
	{
		fprintf(stderr, "Unknown arg %s\n", argv[1]);
	}

	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	parse_args(argc, argv);

	QApplication a(argc, argv);

	// Application description
	QCoreApplication::setOrganizationName("INRIA Bordeaux Sud-Ouest");
	QCoreApplication::setOrganizationDomain("runtime.bordeaux.inria.fr");
	QCoreApplication::setApplicationName("StarPU-Top");
	QCoreApplication::setApplicationVersion("0.1");

	MainWindow w;
	w.show();

	return a.exec();
}
