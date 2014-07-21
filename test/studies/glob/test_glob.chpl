use Glob;

writeln("======== SERIAL ITERATORS (recursive) ==========");

writeln("-------- GLOB ----------");
for s in glob("[a-z]*",recursive=true) do
    writeln(s);

writeln("-------- WORDEXP ----------");
for s in wordexp("[a-z]*",recursive=true) do
    writeln(s);

writeln("-------- WORDEXP `zip` GLOB ----------");
for s in zip(wordexp("[a-z]*", recursive=true), glob("[a-z]*",recursive=true)) do 
  writeln(s);

writeln("======== SERIAL ITERATORS (non-recursive) ==========");

writeln("-------- GLOB ----------");
for s in glob("[a-z]*") do
    writeln(s);

writeln("-------- WORDEXP ----------");
for s in wordexp("[a-z]*") do
    writeln(s);

writeln("-------- WORDEXP `zip` GLOB ----------");
for s in zip(wordexp("[a-z]*"), glob("[a-z]*")) do 
  writeln(s);

writeln("======== PARALLEL ITERATORS (recursive) ==========");

writeln("-------- GLOB ----------");
forall s in glob("[a-z]*",recursive=true) do
    writeln(s);

writeln("-------- WORDEXP ----------");
forall s in wordexp("[a-z]*",recursive=true) do
    writeln(s);

writeln("-------- WORDEXP `zip` GLOB ----------");
forall s in zip(wordexp("[a-z]*",recursive=true), glob("[a-z]*",recursive=true)) do 
  writeln(s);

writeln("======== PARALLEL ITERATORS (non-recursive) ==========");

writeln("-------- GLOB ----------");
forall s in glob("[a-z]*") do
    writeln(s);

writeln("-------- WORDEXP ----------");
forall s in wordexp("[a-z]*") do
    writeln(s);

writeln("-------- WORDEXP `zip` GLOB ----------");
forall s in zip(wordexp("[a-z]*"), glob("[a-z]*")) do 
  writeln(s);
