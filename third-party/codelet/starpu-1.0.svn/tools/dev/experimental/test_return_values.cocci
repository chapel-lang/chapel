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

// I have no idea in Hell why we need "<... ...>", but this will not work with
// "... ..."
@@
@@
main(...)
{
<...
-	return 0;
+	return EXIT_SUCCESS;
...>
}


//
// Is (..)*/common/helper.h included ?
// XXX : OK, that suxx, but it should work. Is there a way to use a regular
// expression to match a header ?
@helper_included@
@@
(
#include "common/helper.h"
|
#include "../common/helper.h"
|
#include "../../common/helper.h"
|
#include "../../../common/helper.h"
)


@depends on helper_included@
@@
main(...)
{
...
-	return 77;
+	return STARPU_TEST_SKIPPED;
...
}
