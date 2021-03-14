#include <stdio.h>

main() {

  #define NUMOPS 16

  /* Variable declarations */
  int i = 0;
  int j = 0;

  char allOps[NUMOPS][3]= {
    "*",
    "/",
    "%",
    "+", 
    "-", 
    "<", 
    "<=", 
    ">",
    ">=",
    "==", 
    "!=", 
    "&", 
    "^", 
    "|", 
    "&&", 
    "||" 
  };

  // This file generates either a C or Chpl test, dependent on this flag.
  int thisIsC = 0;

  if (thisIsC) {
    // Print start of C file.
    printf("#include <stdio.h>\n\n");
    printf("main() {\n\n");
    
    // Print variables for C file.
    printf("  int m = 2;\n");
    printf("  int n = 3;\n");
    printf("  int o = 5;\n");
    printf("  int answer = 0;\n\n");

  } else {
    // Print variables for CHPL file.
    printf("var m = 2;\n");
    printf("var n = 3;\n");
    printf("var o = 5;\n");
    printf("var answer = 0;\n\n");
  }
  
  // Print all the tests.
  for (i = 0; i < NUMOPS; i++) {
    for (j = 0; j < NUMOPS; j++) {

      if (thisIsC) {
	printf("  printf(\"");
      } else {
	printf("write(\"");
      }

      if (strcmp(allOps[i], "%") == 0) {
	if (thisIsC) {
	  printf("%%%% and ");
	} else {
	  printf("%% and ");
	}
      } else {
	printf("%s and ", allOps[i]);
      }

      if (strcmp(allOps[j], "%") == 0) {
	if (thisIsC) {
	  printf("%%%% permutations\\n");
	} else {
	  printf("%% permutations\\n");
	}
      } else {
	printf("%s permutations\\n", allOps[j]);
      }
      
      printf("\");\n");

      if (thisIsC) {
	printf("  printf(\"=========================\\n\");\n");
	printf("  ");
      } else {
	printf("writeln(\"=========================\");\n");
      }

      printf("answer = m %s n %s o;\n",   allOps[i], allOps[j]);

      if (thisIsC) {
	printf("  printf(\"%%d\\n\", answer);\n");
	printf("  ");
      } else {
	printf("writeln(answer);\n");
      }

      printf("answer = (m %s n) %s o;\n", allOps[i], allOps[j]);

      if (thisIsC) {
	printf("  printf(\"%%d\\n\", answer);\n");
	printf("  ");
      } else {
	printf("writeln(answer);\n");
      }

      printf("answer = m %s (n %s o);\n", allOps[i], allOps[j]);

      if (thisIsC) {
	printf("  printf(\"%%d\\n\\n\", answer);\n");
	printf("  ");
      } else {
	printf("writeln(answer, \"\\n\");\n");
      }

      printf("\n");

    }
  }

  if (thisIsC) {
    // Print the closing brace of main. 
    printf("}\n");
  }

}
