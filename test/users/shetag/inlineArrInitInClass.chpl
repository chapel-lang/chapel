class A {
    const k;
    const arrD = [0..k-1];
    const arrA: [i in arrD] int = i*2;
}

def main() {
    const a = new A(6);
    writeln(a);
}
