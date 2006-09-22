`rm -f .preprocess.pl.log`;
`echo "preprocess log" > .preprocess.pl.log`;
@lines = <STDIN>;
foreach $line (@lines) {
    if ($line =~ m/sekshun\{(.*)\}/) {
        $name = $1;
        $name =~ s/\s/_/g;
        $name =~ s/\\em\s//g;
        $name =~ s/\\//g;
        $name =~ s/\?/Q/g;
        $name =~ s/\{//g;
        $name =~ s/\}//g;
        if ($dir) {
            @files = `ls $dir`;
            foreach $file (@files) {
                chomp $file;
                if ($file =~ m/(.*)\.tex/) {
                    if (!$dirnames{$1}) {
                        `echo "WARNING: unused file $dir/$1.tex" >> .preprocess.pl.log`;
                    }
                }
            }
        }
        $dir = $name;
        %dirnames = ();
        if ($names{$name}) {
            `echo "WARNING: duplicate name $name" >> .preprocess.pl.log`;
        }
        $names{$name} = 1;
        $dirnames{$name} = 1;
    }
    if ($line =~ m/section\{(.*)\}/) {
        $name = $1;
        $name =~ s/\s/_/g;
        $name =~ s/\\em\s//g;
        $name =~ s/\\//g;
        $name =~ s/\?/Q/g;
        $name =~ s/\{//g;
        $name =~ s/\}//g;
        if ($names{$name}) {
            `echo "WARNING: duplicate name $name" >> .preprocess.pl.log`;
        }
        $names{$name} = 1;
        $dirnames{$name} = 1;
    }
}
foreach $line (@lines) {
    chomp $line;
    if ($line =~ m/\\sekshun\{(.*)\}/) {
        print "\\sekshun{$1}";
        $name = $1;
        $name =~ s/\s/_/g;
        $name =~ s/\\em\s//g;
        $name =~ s/\\//g;
        $name =~ s/\?/Q/g;
        $name =~ s/\{//g;
        $name =~ s/\}//g;
        print "\\label{$name}";
        print "\\input{$name/$name}";
        print "\n";
        $dir = $name;
        if (!-e $dir) {
            `echo "Making directory $dir" >> .preprocess.pl.log`;
            `mkdir $dir`;
        }
        if (!-e "$dir/$name.tex") {
            `echo "Making file $dir/$name" >> .preprocess.pl.log`;
            `cp stub.tex $dir/$name.tex`;
        }
    } elsif ($line =~ m/\\subsection\{(.*)\}/) {
        print "\\subsection{$1}";
        $name = $1;
        $name =~ s/\s/_/g;
        $name =~ s/\\em\s//g;
        $name =~ s/\\//g;
        $name =~ s/\?/Q/g;
        $name =~ s/\{//g;
        $name =~ s/\}//g;
        print "\\label{$name}";
        print "\\input{$dir/$name}";
        print "\n";
        if (!-e "$dir/$name.tex") {
            `echo "Making file $dir/$name" >> .preprocess.pl.log`;
            `cp stub.tex $dir/$name.tex`;
        }
    } elsif ($line =~ m/\\subsubsection\{(.*)\}/) {
        print "\\subsubsection{$1}";
        $name = $1;
        $name =~ s/\s/_/g;
        $name =~ s/\\em\s//g;
        $name =~ s/\\//g;
        $name =~ s/\?/Q/g;
        $name =~ s/\{//g;
        $name =~ s/\}//g;
        print "\\label{$name}";
        print "\\input{$dir/$name}";
        print "\n";
        if (!-e "$dir/$name.tex") {
            `echo "Making file $dir/$name" >> .preprocess.pl.log`;
            `cp stub.tex $dir/$name.tex`;
        }
    } else {
        print $line, "\n";
    }
}
