#!/usr/bin/perl


$chplhome = $ENV{'CHPL_HOME'};
if ($chplhome eq "") {
    print "Error: CHPL_HOME must be set to use updateSVNLOG.pl\n";
    exit(0);
}

`svn propset --file $chplhome/etc/svn/testSvnIgnores.txt svn:ignore .`
