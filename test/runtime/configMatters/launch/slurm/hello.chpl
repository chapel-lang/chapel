config const numTasks = 2;
for l in Locales do
  on l do
    for tid in 0..#numTasks do
      writeln("Hello from task", tid, "on locale", here.id, sep=" ");
