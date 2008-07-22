#!/usr/bin/perl

use File::Basename;

my($filename, $dirname, $suffix) = fileparse($ARGV[0], ".chpl");

$cwd = `pwd`; chomp($cwd);

$chplhome = $ENV{'CHPL_HOME'};
$workdir = "$chplhome/util/chpl2pdf.tmp";
$latexfile = "$workdir/$filename.tex";

print "filename is: $filename\n";
print "dirname is: $dirname\n";
print "suffix is: $suffix\n";
print "latexfile is: $latexfile\n";

`echo "\\documentclass[11pt]{article}" > $latexfile`;
`echo "\\usepackage{times}" >> $latexfile`;
`echo "\\usepackage{fullpage}" >> $latexfile`;
`echo "\\usepackage{listings}" >> $latexfile`;
`echo "\\input{chapel_listing}" >> $latexfile`;
`echo "\\lstset{stepnumber=1}" >> $latexfile`;
`echo "\\begin{document}" >> $latexfile`;
`echo "\\lstinputlisting{$cwd/$ARGV[0]}" >> $latexfile`;
`echo "\\end{document}" >> $latexfile`;

`cp $chplhome/spec/chapel_listing.tex $workdir`;

`cd $workdir && pdflatex $latexfile < /dev/null`;

`cp $workdir/$filename.pdf .`;
