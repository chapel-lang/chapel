/** Test function for Function1d class
 */

public final class MadFn1d{

    // 1-dimensional f(x) test functions
    
    /** gaussian with square normalized to 1 */
    public static Fn1d test1 = new Fn1d(){
            public double f(double x){
                double a = 500.0;
                return Math.pow(2*a/Math.PI,0.25)*Math.exp(-a*Math.pow(x-0.5,2.0));
            }
        };

    /** derivative of test1 */
    public static Fn1d dtest1 = new Fn1d(){
            public double f(double x){
                double a = 500.0;
                return -2.0*a*(x-0.5)*Math.pow(2*a/Math.PI,0.25)*Math.exp(-a*Math.pow(x-0.5,2.0));
            }
        };
    
    /** superposition of multiple gaussians */
    public static Fn1d test2 = new Fn1d(){
            public double f(double x){
                return test1.f(x-0.3) + test1.f(x)  + test1.f(x+0.3);
            }
        };
    
    /** derivative of test2 */
    public static Fn1d dtest2 = new Fn1d(){
            public double f(double x){
                return dtest1.f(x-0.3) + dtest1.f(x)  + dtest1.f(x+0.3);
            }
        };

     /** a more interesting (singular and oscillating) function
         ... note it is never computed exactly at the singularity except by
         the test code below.
     */
    public static Fn1d test3 = new Fn1d(){
            public double f(double x){
                double a = 100.0*Math.PI;
                if(x == 0.5)
                    return 0.0;
                else
                    return Math.cos(a*(x-0.5))/Math.sqrt(Math.abs(x-0.5));
            }
        };
    
    /** derivative of test3 */
    public static Fn1d dtest3 = new Fn1d(){
            public double f(double x){
                double a = 100.0*Math.PI;
                if(x == 0.5)
                    return 0.0;
                else
                {
                    double s = 1.0;
                    if(x < 0.5) s = -1.0;
                    return -a*Math.sin(a*(x-0.5))/Math.sqrt(Math.abs(x-0.5))
                        - s*0.5*Math.cos(a*(x-0.5))/Math.pow(Math.abs(x-0.5),1.5);
                }
            }
        };


    
}
