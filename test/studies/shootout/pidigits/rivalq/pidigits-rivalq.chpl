
use BigInteger;

config var n:int = 10000;


var q,t:bigint = 1;
var r,doubleQ,tDig,rSubTDig, doubleKTimesDoubleQ, doubleQAndR:bigint;
var q4,q4AndR,q4AndRDivT:bigint;

var k,doubleK,qMultiplicator,digit:int;

var finish:atomic bool;
var tCalculating:atomic bool;
var qCalculating:atomic bool;
var extractCalculating:atomic bool;
var tPrecalculation: bool;


proc extractThread(){
    var q3,q3AndR,q3AndRDivT:bigint;   
    while(finish.read() == false){
          extractCalculating.waitFor(true);
          if(finish.read() == true){
              return;
          }        
          q3.mul(q , 3);
          q3AndR.add(q3,r);
          q3AndRDivT.divQ(q3AndR,t);
          digit = q3AndRDivT:int;
          extractCalculating.write(false);
    }

}

proc multiplierThread(ref waitCond,ref result,ref multiplicator){
    while(finish.read() == false){
        waitCond.waitFor(true);
        if(finish.read() == true){
            return;
        }
        result *= multiplicator;
        waitCond.write(false);
    }
}

proc main{

    tCalculating.write(false);
    qCalculating.write(false);
    extractCalculating.write(false);
    begin with (ref t,ref doubleK)multiplierThread(tCalculating,t,doubleK);
    begin with (ref q,ref qMultiplicator)multiplierThread(qCalculating,q,qMultiplicator);
    begin extractThread();
    var n_digits = 0;
    while (n_digits < n) {
        var i = 0;
        while (i < 10 && n_digits < n) {
            if (!tPrecalculation) {
                k += 1;
                doubleK = 2 * k + 1;
                tCalculating.write(true);
            } else{
                tPrecalculation = false;
            }
            qCalculating.waitFor(false); // Wait for 'q *= 10' to finish (if it runs).
            doubleQ.mul(q,2);
            qMultiplicator = k;
            qCalculating.write(true);

            doubleQAndR.add(doubleQ,r);
            r.mul(doubleQAndR,doubleK);

            qCalculating.waitFor(false); // Wait for 'q *= qMultiplicator' to finish.
            tCalculating.waitFor(false); // Wait for 't *= doubleK' to finish (if it runs).
            if (q <= r) {
                extractCalculating.write(true);
                q4.mul(q,4);
                q4AndR.add(q4,r);
                q4AndRDivT.divQ(q4AndR,t);
                var digit2 = q4AndRDivT:int;
                extractCalculating.waitFor(false); // Wait for 'digit = (q * 3 + r) / t' to finish.

                if (digit == digit2) {
                    qMultiplicator = 10;
                    qCalculating.write(true);
                    write(digit);
                    tDig.mul(t,digit);
                    k += 1;
                    doubleK = 2 * k + 1;
                    tCalculating.write(true);
                    tPrecalculation = true;
                    rSubTDig.sub(r,tDig);
                    r.mul(rSubTDig,10);
                    i += 1;
                    n_digits += 1;
                }
            }
        }
        var spaces = 10 - n_digits%10;
        if(spaces != 10){
            write(" " * spaces);
        }
        writeln("\t:",n_digits);
    }
    finish.write(true);
    tCalculating.write(true);
    qCalculating.write(true);
    extractCalculating.write(true);

}








