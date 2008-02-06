#!/usr/bin/perl

$chplhome = $ENV{'CHPL_HOME'};

$taskfile = "$chplhome/doc/internal/TASKS";
$htmlfile = "$chplhome/doc/internal/taskpage.html";

open TASKS, "$taskfile" or die "can't open $taskfile $!";
my @tasks = <TASKS>;
close (TASKS);

open HTML, ">$htmlfile" or die "can't open $htmlfile for creating $!";

print HTML "<html>\n";
print HTML "  <head>\n";
print HTML "    <title>Chapel Task List</title>\n";
print HTML "    <script src=\"$chplhome/compiler/etc/www/mktree.js\" language=\"JavaScript\"></script><link rel=\"stylesheet\" href=\"$chplhome/compiler/etc/www/mktree.css\">\n";
print HTML "  </head>\n";
print HTML "  <body>\n";

my @blanksAtDepth;

$currentDepth = 0;
@blanksAtDepth[$currentDepth] = 0;
foreach my $task (@tasks) {
    chomp($task);

    $task = urlify($task);

    $newNumBlanks = numBlanks($task);
    if ($newNumBlanks >= 0) {
        $newDepth = $currentDepth;
        if ($newNumBlanks > @blanksAtDepth[$currentDepth]) {
            $newDepth = $currentDepth + 1;
            @blanksAtDepth[$newDepth] = $newNumBlanks;
        } elsif ($newNumBlanks < @blanksAtDepth[$currentDepth]) {
            $newDepth = $currentDepth - 1;
            $foundit = 0;
            do {
#                print "checking whether @blanksAtDepth[$newDepth] == $newNumBlanks\n";
                if (@blanksAtDepth[$newDepth] == $newNumBlanks) {
                    $foundit = 1;
                } else {
                    $newDepth -= 1;
                }
#                print "newDepth = $newDepth\n";
            } while ($foundit == 0 && $newDepth >= 0);
            if ($foundit == 0) {
                print "ERROR: couldn't find appropriate depth\n";
                print "@blanksAtDepth\n";
            }
        }
#        print HTML "<!-- blanks = $newNumBlanks-->\n";
#        print HTML "<!-- depth = $newDepth/$currentDepth-->\n";
        if ($newDepth > $currentDepth) {
            for (; $currentDepth < $newDepth; $currentDepth++) {
                for ($i=-1; $i<$currentDepth+1; $i++) {
                    print HTML "  ";
                }
                print HTML "<ul";
                if ($newDepth == 1 && $currentDepth == 0) {
                    print HTML " class=\"mktree\"";
                }
                print HTML ">\n";
            }
        } elsif ($newDepth < $currentDepth) {
            for (; $currentDepth > $newDepth; $currentDepth--) {
                for ($i=0; $i<$currentDepth+1; $i++) {
                    print HTML "  ";
                }
                print HTML "</ul>\n";
            }
        }
        if ($currentDepth == 0) {
            print HTML "\n";
            print HTML "  <p><b>$task</b><br>\n";
        } else {
            for ($i=-1; $i<$currentDepth+1; $i++) {
                print HTML "  ";
            }
            print HTML "<li> ", $task, "<br>\n";
        }
    }
}

print HTML "  </body>\n";
print HTML "</html>\n";

exit 0;

sub numBlanks {
    $str = $_[0];

    if ($str eq "") {
        $numBlanks = -1;
    } else {
        $numBlanks = 0;
        do {
            $achar = substr($str, $numBlanks, 1);
#            print "substr[$numBlanks] = '$achar'\n";
            $numBlanks += 1;
        } while ($achar eq " ");
        $numBlanks -= 1;
    }
#    print "numBlanks = $numBlanks\n";
    return $numBlanks;
}


sub urlify {
    $str = $_[0];

    if ($str =~ m/(.*)\[(.*)\](.*)/) {
#        print "found a hyperlink: $2\n";
        $path = "$chplhome/test/$2";
        if (-e "$path") {
            $newstr = "$1\[<a href = \"file://$path\">$2</a>]$3";
        } else {
            print "ERROR: Cannot find $2\n";
            exit(1);
        }
    } else {
        $newstr = $str;
    }
    return $newstr;
}
