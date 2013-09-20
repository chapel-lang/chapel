// contains functions for computing energy, temperature, and pressure

use miniMD;
use initMD;
use forces;
use Time;

var curStat : int;

var temp, press, eng : real;

// scientific constants
var tempScale, pressScale, engScale, mvv2e, dof_boltz : real;

// holds data regarding temperature, energy, pressure
// later used in more verbose output (unimplemented)
var stepSpace : domain(1);
var steps : [stepSpace] int;
var temps : [stepSpace] real;
var engs : [stepSpace] real;
var prs : [stepSpace] real;

proc initThermo() {
  const maxStats = if thermoEvery == 0 then 2
    else numSteps / thermoEvery + 2;
  stepSpace = {1..maxStats};

  // depending on the units, establish scientific variables
  if units == "lj" {
    mvv2e = 1.0;
    dof_boltz = numAtoms * 3 - 3;
    tempScale = mvv2e / dof_boltz;
    pressScale = 1.0 / 3 / volume;
    engScale = 0.5;
  } else {
    mvv2e = 1.036427e-4;
    dof_boltz = (numAtoms * 3 - 3) * 8.617343e-5;
    tempScale = mvv2e / dof_boltz;
    pressScale = 1.602176e6 / 3 / volume;
    engScale = 524287.985533;
    dtforce /= mvv2e;
  } 
}

proc computeThermo(step : int, total : Timer) {

  // if not the 0th step and we shouldn't compute, leave
  if step > 0 && (step % thermoEvery >= 1) then return;

  // if this was called as the last compute, and we had just computed 
  // during the last iteration, leave
  if step == -1 && thermoEvery > 0 && (numSteps % thermoEvery == 0) then return;

  // calculate
  temperature();
  eng = fobj.eng_vdwl * engScale / numAtoms;
  press = (temp * dof_boltz + fobj.virial) * pressScale;

  // by convention, -1 will be used as the final compute
  var istep = step;
  if step == -1 then istep = numSteps;
  if step == 0 then curStat = 1;

  // store data
  steps[curStat] = istep;
  temps[curStat] = temp;
  engs[curStat] = eng;
  prs[curStat] = press;

  curStat += 1;

  var tval : real = 0.0;
  if istep != 0 then tval = total.elapsed();

  if printOriginal then writef("%i %er %er %er %.6dr\n", istep, temp, eng, press, tval);
  else if printCorrect then  writef("%i %er %er %er\n", istep, temp, eng, press);
}

proc temperature() {
  temp = 0;
  const act = + reduce forall (b, c) in zip(Bins, RealCount) do
    + reduce forall a in b[1..c] do (dot(a.v, a.v) * mass);
  temp = (act) * tempScale;
  return temp;
}
