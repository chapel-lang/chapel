use Time, IO, BlockDist;

config const timer : bool = false;
var t : Timer;

//Precision to use for calculations

type fptype = real;
config const NUM_RUNS = 100;

record OptionData {
	var s : fptype; 	// spot price
	var strike : fptype; 	// strike price
	var r : fptype;		// risk-free interest rate
	var divq : fptype;	// dividend rate
	var v : fptype;		// volatility
	var t : fptype;		// time to maturity or option expiration in years
				// (1yr = 1.0, 6mos = 0.5, 3mos = 0.25, ..., etc)
	var OptionType : string;	// Option Type. "P"=PUT, "C"=CALL
	var divs : fptype;	// dividend vals (not used in this test)
	var DGrefval : fptype;	// DerivaGem Reference Value
}


var numError : int = 0;

config var numOptions : int(64) = 1000;
config const ERR_CHK = false;
config const filename = "optionData.txt";

const Dist = new dmap(new Block(rank=1, idxType=int(64), boundingBox={0..#numOptions},
                                dataParTasksPerLocale=here.maxTaskPar));
const Dom : domain(1, int(64)) dmapped Dist = {0..#numOptions};

var data : [Dom] OptionData;
var prices : [Dom] fptype;
var otype : [Dom] bool;

// Cumulative Normal Distribution Function
// See Hull, Section 11.8, P.243-244
param inv_sqrt_2xPI = 0.39894228040143270286;

proc CNDF ( in InputX : fptype )  : fptype
{
    var sign : int;
    // Check for negative value of InputX
    if (InputX < 0.0) then {
        InputX = -InputX;
        sign = 1;
    } else 
        sign = 0;
 
    // Compute NPrimeX term common to both four & six decimal accuracy calcs
    var xNPrimeofX = exp(-0.5 * InputX * InputX) * inv_sqrt_2xPI;

    var xK2 = 0.2316419 * InputX;
    xK2 = 1.0 + xK2;
    xK2 = 1.0 / xK2;
    var xK2_2 = xK2 * xK2;
    var xK2_3 = xK2_2 * xK2;
    var xK2_4 = xK2_3 * xK2;
    var xK2_5 = xK2_4 * xK2;
    
    var xLocal_1 = xK2 * 0.319381530;
    var xLocal_2 = xK2_2 * (-0.356563782);
    var xLocal_3 = xK2_3 * 1.781477937;
    xLocal_2 += xLocal_3;
    xLocal_3 = xK2_4 * (-1.821255978);
    xLocal_2 += xLocal_3;
    xLocal_3 = xK2_5 * 1.330274429;
    xLocal_2 += xLocal_3;

    xLocal_1 += xLocal_2;
    var xLocal   = xLocal_1 * xNPrimeofX;
    xLocal   = 1.0 - xLocal;

    if sign then
        xLocal = 1.0 - xLocal;

    return xLocal;
} 

proc BlkSchlsEqEuroNoDiv( sptprice : fptype, strike : fptype, rate : fptype,
                         volatility : fptype, time : fptype, otype : bool) : fptype
{
	// local private working variables for the calculation
	var OptionPrice  : fptype ;
	var xD1 = rate + volatility * volatility * 0.5;
	xD1 *= time;
	xD1 += log( sptprice / strike );

	var xDen = volatility * sqrt(time);
	xD1 /= xDen;
	var xD2 = xD1 -  xDen;

	var NofXd1 = CNDF( xD1 );
	var NofXd2 = CNDF( xD2 );

	var FutureValueX = strike * ( exp( -(rate)*(time) ) );        
	if (otype == false) {            
		OptionPrice = (sptprice * NofXd1) - (FutureValueX * NofXd2);
	} else { 
		var NegNofXd1 = (1.0 - NofXd1);
		var NegNofXd2 = (1.0 - NofXd2);
		OptionPrice = (FutureValueX * NegNofXd2) - (sptprice * NegNofXd1);
	}

	return OptionPrice;
}

proc errChk(i, refval, price) : bool {
	var priceDelta = refval - price;
	if (abs(priceDelta) >= 1e-4) then {
				    writeln("Error on ",i,". Computed=",price,
						    ", Ref=",data(i).DGrefval,
						    ", Delta=",priceDelta);
				    return true;
	}
	else return false;
}

proc bs() {
	for 0..#NUM_RUNS do {
		/* Calling main function to calculate option value based on 
		 * Black & Sholes's equation.
		 */
		prices = BlkSchlsEqEuroNoDiv( data.s, data.strike,
				data.r, data.v, data.t, otype);
	}
	if ERR_CHK then
		numError = + reduce [i in Dom] errChk(i, data(i).DGrefval, prices(i));
}

proc main() {
	var infile = open(filename, iomode.r).reader();

	// Given the input file, there are 1000 options. Manually read the input in.
	for i in 0..#numOptions do {
		data(i) = new OptionData();
		if ((i % 1000 == 0) && (i > 999)) then {
			infile.close();
			infile = open(filename, iomode.r).reader();
		}

		infile.read(data(i).s, data(i).strike, data(i).r, data(i).divq, 
				data(i).v, data(i).t, data(i).OptionType,
				data(i).divs, data(i).DGrefval);

		otype(i) = (data(i).OptionType == "P");
	}

	writeln("Num of Options :",numOptions);
	writeln("Num of Runs :", NUM_RUNS);

	if timer then
		t.start();

	bs();
        if timer then {
		t.stop();
		writeln("Time elapsed : ", t.elapsed(), " seconds");
	}
	if ERR_CHK then
		writeln("Num Errors: ", numError);
}
