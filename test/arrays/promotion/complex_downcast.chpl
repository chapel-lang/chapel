var A: [0..3] complex(64);
// Note: If rewritten (more correctly) as `A = (1 + 0i): t;`, error does not occur
A = 1 + 0i;

writeln('OK');
