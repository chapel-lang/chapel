config param version:string;

for (c, i) in zip(version.codepoints(), 0..<version.size) {
    writeln("codepoint:", c, ", iteration:", i);
}
