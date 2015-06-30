use BlockDist;
use VisualDebug;

config var ncells = 100, nsteps = 20,  leftX = -10.0, rightX = +10.0,  
           sigma = 3.0, ao = 1.0, coeff = .375;

proc main() {  
  const pDomain  = {1..ncells} dmapped Block({1..ncells});
  const interior = pDomain.expand(-1);
  const dx = (rightX - leftX)/(ncells-1);
  var x, temp, tempNew : [pDomain] real = 0.0;

  startVdebug("DF");
  
  forall i in pDomain do {
    x[i] = leftX + (i-1)*dx;
    temp[i] = ao*exp(-x[i]*x[i]/(2.0*sigma*sigma));
  }

  pauseVdebug("point 1");

  writeln("ICs: ", temp, "\n");

  resumeVdebug();

  for step in [1..nsteps] do {
    forall i in interior do
      tempNew(i) = temp(i) + coeff*(temp(i-1) - 2.0*temp(i) + temp(i+1));
    temp[interior] = tempNew[interior];
  }
  stopVdebug();

  writeln("Final: ", temp);
}
