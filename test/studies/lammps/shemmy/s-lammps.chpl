//
// This test is pulled from intern ii02 in the codes written in the
// Summer of 2011 to measure increase in productivity using Chapel
// versus C/MPI. This is a pared down version of LAMMPS -- a “classical
// molecular dynamics” simulation code, developed primarily at the 
// Sandia National Laboratory.
//

use Norm, Time;
config const perfTest:bool = false;
const numAtoms:int = 3200;
const numSteps:int = 20;
const stepSize:real = 0.0025;
const thresh:real = 6.25;
const mass:real = 1.0;
param maxSize:int=256;
const ljs:4*real = (48.0,24.0,4.0,4.0);
const dts:3*real = (stepSize, 0.5*stepSize, 0.5*stepSize/mass);

const bounds:3*real = (33.5919,33.5919,33.5919);

const atomsExt:domain(1) = {0..numAtoms-1};
var   positions:[atomsExt] 3*real;

var   velocities:[atomsExt] 3*real;
var   forces:[atomsExt] 3*real;
var   neighbors:[atomsExt] maxSize*int;
var   membcounts:[atomsExt] int;
const infile:string = 'ljcut.in';

loadParticles(infile, positions, velocities);
var timer:Timer;
writeln("-- LAMMPS - (Serial,Chapel) --");
timer.start();

for i in 1..numSteps
{
//    var it:Timer;
//    it.start();
//    writeln("--------------------------");
//    writeln("iteration=",i);
//    writeln("initializing forces");

    for j in atomsExt
    {
	forces[j]=(0,0,0);
    }

//    writeln("updating neighbors");
    if(i==1 || i % 5 == 0) 
    {
	updateNeighbors();
    }

//    writeln("computing forces");
    computeForces();

//    writeln("updating positions");
    updatePositions();
    
    if(i==1 || i==numSteps)
    {
	writeln("i=",i, " positions[111]=",positions[111]);
    }
//    it.stop();
   // writeln("Iteration Time:",it.elapsed());
}

timer.stop();
if perfTest then writeln("TIME:",timer.elapsed());

proc computeForces()
{
    for i in atomsExt
    {
	var nhbrs:maxSize*int=neighbors[i];
	for m in 1..membcounts[i]
	{
	    var j:int = nhbrs[m];
	    var d:3*real = positions[i]-positions[j];
	  //  assert(positions[i]!=positions[j]);
	    var dist2:real = d[0]*d[0]+d[1]*d[1]+d[2]*d[2];
	    if(dist2<thresh)
	    {
	//	assert(dist2!=0);
		var f:3*real = computeForce(dist2,d);
		forces[i] += f;
		forces[i] += f;	
	    }
	}
    }
}

proc computeForce(dist2:real,d:3*real)
{
    var r2:real = 1.0/dist2;
    var r6:real = r2*r2*r2;  
    var fcoeff:real = r6*(ljs[0]*r6-ljs[1])*r2;      
    var f:3*real = (fcoeff*d[0],fcoeff*d[1],fcoeff*d[2]);
    return f;
}

proc updatePositions()
{ 
//                       dt         dtf           dtfm
//const dts:3*real = (stepSize, 0.5*stepSize, 0.5*stepSize/mass);
    for i in atomsExt
    {
	var f:3*real = forces[i];
	var a:3*real = (dts[2]*f[0],dts[2]*f[1],dts[2]*f[2]);
	var v:3*real = velocities[i] + a;

	velocities[i] = v;	
	v = (dts[0]*v[0],dts[0]*v[1],dts[0]*v[2]);
	positions[i] += v;
    }
}

proc updateNeighbors()
{
    for i in atomsExt
    {
	var li:maxSize*int;
	var cnt:int=0;
	for j in atomsExt
	{
	    if(i!=j)
	    {
		var d:3*real = positions[i]-positions[j];
		var dist2:real = d[0]*d[0]+d[1]*d[1]+d[2]*d[2];
		if(dist2<thresh)
		{
		    if(cnt<maxSize)
		    {
			cnt+=1;
			li[cnt]=j;
		    }
		}
	    }
	}
	membcounts[i]=cnt;
	neighbors[i]=li;
    }
}

proc loadParticles(filename:string, p:[?D], v:[D])
{
    use IO;
    var rawFile=open(filename, iomode.r, IOHINT_SEQUENTIAL);
    var fileIn=rawFile.reader(iokind.dynamic,true,0,max(int(64)), IOHINT_SEQUENTIAL);

    for i in D
    {
	var (idx,p0,p1,p2,v0,v1,v2):(int,real,real,real,real,real,real);
	fileIn.readln(idx,p0,p1,p2,v0,v1,v2);
	positions[i]=(p0,p1,p2);
	velocities[i]=(v0,v1,v2);
    }
}    
