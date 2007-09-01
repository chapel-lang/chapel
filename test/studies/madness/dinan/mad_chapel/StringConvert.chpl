_extern def snprintf(buf: string, size: int, fmt: string, vals...?numvals): string;

def toString(fmt: string, x) {
    var buf: string = "1234567890123456789012";
    snprintf(buf, 32, fmt, x);
    return buf;
}
