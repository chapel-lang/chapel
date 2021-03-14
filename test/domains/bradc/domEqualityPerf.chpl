use Time;

config var numTrials = 10,
           printTimings = false;


const D = {1..50};
var DS: sparse subdomain(D);
var DA: domain(real);

DS += 42;

DA += 4.2;


// Rectangular

{
  const startTime = getCurrentTime();
  for i in 1..numTrials do
    if (D == D) then
      ;
  else
    halt("Bad answer in D comparison");
  const execTime = getCurrentTime() - startTime;
  if printTimings then writeln("Time for rectangular equality = ", execTime);
}

{
  const startTime = getCurrentTime();
  for i in 1..numTrials do
    if (D != D) then
      halt("Bad answer in D comparison");
  const execTime = getCurrentTime() - startTime;
  
  if printTimings then writeln("Time for rectangular inequality = ", execTime);
}

// Sparse

{
  const startTime = getCurrentTime();
  for i in 1..numTrials do
    if (DS == DS) then
      ;
  else
    halt("Bad answer in D comparison");
  const execTime = getCurrentTime() - startTime;
  if printTimings then writeln("Time for sparse equality = ", execTime);
}

{
  const startTime = getCurrentTime();
  for i in 1..numTrials do
    if (DS != DS) then
      halt("Bad answer in D comparison");
  const execTime = getCurrentTime() - startTime;
  if printTimings then writeln("Time for sparse inequality = ", execTime);
}

// Associative

{
  const startTime = getCurrentTime();
  for i in 1..numTrials do
    if (DA == DA) then
      ;
  else
    halt("Bad answer in D comparison");
  const execTime = getCurrentTime() - startTime;
  if printTimings then writeln("Time for associative equality = ", execTime);
}

{
  const startTime = getCurrentTime();
  for i in 1..numTrials do
    if (DA != DA) then
      halt("Bad answer in D comparison");
  const execTime = getCurrentTime() - startTime;
  if printTimings then writeln("Time for associative inequality = ", execTime);
}

writeln("Done");
