This directory contains vectorization tests. Few things to note:
1. Easiest way to check for occurence of vectorization is to check if vector operation was generated, for example <4 x i32>
2. LLVM uses cost-model to estimate whether it is profitable to vectorize the loop. In order to force vectorization when cost model says it is not worth to do so it is necessary to pass "-force-vector-width=x" "-force-vector-interleave=y". Since we'd check whether vectorization occured using method in 1. it is best to set x,y to some constant (like 4 and 1).
