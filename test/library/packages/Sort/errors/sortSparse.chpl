use Sort;

proc main() {
    var start : int(64) = 0;
    var end : int(64) = 1 << 40;
    var ALL : domain(1) = {start..end};
    var D : sparse subdomain(ALL);
    D.add(0);
    D.add(10);
    var values : [D] real;
    values.IRV = 5.0;
    values[0] = 0.0;
    values[10] = 10.0;
    sort(values);
}