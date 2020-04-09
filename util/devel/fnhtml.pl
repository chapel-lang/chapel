#!/usr/bin/env perl

if ($#ARGV != 2) {
    print"Content-type: text/html\n\n";
    die "usage: perl fnhtml.pl sys_dir log_dir fnid\n";
}

$sys_dir = $ARGV[0];
$log_dir = $ARGV[1];
$fnid = $ARGV[2];

print "Content-type: text/html\n\n";

print "<HTML>\n";
print "<HEAD>\n";
print "<TITLE> AST for Function $fnid </TITLE>\n";
print "<SCRIPT SRC=\"$sys_dir/etc/www/mktree.js\" LANGUAGE=\"JavaScript\"></SCRIPT>";
print "<LINK REL=\"stylesheet\" HREF=\"$sys_dir/etc/www/mktree.css\">";
print "</HEAD>\n";
print "<div style=\"text-align: center;\"><big><big><span style=\"font-weight: bold;\">";
print "AST for Function $fnid <br><br></span></big></big>\n";
print "<div style=\"text-align: left;\">\n\n";

for ($i = 1; $i < 100; $i++) {
    @files = `ls $log_dir/pass$i\_*.html`;
    if ($#files != -1) {
        @lines = `cat $log_dir/pass$i\_*.html`;
        @passname = split(/\"/, $lines[0]);
        print "<B>$passname[1]<BR>\n";
        $on = 0;
        foreach $line (@lines) {
            if ($line =~ m/CHPLTAG/) {
                if ($line =~ m/\"FN$fnid\"/) {
                    $on = 1 - $on;
                }
            } else {
                if ($on) {
                    print $line;
                }
            }
        }
    }
}
print "</HTML>\n";
