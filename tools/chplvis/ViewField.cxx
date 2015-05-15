/*
 * Copyright 2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "chplvis.h"
#include "ViewField.h"
#include "math.h"
#include <FL/fl_draw.H>

// Local utility functions

Fl_Color heatColor ( double val, double max ) {
  if (val == 0) return FL_WHITE;
  return fl_rgb_color( 255*((val)/max), 255 * ((max-val)/max), 0);
}

//  ViewField Constructors


ViewField::ViewField (int bx, int by, int bw, int bh, const char *label)
  : Fl_Box (bx, by, bw, bh, 0) 
{
  // printf ("ViewField init. h=%d, w=%d, numlocales is %d\n",bh,bw, VisData.NumLocales());
  numlocales = 0;
  theLocales = NULL;
  comms = NULL;
  tags = NULL;
  tagsSize = 0;
  tagMenu = -1;
  if (VisData.NumLocales() > 0) {
    setNumLocales(VisData.NumLocales());
  }
  maxTasks = 1;
  maxComms = 1;
  maxCpu = 0.000001;
  maxDatasize = 1;
  showtasks = true;
  showcomms = true;
  
};

ViewField::ViewField (Fl_Boxtype b, int bx, int by, int bw, int bh, const char *label)
  : Fl_Box (b, bx, by, bw, bh, 0)
{
  // printf ("ViewField init with boxtype. h=%d, w=%d\n",bh,bw);
  numlocales = 0;
  theLocales = NULL;
  comms = NULL;
  tags = NULL;
  tagsSize = 0;
  tagMenu = -1;
  if (VisData.NumLocales() > 0) {
    setNumLocales(VisData.NumLocales());
  }
  maxTasks = 1;
  maxComms = 1;
  maxCpu = 0.000001;
  maxDatasize = 1;
  showtasks = true;
  showcomms = true;
};


// Private methods 

void ViewField::allocArrays()
{
  int ix;
  int ix2;

  //printf ("allocArrays\n");
  // Dealloc anything current
  if (theLocales != NULL) {
    for (ix = 0; ix < getSize; ix++)
      if (theLocales[ix].win != NULL) {
	delete theLocales[ix].win;
	//printf ("deleting win for %d\n", ix);
      }
    delete [] theLocales;
  }
  if (comms != NULL) {
    for (ix = 0; ix < getSize; ix++)  delete [] comms[ix];
    delete [] comms;
  }
  if (tags != NULL) {
    delete [] tags;
    tags = NULL;
  }
  
  // Alloc new space ... except for tags, done in process data.
  theLocales = new localeInfo [numlocales];
  comms = new  commInfo* [numlocales];
  if (theLocales == NULL || comms == NULL) {
    fprintf (stderr, "chplvis: out of memory.\n");
    exit(1);
  }
  for (ix =0; ix < numlocales; ix++)
    theLocales[ix].numTasks = 0;
  getSize = numlocales;
  for (ix = 0; ix < getSize; ix++) {
    comms[ix] = new commInfo[numlocales];
    if (comms[ix] == NULL) {
      fprintf (stderr, "chplvis: out of memory.\n");
      exit(1);
    }
    for (ix2 = 0; ix2 < numlocales; ix2++) {
      comms[ix][ix2].numGets = 0;
      comms[ix][ix2].commSize = 0;
    }
  }

  for (ix = 0; ix < numlocales; ix++) {
    theLocales[ix].numTasks = 0;
    theLocales[ix].refUserCpu = 0;
    theLocales[ix].refSysCpu = 0;
    theLocales[ix].Cpu = 0;
    theLocales[ix].userCpu = 0;
    theLocales[ix].sysCpu = 0;
    theLocales[ix].win = NULL;
  }

  if (tags != NULL) {
    for (ix = 0; ix < tagsSize; ix++)
      if (tags[ix].tagName != NULL)
	delete [] tags[ix].tagName;
    delete [] tags;
  }
  tagsSize = VisData.NumTags();
  tags = new tagInfo [tagsSize];
  for (ix = 0; ix < VisData.NumTags(); ix++) {
    tags[ix].tagNo = -1;
    tags[ix].tagName = NULL;
  }
}


// Public Methods 

void ViewField::processData()
{
  int ix1, ix2;  // For processing the arrays

  // Initialize ... in case data has changed.
  for (ix1 = 0; ix1 < numlocales; ix1++) {
    theLocales[ix1].numTasks = 0;
    for (ix2 = 0; ix2 < numlocales; ix2++)  {
      comms[ix1][ix2].numGets = 0;
      comms[ix1][ix2].commSize = 0;
    }
  }
  maxTasks = 1;
  maxComms = 1;
  maxCpu = 0.0000001;
  maxDatasize = 1;

  Event *ev;

  for ( ev = VisData.getFirstEvent(); ev != NULL; ev = VisData.getNextEvent() ) {
    E_task   *tp;
    E_comm   *cp;
    E_fork   *fp;
    E_start  *sp;
    E_end    *ep;
    E_tag    *gp;
    E_resume *rp;
    switch (ev->Ekind()) {
      case Ev_task:
        //  Task event
        tp = (E_task *)ev;
	if (++theLocales[tp->nodeId()].numTasks > maxTasks)
	  maxTasks = theLocales[tp->nodeId()].numTasks;
        break;

      case Ev_comm:
        //  Comm event
        cp = (E_comm *)ev;
	if (++(comms[cp->srcId()][cp->dstId()].numGets) > maxComms)
	  maxComms = comms[cp->srcId()][cp->dstId()].numGets;
	comms[cp->srcId()][cp->dstId()].commSize += cp->totalLen();
	if (comms[cp->srcId()][cp->dstId()].commSize > maxDatasize)
	  maxDatasize = comms[cp->srcId()][cp->dstId()].commSize;
        break;

      case Ev_fork:
        fp = (E_fork *)ev;
        if (++(comms[fp->srcId()][fp->dstId()].numGets) > maxComms)
	  maxComms = comms[fp->srcId()][fp->dstId()].numGets;
	comms[fp->srcId()][fp->dstId()].commSize += fp->argSize();
	if (comms[fp->srcId()][fp->dstId()].commSize > maxDatasize)
	  maxDatasize = comms[fp->srcId()][fp->dstId()].commSize;
	if (!fp->fast()) {
	  if (++theLocales[fp->dstId()].numTasks > maxTasks)
	    maxTasks = theLocales[fp->dstId()].numTasks;
	}
	break;

      case Ev_start:
	sp = (E_start *)ev;
	theLocales[sp->nodeId()].refUserCpu = sp->user_time();
	theLocales[sp->nodeId()].refSysCpu = sp->sys_time();
        break;

      case Ev_end:
	ep = (E_end *)ev;
	theLocales[ep->nodeId()].userCpu += ep->user_time()
	  - theLocales[ep->nodeId()].refUserCpu;
	theLocales[ep->nodeId()].sysCpu += ep->sys_time()
	  - theLocales[ep->nodeId()].refSysCpu;
	theLocales[ep->nodeId()].Cpu =	theLocales[ep->nodeId()].userCpu 
	  + theLocales[ep->nodeId()].sysCpu;  
	if (maxCpu < theLocales[ep->nodeId()].Cpu)
	  maxCpu = theLocales[ep->nodeId()].Cpu;
	// printf ("end: node = %d cpu = %f\n",  ep->nodeId(), 
	//	theLocales[ep->nodeId()].Cpu);
	break;

      case Ev_resume:
	// Reset the ref time?
	rp = (E_resume *)ev;
 	theLocales[rp->nodeId()].refUserCpu = rp->user_time();
	theLocales[rp->nodeId()].refSysCpu = rp->sys_time();
	break;
	 
      case Ev_tag:
	// Checking out the tag ...
	gp = (E_tag *)ev;
        int tgNo = gp->tagNo();
        if (tags[tgNo].tagNo < 0) {
	  tags[tgNo].tagNo = tgNo;
	  // printf("processing tag '%s'\n", gp->tagName().c_str());
	  tags[tgNo].tagName = new char [gp->tagName().length()+6];
	  strcpy(tags[tgNo].tagName, "Tags/");
	  strcat(tags[tgNo].tagName, gp->tagName().c_str());
	}
	if (gp->isPause()) {
	  // Need to update times so that resume can reset ref times
	  theLocales[gp->nodeId()].userCpu += gp->user_time()
	    - theLocales[gp->nodeId()].refUserCpu;
	  theLocales[gp->nodeId()].sysCpu += gp->sys_time()
	    - theLocales[gp->nodeId()].refSysCpu;
	  theLocales[gp->nodeId()].Cpu = theLocales[gp->nodeId()].userCpu 
	    + theLocales[gp->nodeId()].sysCpu;
	  //printf ("tag: node = %d cpu = %f\n",  gp->nodeId(), 
	  //         theLocales[gp->nodeId()].Cpu);

	}
	break;
    }
  }

  Info->setMaxes(maxTasks, maxComms, maxDatasize, maxCpu);
  //printf ("maxTasks %d, maxComms %d\n", maxTasks, maxComms);

  makeTagsMenu();

 }

static void selTag(Fl_Widget *w, void *p)
{
  long ix = (long) p;
  printf ("selTag called, %ld\n", ix);
}

void ViewField::makeTagsMenu(void)
{
  // Remove the old one if it exists
  if (tagMenu > 0) {
    MainMenuBar->remove(tagMenu);
    tagMenu = -1;
  }
  if (VisData.NumTags() >= 1) {

    //printf("Make tags menu, %d tags\n", VisData.NumTags());

    // Build the menu

    tagMenu = MainMenuBar->add("Tags", 0, 0, 0, FL_SUBMENU);
    if (tagMenu < 1) {
      printf ("Menu problem!\n");
      return;
    }
    MainMenuBar->add("Tags/All", 0, selTag, (void *)-2);
    MainMenuBar->add("Tags/Start", 0, selTag, (void *)-1);

    long ix;
    for (ix = 0; ix < VisData.NumTags(); ix++) {
      // printf ("Tag[%d] is '%s'\n", ix, tags[ix].tagName);
      MainMenuBar->add(tags[ix].tagName, 0, selTag, (void *)&tags[ix], 0);
    }
    MainMenuBar->redraw();
  }
}


void ViewField::processTag(int n)
{
  
}


void ViewField::drawLocale ( int ix, Fl_Color col)
{
  //printf ("drawLocale %d\n", ix);
    char ixchars[10];
    if (theLocales != NULL) {
      localeInfo *loc = &theLocales[ix];
      
      // Draw a locale ...
      loc->x = cx + (int) rint(rx * sin (angle * ix - start));
      loc->y = cy - (int) rint(ry * cos (angle * ix - start));
      snprintf (ixchars, 10, "%d", ix);
      loc->w = 30;
      loc->h = 30;
      
      fl_color(col);
      fl_rectf(loc->x-loc->w/2, loc->y-loc->h/2, loc->w, loc->h);
      fl_color(FL_BLACK);
      fl_rect(loc->x-loc->w/2, loc->y-loc->h/2, loc->w, loc->h);
      fl_draw(ixchars, loc->x+3-loc->w/2, loc->y+3-loc->h/2,
	      loc->w-6, loc->h-6, FL_ALIGN_CENTER, NULL, 0);
    }
}


void ViewField::drawCommLine (int ix1, Fl_Color col1,  int ix2, Fl_Color col2)
{
  int x1, x2, y1, y2, dx, dy, midx, midy;
  if (ix1 < 0 || ix1 >= numlocales || ix2 < 0 || ix2 >= numlocales) {
    fprintf (stderr, "DATA ERROR: drawCommLine\n");
    return;
  }
  localeInfo *loc1 = &theLocales[ix1], *loc2 = &theLocales[ix2];
  double theta, cw, ch;
  
  dx = loc2->x - loc1->x;
  dy = loc2->y - loc1->y;
  if (dx == 0) {
    theta = (dy < 0 ? pi / 2 : pi * 1.5);
  } else {
    if (dx < 0) 
      theta = pi + atan((double)dy/(double)dx);
    else
      theta = atan((double)dy/(double)dx);
  }
  cw = loc1->w / sin(pi/4);
  ch = loc1->h / sin(pi/4);
  //printf ("dx = %d, dy = %d, Theta = %lf\n", dx, dy, theta);
  
  //  Need correct calculations here.
  x1 = rint (cw/2 * cos(theta));
  x1 = loc1->x + (abs(x1) > loc1->w/2 ? (dx < 0 ? -1 : 1)*loc1->w/2 : x1);
  y1 = rint (ch/2 * sin(theta));
  y1 = loc1->y + (abs(y1) > loc1->h/2 ? (dy < 0 ? -1 : 1)*loc1->h/2 : y1);

  x2 = rint (cw/2 * cos(pi+theta));
  x2 = loc2->x + (abs(x2) > loc2->w/2 ? (dx < 0 ? 1 : -1)*loc2->w/2 : x2);
  y2 = rint (ch/2 * sin(pi+theta));
  y2 = loc2->y + (abs(y2) > loc2->h/2 ? (dy < 0 ? 1 : -1)*loc2->h/2 : y2);

  midx = loc1->x + dx/2;
  midy = loc1->y + dy/2;

  if (col1 != FL_WHITE) {
    fl_color(col1);
    fl_line_style(FL_SOLID, 3, NULL);
    fl_line(x1,y1,midx,midy);
  }
  if (col2 != FL_WHITE) {
    fl_color(col2);
    fl_line_style(FL_SOLID, 3, NULL);
    fl_line(midx,midy,x2,y2);
  }
  fl_line_style(FL_SOLID, 1, NULL);
}

void ViewField::draw()
{
  //printf ("ViewField draw, numlocales is %d\n", numlocales);

  cx = x() + w()/2;
  cy = y() + h()/2;
  rx = cx * 0.70;
  ry = cy * 0.70;

  Fl_Box::draw();

  int ix;
  for (ix = 0; ix < numlocales; ix++) {
    if (showtasks) {
      drawLocale(ix, heatColor(theLocales[ix].numTasks, maxTasks));
    } else {
      drawLocale(ix, heatColor(theLocales[ix].Cpu, maxCpu));
    }
  }

  int iy;
  for (ix = 0; ix < numlocales-1; ix++) {
    for (iy = ix + 1; iy < numlocales; iy++) {
      int  com2ix, com2iy, comMax; 
      if (showcomms) {
	com2ix = comms[ix][iy].numGets;
	com2iy = comms[iy][ix].numGets;
	comMax = maxComms;
      } else {
	com2ix = comms[ix][iy].commSize;
	com2iy = comms[iy][ix].commSize;
	comMax = maxDatasize;
      }
      if (com2ix || com2iy) {
	// Draw a line ... gray if no communication
        if (!com2ix || !com2iy) {
	  drawCommLine(ix, com2ix ? heatColor(com2ix, comMax) : FL_GRAY,
		       iy, com2iy ? heatColor(com2iy, comMax) : FL_GRAY);
	} else {
	  drawCommLine(ix, heatColor(com2ix, comMax),
		       iy, heatColor(com2iy, comMax));
	}
      }
    }
  }
  // Set up the info widget correctly
  if (showcomms)
    Info->showComms();
  else 
    Info->showSize();
  if (showtasks)
      Info->showTasks();
  else
      Info->showCpu();
  Info->draw();
}

int ViewField::handle(int event)
{
  int ix;

  int x = Fl::event_x();
  int y = Fl::event_y();
  
  switch (event) {
  case FL_PUSH:
    //printf ("Push at (%d,%d)\n", x, y);
    break;
  case FL_RELEASE:
    //printf ("Release at (%d,%d)\n", x, y);
    if (numlocales > 0) {
      for (ix = 0; ix < numlocales; ix++) {
	// See if release is inside a locale
	localeInfo *loc = &theLocales[ix];
	if ( x > loc->x-loc->w/2 && x <= loc->x + loc->w/2 &&
	     y > loc->y-loc->h/2 && y <= loc->y + loc->h/2) {
	  if (theLocales[ix].win == NULL) {
	    // Create the window
	    theLocales[ix].win = make_locale_window(ix, &theLocales[ix]);
	  }
	  theLocales[ix].win->show();
	  return 1;
	}
      }
    }
    break;
  }
  return Fl_Box::handle(event);
}

