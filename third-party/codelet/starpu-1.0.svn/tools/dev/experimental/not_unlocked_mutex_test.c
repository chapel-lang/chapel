/*
 * StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2012 inria
 *
 * StarPU is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * StarPU is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU Lesser General Public License in COPYING.LGPL for more details.
 */
static pthread_mutex_t m;

int 
bad_0(void)
{
        blah();
        _STARPU_PTHREAD_MUTEX_LOCK(&m);

        if (err)
                return 1;

        if (err2)
                return 2;

        if (err3)
        {   
                lol();
                return 3;
        }   

        _STARPU_PTHREAD_MUTEX_UNLOCK(&m);
}

int
good_0(void)
{
        _STARPU_PTHREAD_MUTEX_lock(&m);

        if (brol)
        {   
                _STARPU_PTHREAD_MUTEX_unlock(&m);
                return;
        }   

        _STARPU_PTHREAD_MUTEX_unlock(&m);
}

