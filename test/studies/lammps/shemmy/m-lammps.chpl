//
// This test is pulled from intern ii02 in the codes written in the
// Summer of 2011 to measure increase in productivity using Chapel
// versus C/MPI. This is a pared down version of LAMMPS -- a “classical
// molecular dynamics” simulation code, developed primarily at the 
// Sandia National Laboratory.
//

use Norm, Time, BlockDist;
config const perfTest:bool = false;
config const numAtoms:int = 3200;
config const numSteps:int = 20;
const stepSize:real = 0.0025;
const thresh:real = 6.25;
const mass:real = 1.0;
param maxSize:int=256;
const ljs:4*real = (48.0,24.0,4.0,4.0);
const dts:3*real = (stepSize, 0.5*stepSize, 0.5*stepSize/mass);

const bounds:3*real = (33.5919,33.5919,33.5919);

const atomsExt:domain(1) = {0..numAtoms-1};
const atomsDist:domain(1) dmapped Block(atomsExt)=atomsExt;

const locExt:domain(1) = {0..numLocales-1};
const locDist:domain(1) dmapped Block(locExt) = locExt;

const locGridExt:domain(2) = {0..numLocales-1,1..1};		
const locGrid:[locGridExt] locale = reshape(Locales,locGridExt);

const copyExt:domain(2) = {0..numLocales-1, 0..numAtoms-1};
const copyDist:domain(2) dmapped Block(copyExt,locGrid) = copyExt;


var   positionsLoc:[copyDist] 3*real;
var   needsUpdate:[copyDist] bool;
var   locks:[locDist] sync bool;

var   positions:[atomsDist] 3*real;

var   velocities:[atomsDist] 3*real;
var   forces:[atomsDist] 3*real;

const neighborExt:domain(2) = {0..numAtoms-1,0..maxSize-1};
const neighborDist:domain(2) dmapped Block(neighborExt,locGrid)=neighborExt;
var   neighbors:[neighborDist] int;
var   membcounts:[atomsDist] int;

config const infile:string = 'ljcut.in';

loadParticles(infile, positions, velocities);
var timer:Timer;
writeln("-- LAMMPS - (Parallel,Chapel) -- ");
timer.start();

for i in 1..numSteps
{
//    var it:Timer;
//    it.start();

    for j in atomsDist
    {
	forces[j]=(0,0,0);
    }

    if(i==1 || i % 5 == 0) 
    {
	updateNeighbors();
    }

/**
    if(i==1)
    {
	for k in [0..9]
	{
	    writeln("nbhrs[",k,"]=",neighbors[k,0..membcounts[k]-1]);
	}
    }
    **/

    computeForces();
/**
    if(i==1)
    {
	for k in [0..9]
	{
	    writeln("f[",k,"]=",forces[k]);
	}
    }
    **/

    updatePositions();
  
    if(i==1 || i==numSteps)
    {
	writeln("i=",i, " positions[111]=",positions[111]);
    }
//    it.stop();
//    writeln("Iteration Time:",it.elapsed());
}

timer.stop();
if perfTest then writeln("TIME   :",timer.elapsed());
//writeln("LOCALES:",numLocales);
proc computeForces()
{
    forall i in atomsDist
    {
	local
	{
	    var p:3*real = positionsLoc[here.id,i];	
	    for m in 0..membcounts[i]-1
	    {		
		var j:int = neighbors[i,m];
		var q:3*real = positionsLoc[here.id,j];
		var d:3*real = p-q;

		var dist2:real = d[0]*d[0]+d[1]*d[1]+d[2]*d[2];
		if(dist2<thresh)
		{
		    var f:3*real = computeForce(dist2,d);
		    forces[i] += f;
		    forces[i] += f;	
		}
	    }
	}
    }
    //writeln("forces=",forces);
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
    forall i in atomsDist
    {
	local
	{
	    var f:3*real = forces[i];
	    var a:3*real = (dts[2]*f[0],dts[2]*f[1],dts[2]*f[2]);
	    var v:3*real = velocities[i] + a;

	    velocities[i] = v;	
	    v = (dts[0]*v[0],dts[0]*v[1],dts[0]*v[2]);
	    positionsLoc[here.id,i] += v;
	}
    }
    
    forall (loc,i) in copyDist
    {
	local do needsUpdate[loc,i]=true;
    }

    forall i in atomsDist
    {
	local 
	{
	    positions[i]=positionsLoc[here.id,i];
	    needsUpdate[here.id,i]=false;
	}
    }

    forall i in atomsDist
    {
	locks[here.id]=true;
	for m in 0..membcounts[i]-1
	{
	    var j:int=neighbors[i,m];
	    if(needsUpdate[here.id,j])
	    {
		positionsLoc[here.id,j]=positions[j];
		needsUpdate[here.id,j]=false;
	    }
	}
	locks[here.id];
    }
}

proc updateNeighbors()
{
    forall ii in atomsDist
    {
	local do positions[ii]=positionsLoc[here.id,ii];
    }

    forall (loc,i) in copyDist
    {
	positionsLoc[loc,i]=positions[i];
    }

    forall i in atomsDist
    {
	local
	{
	    var cnt:int=0;
	    for j in 0..numAtoms-1
	    {
		var p:3*real=positionsLoc[here.id,i];
		if(i!=j)
		{
		    var q:3*real = positionsLoc[here.id,j];
		    var d:3*real = p-q;
		    var dist2:real = d[0]*d[0]+d[1]*d[1]+d[2]*d[2];
		    if(dist2<thresh)
		    {
			if(cnt<maxSize)
			{
			    neighbors[i,cnt]=j;
			    cnt+=1;	
			}
		    }
		}
	    }
	    membcounts[i]=cnt;
	}
    }
   // writeln("membcounts=",membcounts);
   // writeln("neighbors=",neighbors);
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
    forall i in atomsDist
    {
	positionsLoc[here.id,i]=positions[i];
    }
}    
