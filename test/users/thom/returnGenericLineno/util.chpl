/* Stuff that might be reusable, with no discernable categorisation */
module Util
{

    proc stringMultiply(s : string, m : int) : string
    {
        var retr : string;
        for i in 1..m
        { 
            retr+=s;
        }
        return retr;
    }

    //untested, probably wont compile
    proc tupleToArray(t...?k) : [1..k] int
    {
        var retr : [1..k] int;
        for i in 1..k
        {
            retr(i) = t(i);
        }

        return retr;
    }



















    proc arrayEquals(A : [?D] real, B : [D] real, tolerance : real = 1e-15): bool
    {
        //Check if any array entries do not fall within tolerance
        for ij in D do
            if ( abs( A(ij) - B(ij) ) > tolerance) then
                return false;
       
        //Have failed to find mismatch. Return true;
        return true;
    }
}
