@edition(first="preview")
proc bam(p: real(?)) do
  writeln("In preview bam");

@edition(last="2.0")
proc bam(p: real(?)) do
  writeln("In 2.0 bam");

bam(2.0);
