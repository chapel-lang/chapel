phrase : words ( ABC | AB | A ) words;
words : word*;
word : "[a-z]+";
A : "a[a-z]*";
AB : A "b[a-z]*";
ABC : AB "c[a-z]*";
