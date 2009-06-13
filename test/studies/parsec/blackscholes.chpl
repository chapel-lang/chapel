use Time, MultiBlockDist;

config const timer : bool = false;
var t : Timer;

//Precision to use for calculations

type fptype = real;
config const NUM_RUNS = 100;

class OptionData {
	var s : fptype; 	// spot price
	var strike : fptype; 	// strike price
	var r : fptype;		// risk-free interest rate
	var divq : fptype;	// dividend rate
	var v : fptype;		// volatility
	var t : fptype;		// time to maturity or option expiration in years
				// (1yr = 1.0, 6mos = 0.5, 3mos = 0.25, ..., etc)
	const OptionType : string;	// Option Type. "P"=PUT, "C"=CALL
	var divs : fptype;	// dividend vals (not used in this test)
	var DGrefval : fptype;	// DerivaGem Reference Value
}


var numError : int = 0;

config var numOptions : int(64) = 1000;
config const ERR_CHK = false;
config const filename = "optionData.txt";

var Dist = new Block(rank=1, bbox=[0..#numOptions], tasksPerLocale=here.numCores);
var Dom : domain(1, int(64)) distributed Dist = [0..#numOptions];

var data : [Dom] OptionData;
var prices : [Dom] fptype;
var otype : [Dom] bool;
var sptprice : [Dom] fptype;
var strike : [Dom] fptype;
var rate : [Dom] fptype;
var volatility : [Dom] fptype;
var otime : [Dom] fptype;

// Cumulative Normal Distribution Function
// See Hull, Section 11.8, P.243-244
param inv_sqrt_2xPI = 0.39894228040143270286;

def CNDF ( in InputX : fptype )  : fptype
{
    var sign : int;

    var OutputX : fptype;
    var xInput : fptype;
    var xNPrimeofX : fptype;
    var expValues : fptype;
    var xK2 : fptype;
    var xK2_2 : fptype;
    var xK2_3 : fptype;
    var xK2_4 : fptype;
    var xK2_5 : fptype;
    var xLocal: fptype;
    var xLocal_1 : fptype;
    var xLocal_2 : fptype;
    var xLocal_3 : fptype;

    // Check for negative value of InputX
    if (InputX < 0.0) then {
        InputX = -InputX;
        sign = 1;
    } else 
        sign = 0;

    xInput = InputX;
 
    // Compute NPrimeX term common to both four & six decimal accuracy calcs
    expValues = exp(-0.5 * InputX * InputX);
    xNPrimeofX = expValues;
    xNPrimeofX = xNPrimeofX * inv_sqrt_2xPI;

    xK2 = 0.2316419 * xInput;
    xK2 = 1.0 + xK2;
    xK2 = 1.0 / xK2;
    xK2_2 = xK2 * xK2;
    xK2_3 = xK2_2 * xK2;
    xK2_4 = xK2_3 * xK2;
    xK2_5 = xK2_4 * xK2;
    
    xLocal_1 = xK2 * 0.319381530;
    xLocal_2 = xK2_2 * (-0.356563782);
    xLocal_3 = xK2_3 * 1.781477937;
    xLocal_2 = xLocal_2 + xLocal_3;
    xLocal_3 = xK2_4 * (-1.821255978);
    xLocal_2 = xLocal_2 + xLocal_3;
    xLocal_3 = xK2_5 * 1.330274429;
    xLocal_2 = xLocal_2 + xLocal_3;

    xLocal_1 = xLocal_2 + xLocal_1;
    xLocal   = xLocal_1 * xNPrimeofX;
    xLocal   = 1.0 - xLocal;

    OutputX  = xLocal;
    
    if sign then {
        OutputX = 1.0 - OutputX;
    }
    
    return OutputX;
} 

def BlkSchlsEqEuroNoDiv( sptprice : fptype, strike : fptype, rate : fptype,
                         volatility : fptype, time : fptype, otype : bool, 
                         timet) : fptype   
{
	var OptionPrice  : fptype ;

    // local private working variables for the calculation
    var xStockPrice : fptype;
    var xStrikePrice : fptype;
    var xRiskFreeRate : fptype;
    var xVolatility : fptype;
    var xTime : fptype;
    var xSqrtTime : fptype;

    var logValues : fptype;
    var xLogTerm : fptype;
    var xD1 : fptype; 
    var xD2 : fptype;
    var xPowerTerm : fptype;
    var xDen : fptype;
    var d1 : fptype;
    var d2 : fptype;
    var FutureValueX : fptype;
    var NofXd1 : fptype;
    var NofXd2 : fptype;
    var NegNofXd1 : fptype;
    var NegNofXd2 : fptype;
    
    xStockPrice = sptprice;
    xStrikePrice = strike;
    xRiskFreeRate = rate;
    xVolatility = volatility;

    xTime = time;
    xSqrtTime = sqrt(xTime);

    logValues = log( sptprice / strike );
        
    xLogTerm = logValues;
        
    
    xPowerTerm = xVolatility * xVolatility;
    xPowerTerm = xPowerTerm * 0.5;
        
    xD1 = xRiskFreeRate + xPowerTerm;
    xD1 = xD1 * xTime;
    xD1 = xD1 + xLogTerm;

    xDen = xVolatility * xSqrtTime;
    xD1 = xD1 / xDen;
    xD2 = xD1 -  xDen;

    d1 = xD1;
    d2 = xD2;
    
    NofXd1 = CNDF( d1 );
    NofXd2 = CNDF( d2 );

    FutureValueX = strike * ( exp( -(rate)*(time) ) );        
    if (otype == false) {            
        OptionPrice = (sptprice * NofXd1) - (FutureValueX * NofXd2);
    } else { 
        NegNofXd1 = (1.0 - NofXd1);
        NegNofXd2 = (1.0 - NofXd2);
        OptionPrice = (FutureValueX * NegNofXd2) - (sptprice * NegNofXd1);
    }
    
    return OptionPrice;
}

def bs_thread(tid : int) : int {
    for j in 0..#NUM_RUNS do {
	    forall i in [Dom] do {
		    /* Calling main function to calculate option value based on 
		     * Black & Sholes's equation.
		     */
		    var price = BlkSchlsEqEuroNoDiv( sptprice(i), strike(i),
				    rate(i), volatility(i), otime(i), 
				    otype(i), 0);
		    prices(i) = price;

		    if ERR_CHK then {
			    var priceDelta = data(i).DGrefval - price;
			    if( abs(priceDelta) >= 1e-4 ){
				    writeln("Error on ",i,". Computed=",price,
						    ", Ref=",data(i).DGrefval,
						    ", Delta=",priceDelta);
				    numError += 1;
			    }
		    }

        }
    }

    return 0;
}


def main() {
	var i : int;
	var loopnum : int;
	var buffer : fptype;
	var buffer2 : int;
	var initOptionNum : int;

	var infile = new file(filename, FileAccessMode.read);
	infile.open();

	// Given the input file, there are 1000 options. Manually read the input in.
	for i in 0..#numOptions do {
		data(i) = new OptionData();
		if (i % 1000 == 0) then {
			infile.close();
			infile = new file(filename, FileAccessMode.read);
			infile.open();
		}

		infile.read(data(i).s, data(i).strike, data(i).r, data(i).divq, 
				data(i).v, data(i).t, data(i).OptionType,
				data(i).divs, data(i).DGrefval);

		otype(i) = (data(i).OptionType == "P");
		sptprice(i) = data(i).s;
		strike(i) = data(i).strike;
		rate(i) = data(i).r;
		volatility(i) = data(i).v;
		otime(i) = data(i).t;
	}

	writeln("Num of Options :",numOptions);
	writeln("Num of Runs :", NUM_RUNS);

	if timer then
		t.start();

	bs_thread(0);
        if timer then {
		t.stop();
		writeln("Time elapsed : ", t.elapsed(), " seconds");
	}
	if ERR_CHK then
		writeln("Num Errors: ", numError);
}
