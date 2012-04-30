// StarPU --- Runtime system for heterogeneous multicore architectures.
//
// Copyright (C) 2011 Institut National de Recherche en Informatique et Automatique
//
// StarPU is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.
//
// StarPU is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// See the GNU Lesser General Public License in COPYING.LGPL for more details.

// $ spatch -sp_file cpu_func_to_cpu_funcs.cocci cpu_func_to_cpu_funcs_test.c

///////////////////////////////////////////////////////////////////////////////
// There is no need to specify STARPU_MULTIPLE_CPU_IMPLEMENTATIONS any more. //
// XXX : We must find a way to make sure cpu_funcs is NULL-terminated.       //
///////////////////////////////////////////////////////////////////////////////
@@
identifier cl;
@@
struct starpu_codelet cl = {
-	.cpu_func = STARPU_MULTIPLE_CPU_IMPLEMENTATIONS,
	.cpu_funcs = { ...
- }
+ ,NULL}
};


////////////////////////////////////////////////////////////////////////////////
// The previous rule may have added "NULL" twice in the initialization of the //
// cpu_funcs field. Let's take care of that.                                  //
////////////////////////////////////////////////////////////////////////////////
@@
identifier cl;
@@
struct starpu_codelet cl = {
	.cpu_funcs = { ..., NULL
- ,NULL
}
};


////////////////////////////////////////////////////
// Find all the codelets using the cpu_func field //
////////////////////////////////////////////////////
@cpu_func@
expression f;
identifier cl;
@@
struct starpu_codelet cl = {
	.cpu_func = f
};



///////////////////////////////////////////////////////////////////
// Find all the codelets that define both cpu_func and cpu_funcs //
///////////////////////////////////////////////////////////////////
@cpu_funcs depends on cpu_func@
expression cpu_func.f;
identifier cpu_func.cl;
@@
struct starpu_codelet cl = {
	.cpu_funcs = { f, NULL }
};


//////////////////////////////////////////////////////////////////////////////
// For codelets that define cpu_func but not cpu_funcs, remove cpu_func and //
// add cpu_funcs                                                            //
//////////////////////////////////////////////////////////////////////////////
@depends on !cpu_funcs@
identifier cpu_func.cl;
expression cpu_func.f;
@@
struct starpu_codelet cl = {
-	.cpu_func = f
+	.cpu_funcs = { f, NULL }
};


/////////////////////////////////////////////////////////////////
// If both cpu_func and cpu_funcs are defined, remove cpu_func //
/////////////////////////////////////////////////////////////////
@depends on cpu_funcs@
identifier cpu_func.cl;
expression cpu_func.f;
@@
struct starpu_codelet cl = {
-	.cpu_func = f
};
