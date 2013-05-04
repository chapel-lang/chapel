config const printOutput=false;
config const n: int=10;

var D1={4..5 by 1,6..8 by 1};
var D2={4..5 by 1,6..8 by 1};
if printOutput {
    writeln("Example 1) A",D1 ," = B",D2);
}
call();

D1={4..5,4..8};
D2={4..5,3..7};
if printOutput {
    writeln("Example 2) A",D1 ," = B",D2);
}
call();

D1={4..5,4..8};
D2={6..7,4..8};
if printOutput {
    writeln("Example 3) A",D1 ," = B",D2);
}
call();

D1={4..5,4..8};
D1={6..7,3..7};
if printOutput {
    writeln("Example 4) A",D1 ," = B",D2);
}
call();

D1={4..5,4..8};
D1={1..9 by 8,3..7};
if printOutput {
    writeln("Example 5) A",D1 ," = B",D2);
}
call();

proc call()
{
    on Locales(1)
    {
        var ADR: [1..n,1..n] real;
        var BDR: [1..n,1..n] real;
  
        on Locales(2)
        {
            var a,b:real;
            var i:int;
            for (a,i) in zip(ADR,{1..n*n}) do a=i;
            for (b,i) in zip(BDR,{1..n*n}) do b=i+100.0;          
        
            ADR[D1] = BDR[D2];
            
            for (a,b) in zip(ADR[D1],BDR[D2]) do if (a!=b) then writeln("ERROR!!!!");
            //writeln("ADR: ",ADR);
            //writeln("BDR: ",BDR);
        }
    }
}