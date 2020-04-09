/** Interface for one-dimensional analytic functions f(x)
 */
class AFcn {
    proc this(x: real): real {
        halt("Fn1d.f() is abstract and should not be called directly");
        return 0.0;
    }
}
