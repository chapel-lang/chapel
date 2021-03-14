/* Comments
  - 'Locales' is being flagged as a compile-time error
  - No support for dimensional distributions
  - array index out of bounds thrown at run-time when try to do forall (i, j, k) in lndD
  - array slicing using arithmetic sequence necessarily expects a sequence and not a singular value
  - error messages could be more informative	
*/
 
/* Component model names */
param atm = "ATMOSPHERE",
      ocn = "OCEAN",
      lnd = "LAND",
      cpl = "COUPLED";

/* Cell parameters */
config const atx = 5, aty = 4, atz = 2,
	     otx = 5, oty = 4, otz = 3,
	     ltx = 5, lty = 4, ltz = 1;

/* Grid parameters */
config const anx = 30, any = 8, anz = 4,
	     onx = 10, ony = 8, onz = 9,
	     lnx = 20, lny = 8, lnz = 1;

/* Processor parameters */
const apx = anx / atx, apy = any / aty, apz = anz / atz,
      opx = onx / otx, opy = ony / oty, opz = onz / otz,
      lpx = lnx / ltx, lpy = lny / lty, lpz = lnz / ltz;  

config const typeSim = cpl; 

/* Code for 3D distribution of atmosphere grid 
const atmP : domain(3) = {1..apx, 1..apy, 1..apz};
const atmL : [atmP] locale = reshape(Locales);
const atmD : domain(3) dmapped(Block, Block, Block) on atmL = {1..anx, 1..any, 1..anz};
const atmA : [atmD] real; 
*/

const atmD : domain(3) = {0..anz+1, 0..any+1, 0..anx+1};
var   atmA : [atmD] real = -1; 

const ocnD : domain(3) = {0..onz+1, 0..ony+1, 0..onx+1};
var   ocnA : [ocnD] real = -1;

const lndD : domain(3) = {0..lnz+1, 0..lny+1, 0..lnx+1};
var   lndA : [lndD] real = -1;

atmA[1..anz, 1..any, 1..anx] = 1; 
ocnA[1..onz, 1..ony, 1..onx] = 2;
lndA[1..lnz, 1..lny, 1..lnx] = 3;

cobegin {
	/* Atmosphere computation */

	/* Ocean computation */

	/* Land computation */
}

cobegin {
	/* Coupling atmosphere and ocean */
	atmA[0..0, 1..any, 1..onx] = ocnA[onz..onz, 1..ony, 1..onx];
	ocnA[onz+1..onz+1, 1..ony, 1..onx] = atmA[1..1, 1..any, 1..onx];
 
	/* Coupling atmosphere and land */ 
	atmA[0..0, 1..any, onx+1..anx] = lndA[lnz..lnz, 1..lny, 1..lnx];
	lndA[lnz+1..lnz+1, 1..lny, 1..lnx] = atmA[1..1, 1..any, onx+1..anx]; 

	/* Coupling ocean and land */
	ocnA[onz..onz, 1..ony, onx+1..onx+1] = lndA[lnz..lnz, 1..lny, 1..1];
	lndA[lnz..lnz, 1..lny, 0..0] = ocnA[onz..onz, 1..ony, onx..onx];	
}

writeln("Atmosphere\n", atmA);
writeln("Ocean\n", ocnA);
writeln("Land\n", lndA);
