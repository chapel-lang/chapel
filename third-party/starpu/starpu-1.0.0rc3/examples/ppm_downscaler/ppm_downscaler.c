/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
 * Copyright (C) 2010, 2011  Centre National de la Recherche Scientifique
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

#include "ppm_downscaler.h"

#include <starpu.h>

#ifdef STARPU_HAVE_MALLOC_H
#include <malloc.h>
#endif
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

struct ppm_image *allocate_new_ppm(int ncols, int nlines, int coldepth)
{
	struct ppm_image *ppm = (struct ppm_image *) malloc(sizeof(struct ppm_image));
	assert(ppm);

	ppm->ncols = ncols;
	ppm->nlines = nlines;
	ppm->coldepth = coldepth;

#ifdef STARPU_HAVE_MEMALIGN
	ppm->data = (struct ppm_color *) memalign(16384, ncols*nlines*sizeof(struct ppm_color));
#else
	ppm->data = (struct ppm_color *) malloc(ncols*nlines*sizeof(struct ppm_color));
#endif
	assert(ppm->data);

	return ppm;
}

struct ppm_image *file_to_ppm(char *filename)
{
	int ret;

	struct ppm_image *ppm = (struct ppm_image *) malloc(sizeof(struct ppm_image));
	assert(ppm);
	
	FILE *file = fopen(filename, "r");
	assert(file);

	/* read the file's dimensions */
	ret = fscanf(file, "P6\n%d %d\n%d\n", &ppm->ncols, &ppm->nlines, &ppm->coldepth);
	if (ret != 3)
	{
		fclose(file);
		fprintf(stderr, "file %s is not valid\n", filename);
		exit(-1);
	}
	
	/* allocate a buffer for the image */
#ifdef STARPU_HAVE_MEMALIGN
	ppm->data = (struct ppm_color *) memalign(16384, ppm->ncols*ppm->nlines*sizeof(struct ppm_color));
#else
	ppm->data = (struct ppm_color *) malloc(ppm->ncols*ppm->nlines*sizeof(struct ppm_color));
#endif
	assert(ppm->data);

	ret = fread(ppm->data, sizeof(struct ppm_color), ppm->ncols*ppm->nlines, file);
	STARPU_ASSERT(ret == ppm->ncols*ppm->nlines);

	unsigned i;
	for (i = 0; i < ppm->ncols*ppm->nlines; i++)
	{
/*		fprintf(stderr, "READ (index %d) -> r %d g %d b %d\n", i, ppm->data[i].r, ppm->data[i].g, ppm->data[i].b); */
	}

	fclose(file);

	return ppm;
}

void ppm_to_file(struct ppm_image *ppm, char *filename)
{
	FILE *file = fopen(filename, "w+");
	assert(file);

	/* read the file's dimensions */
	fprintf(file, "P6\n%d %d\n%d\n", ppm->ncols, ppm->nlines, ppm->coldepth);

	fwrite(&ppm->data[0], sizeof(struct ppm_color), ppm->ncols*ppm->nlines, file);
	
	fclose(file);
}



char *filename_in = "serpents.ppm";
char *filename_out = "serpents.small.ppm";

void parse_args(int argc, char **argv)
{
	if (argc == 3)
	{
		filename_in = argv[1];
		filename_out = argv[2];
	}
}

/* what is the downscaling factor ? */
#define FACTOR	2

void dummy_downscale(struct ppm_image *input_ppm, struct ppm_image *output_ppm)
{
	struct ppm_color *in = input_ppm->data;
	struct ppm_color *out = output_ppm->data;

	unsigned line, col;
	for (line = 0; line < output_ppm->nlines; line++)
	{
		for (col = 0; col < output_ppm->ncols; col++)
		{
			unsigned sum_r = 0, sum_g = 0, sum_b = 0;

			unsigned big_col = col*FACTOR;
			unsigned big_line = line*FACTOR;
			
			/* compute the average value of all components */
			unsigned i, j;
			for (i = 0; i < FACTOR; i++)
			{
				for (j = 0; j < FACTOR; j++)
				{
					unsigned index = (big_col + i)+(big_line + j)*input_ppm->ncols;

/*					fprintf(stderr, "(col %d, line %d) i %d j %d index %d -> r %d g %d b %d\n", col, line, i, j, index, in[index].r, in[index].g, in[index].b); */

					sum_r += (unsigned)in[index].r;
					sum_g += (unsigned)in[index].g;
					sum_b += (unsigned)in[index].b;
				}
			}

			out[col + line*output_ppm->ncols].r = (unsigned char)(sum_r/(FACTOR*FACTOR));
			out[col + line*output_ppm->ncols].g = (unsigned char)(sum_g/(FACTOR*FACTOR));
			out[col + line*output_ppm->ncols].b = (unsigned char)(sum_b/(FACTOR*FACTOR));

/*			fprintf(stderr, "col %d line %d -> sum_r = %d out -> %d\n", col, line, sum_r, out[col + line*FACTOR].r); */
	
		}
	}
}

int main(int argc, char **argv)
{
	struct ppm_image *input_ppm, *output_ppm;

	parse_args(argc, argv);

	input_ppm = file_to_ppm(filename_in);

	fprintf(stderr, "Read input ppm file : ncols = %d, nlines = %d, coldept = %d\n",
		input_ppm->nlines, input_ppm->ncols, input_ppm->coldepth);

	assert(input_ppm->nlines % FACTOR == 0);
	assert(input_ppm->ncols % FACTOR == 0);

	output_ppm = allocate_new_ppm(input_ppm->ncols/FACTOR, input_ppm->nlines/FACTOR, input_ppm->coldepth);

	dummy_downscale(input_ppm, output_ppm);

	ppm_to_file(output_ppm, filename_out);

	return 0;
}
