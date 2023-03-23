
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
          mul(q3, q, 3);
          add(q3AndR, q3,r);
          divQ(q3AndRDivT, q3AndR,t);
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
            mul(doubleQ, q,2);
            qMultiplicator = k;
            qCalculating.write(true);

            add(doubleQAndR, doubleQ,r);
            mul(r, doubleQAndR,doubleK);

            qCalculating.waitFor(false); // Wait for 'q *= qMultiplicator' to finish.
            tCalculating.waitFor(false); // Wait for 't *= doubleK' to finish (if it runs).
            if (q <= r) {
                extractCalculating.write(true);
                mul(q4, q,4);
                add(q4AndR, q4,r);
                divQ(q4AndRDivT, q4AndR,t);
                var digit2 = q4AndRDivT:int;
                extractCalculating.waitFor(false); // Wait for 'digit = (q * 3 + r) / t' to finish.

                if (digit == digit2) {
                    qMultiplicator = 10;
                    qCalculating.write(true);
                    write(digit);
                    mul(tDig, t,digit);
                    k += 1;
                    doubleK = 2 * k + 1;
                    tCalculating.write(true);
                    tPrecalculation = true;
                    sub(rSubTDig, r,tDig);
                    mul(r, rSubTDig,10);
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








