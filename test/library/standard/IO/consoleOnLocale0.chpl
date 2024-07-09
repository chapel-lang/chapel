use IO;

if stdin.locale != here then writeln("stdin on dummy locale");
if stdout.locale != here then writeln("stdout on dummy locale");
if stderr.locale != here then writeln("stderr on dummy locale");

if stdin._home != here then writeln("stdin storing dummy locale");
if stdout._home != here then writeln("stdout storing dummy locale");
if stderr._home != here then writeln("stderr storing dummy locale");

writeln("Done!");
