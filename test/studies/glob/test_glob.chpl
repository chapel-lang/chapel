use Glob;

writeln("======== SERIAL ITERATORS (recursive) ==========");

writeln("-------- GLOB ----------");
for s in my_glob("[a-p]*",recursive=true) do
    writeln(s);

writeln("-------- WORDEXP ----------");
for s in my_wordexp("[a-p]*",recursive=true) do
    writeln(s);

writeln("-------- WORDEXP `zip` GLOB ----------");
for s in zip(my_wordexp("[a-p]*", recursive=true), my_glob("[a-p]*",recursive=true)) do 
  writeln(s);

writeln("======== SERIAL ITERATORS (non-recursive) ==========");

writeln("-------- GLOB ----------");
for s in my_glob("[a-p]*") do
    writeln(s);

writeln("-------- WORDEXP ----------");
for s in my_wordexp("[a-p]*") do
    writeln(s);

writeln("-------- WORDEXP `zip` GLOB ----------");
for s in zip(my_wordexp("[a-p]*"), my_glob("[a-p]*")) do 
  writeln(s);

writeln("======== PARALLEL ITERATORS (recursive) ==========");

writeln("-------- GLOB ----------");
forall s in my_glob("[a-p]*",recursive=true) do
    writeln(s);

writeln("-------- WORDEXP ----------");
forall s in my_wordexp("[a-p]*",recursive=true) do
    writeln(s);

writeln("-------- WORDEXP `zip` GLOB ----------");
forall s in zip(my_wordexp("[a-p]*",recursive=true), my_glob("[a-p]*",recursive=true)) do 
  writeln(s);

writeln("======== PARALLEL ITERATORS (non-recursive) ==========");

writeln("-------- GLOB ----------");
forall s in my_glob("[a-p]*") do
    writeln(s);

writeln("-------- WORDEXP ----------");
forall s in my_wordexp("[a-p]*") do
    writeln(s);

writeln("-------- WORDEXP `zip` GLOB ----------");
forall s in zip(my_wordexp("[a-p]*"), my_glob("[a-p]*")) do 
  writeln(s);
