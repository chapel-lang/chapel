/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010-2011  Université de Bordeaux 1
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

#include "heat.h"

#ifdef STARPU_OPENGL_RENDER
/*
 * Just some dummy OpenGL code to display our results 
 *
 */

static float minval, maxval;

static unsigned ntheta;
static unsigned nthick;
static float *result;
static unsigned printmesh =0;
static point *pmesh;


float xmin, xmax, ymin, ymax;
float xcenter, ycenter;

static void generate_graph(void)
{
	unsigned theta, thick;

	for (theta = 0; theta < ntheta-1; theta++)
	{
		for (thick = 0; thick < nthick-1; thick++)
		{
			unsigned nodeA = NODE_NUMBER(theta, thick);
			unsigned nodeB = NODE_NUMBER(theta, thick+1);
			unsigned nodeC = NODE_NUMBER(theta+1, thick+1);
			unsigned nodeD = NODE_NUMBER(theta+1, thick);

			float colorA_R, colorB_R, colorC_R, colorD_R;
			float colorA_G, colorB_G, colorC_G, colorD_G;
			float colorA_B, colorB_B, colorC_B, colorD_B;

			if (maxval == minval)
			{
				colorA_R = 1.0f; colorA_G = 1.0f; colorA_B = 1.0f;
				colorB_R = 1.0f; colorB_G = 1.0f; colorB_B = 1.0f;
				colorC_R = 1.0f; colorC_G = 1.0f; colorC_B = 1.0f;
				colorD_R = 1.0f; colorD_G = 1.0f; colorD_B = 1.0f;
			}
			else
			{
				float amplitude = maxval - minval;

				float coeffA, coeffB, coeffC, coeffD;

				coeffA = (result[nodeA] - minval)/amplitude;
				coeffB = (result[nodeB] - minval)/amplitude;
				coeffC = (result[nodeC] - minval)/amplitude;
				coeffD = (result[nodeD] - minval)/amplitude;

				colorA_R = coeffA>0.5f?1.0f:(2.0*coeffA)*1.0f; 
				colorB_R = coeffB>0.5f?1.0f:(2.0*coeffB)*1.0f; 
				colorC_R = coeffC>0.5f?1.0f:(2.0*coeffC)*1.0f; 
				colorD_R = coeffD>0.5f?1.0f:(2.0*coeffD)*1.0f; 

				colorA_B = 0.0f; 
				colorB_B = 0.0f; 
				colorC_B = 0.0f; 
				colorD_B = 0.0f; 

				colorA_G = coeffA<0.5f?1.0f:2.0*(1 - coeffA)*1.0f;
				colorB_G = coeffB<0.5f?1.0f:2.0*(1 - coeffB)*1.0f;
				colorC_G = coeffC<0.5f?1.0f:2.0*(1 - coeffC)*1.0f;
				colorD_G = coeffD<0.5f?1.0f:2.0*(1 - coeffD)*1.0f;
			}

			if (printmesh)
			{
				glColor3f (0.0f, 0.0f, 0.0f);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glLineWidth(3.0f);
				glBegin(GL_POLYGON);
				glVertex3f(pmesh[nodeA].x, pmesh[nodeA].y, 2.0f);
				glVertex3f(pmesh[nodeD].x, pmesh[nodeD].y, 2.0f);
				glVertex3f(pmesh[nodeC].x, pmesh[nodeC].y, 2.0f);
				glVertex3f(pmesh[nodeA].x, pmesh[nodeA].y, 2.0f);
				glEnd();

				glBegin(GL_POLYGON);
				glVertex3f(pmesh[nodeA].x, pmesh[nodeA].y, 1.0f);
				glVertex3f(pmesh[nodeC].x, pmesh[nodeC].y, 1.0f);
				glVertex3f(pmesh[nodeB].x, pmesh[nodeB].y, 1.0f);
				glVertex3f(pmesh[nodeA].x, pmesh[nodeA].y, 1.0f);
				glEnd();
			}

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBegin(GL_POLYGON);
   				glColor3f (colorA_R, colorA_G, colorA_B);
				glVertex3f(pmesh[nodeA].x, pmesh[nodeA].y, 0.0f);
   				glColor3f (colorD_R, colorD_G, colorD_B);
				glVertex3f(pmesh[nodeD].x, pmesh[nodeD].y, 0.0f);
   				glColor3f (colorC_R, colorC_G, colorC_B);
				glVertex3f(pmesh[nodeC].x, pmesh[nodeC].y, 0.0f);
			glEnd();

			glBegin(GL_POLYGON);
   				glColor3f (colorA_R, colorA_G, colorA_B);
				glVertex3f(pmesh[nodeA].x, pmesh[nodeA].y, 0.0f);
   				glColor3f (colorC_R, colorC_G, colorC_B);
				glVertex3f(pmesh[nodeC].x, pmesh[nodeC].y, 0.0f);
   				glColor3f (colorB_R, colorB_G, colorB_B);
				glVertex3f(pmesh[nodeB].x, pmesh[nodeB].y, 0.0f);
			glEnd();
		}
	}
}

static void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	glLoadIdentity ();             /* clear the matrix */
	float amplitude = STARPU_MAX(xmax - xmin, ymax - ymin);
	float factor = 1.0/amplitude;
	glScalef (factor, factor, factor);      /* modeling transformation */
	gluLookAt (xcenter, ycenter, 30.0f, xcenter, ycenter, 0.0f, 0.0f, 1.0f, 0.0f);
/*	printf("factor %f\n", factor);
	   glRotatef(-0,0.0,0.0,0.0); */
	generate_graph();
	glFlush ();
}


static void pressKey(unsigned char key, int x __attribute__ ((unused)), int y  __attribute__ ((unused)))
{
	switch (key)
	{
		case 'q':
			exit(0);
		default:
			printmesh = !printmesh;
			display();
			break;
	}
}



static void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glFrustum (xmin, xmax, ymin, ymax, 5.0f, 5.0f);
	glMatrixMode (GL_MODELVIEW);
}


void find_limits(void)
{
	minval = 100000000.0f;
	maxval = -10000000.0f;

	unsigned i;
	for (i = 0; i < DIM; i++)
	{
		/* find min */
		minval = STARPU_MIN(result[i], minval);

		/* find max */
		maxval = STARPU_MAX(result[i], maxval);
	}

	xmin = 10000000.0f;
	xmax = -10000000.0f;
	ymin = 10000000.0f;
	ymax = -10000000.0f;

	unsigned theta, thick;
	for (theta = 0; theta < ntheta; theta++)
	{
		for (thick = 0; thick < nthick; thick++)
		{
			point *p = &pmesh[NODE_NUMBER(theta, thick)];

			if (p->x < xmin)
				xmin = p->x;

			if (p->x > xmax)
				xmax = p->x;

			if (p->y < ymin)
				ymin = p->y;

			if (p->y > ymax)
				ymax = p->y;
		}
	}
	
	ycenter = (ymin + ymax)/2;
	xcenter = (xmin + xmax)/2;
}

void opengl_render(unsigned _ntheta, unsigned _nthick, float *_result, point *_pmesh, int argc_, char **argv_)
{
	FPRINTF(stderr, "OpenGL rendering ... \n");

	ntheta = _ntheta;
	nthick = _nthick;
	result = _result;
	printmesh = 0;
	pmesh = _pmesh;

	find_limits();

	glutInit(&argc_, argv_);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (800, 800);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Temperature");

	/* init */
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_MODELVIEW);

	glutKeyboardFunc(pressKey);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
}
#endif /* STARPU_OPENGL_RENDER */
