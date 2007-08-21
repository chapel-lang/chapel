/** Interface for one-dimensional analytic functions f(x)
 */
class Fn1d {
    def f(x: real): real {
        halt("Fn1d.f() is abstract and should not be called directly");
        return 0.0;
    }
}
