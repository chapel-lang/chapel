function binsearch(x : [?lo..?hi] , y]) {
         if(hi < lo ) return lo;
         if(x(hi) > y) return hi;
         if(y <= x(lo) return lo;
         while(lo+1 < hi) {
              assert x(lo) < y and y <= x(hi) ;
              var mid = (hi+lo)/2;
              if(x[mid] < y)
                lo = mid;
              else
                hi = mid;
         }
         return hi;
}

var d : domain(1);
d = 1..5;
var a[d] : integer;

a[1] = 1;
a[2] = 2;
a[3] = 3;
a[4] = 4;
a[5] = 5;

writeln("expect 4 ", binsearch(a, 4));
