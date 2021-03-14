#!/usr/bin/env perl

@files = `find . -name "*.cpp"`;

foreach $oldfile (@files) {
    chomp $oldfile;
    $oldfile =~ s/\.\/(.*)/$1/;
    $newfile = $oldfile;
    $newfile = reverse $newfile;
    $newfile =~ s/\//\/ung\/46xunil\/neg\//;
    $newfile = reverse $newfile;
    `cp $oldfile $newfile >& /dev/null`;

    ($pathname, $filename) = $newfile =~ m|^(.*[/\\])([^/\\]+?)$|;
    `cd $pathname >& /dev/null; gcov $filename >& /dev/null`;

    @lines = `cat $newfile.gcov`;

    $print_header = 0;
    $found_function = 0;
    foreach $line (@lines) {
	if ($found_function == 1) {
	    $old = $line;
	    chomp $old;
	    if ($line =~ s/.*?:.*?:(.*?)\(.*/$1/) {
		chomp $line;
		$function = $line;
		$print_function = 0;
	    }
	    $found_function = 0;
	}
	if ($line =~ m/function(.*)called(.*)returned(.*)executed/) {
	    $found_function = 1;
	}
	if ($line =~ m/#####/) {
	    if ($print_header == 0) {
		print "\n";
		print "$newfile\n";
		$print_header = 1;
	    }
	    if ($print_function == 0) {
		print "\n  ", $function, "\n";
		$print_function = 1;
	    }
	    chomp $line;
	    $line =~ s/\ \ \ \ \#\#\#\#\#\:\ (.*)/$1/;
	    print "  $line\n";
	}
    }
}
