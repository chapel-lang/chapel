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
  if (max == 1) return fl_rgb_color(255,0,0);
  if (val == 1) return fl_rgb_color(0,180,255);
  return fl_rgb_color( 255*(val/max), 180 * ((max-val)/max), 255 * ((max-val)/max));
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
    for (ix = 0; ix < getSize; ix++)  { 
      for (ix2 = 0; ix2 < getSize; ix2++) 
	if (comms[ix][ix2].win != NULL)
	  delete comms[ix][ix2].win;
      delete [] comms[ix];
    }
    delete [] comms;
  }
  if (tags != NULL) {
    delete [] tags;
    tags = NULL;
  }
  
  // Alloc new space ... except for tags, done in process data.
  theLocales = new localeInfo [numlocales];
  comms = new  struct commInfo* [numlocales];
  if (theLocales == NULL || comms == NULL) {
    fprintf (stderr, "chplvis: out of memory.\n");
    exit(1);
  }
  for (ix =0; ix < numlocales; ix++)
    theLocales[ix].numTasks = 0;
  getSize = numlocales;
  for (ix = 0; ix < getSize; ix++) {
    comms[ix] = new struct commInfo[numlocales];
    if (comms[ix] == NULL) {
      fprintf (stderr, "chplvis: out of memory.\n");
      exit(1);
    }
    for (ix2 = 0; ix2 < numlocales; ix2++) {
      comms[ix][ix2].numComms = 0;
      comms[ix][ix2].numGets = 0;
      comms[ix][ix2].numPuts = 0;
      comms[ix][ix2].commSize = 0;
      comms[ix][ix2].win = NULL;
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

//  tagNo:  -3 => new open
//          -2 => All: start to finish
//          -1 => Start to first tag (tag 0)
//          0 and greater => start at tagNo to next tag.

void ViewField::processData(int tagNum)
{
  int ix1, ix2;  // For processing the arrays

  // printf ("ViewField::processData(%d)\n", tagNum);

  // Initialize ... in case data has changed.
  for (ix1 = 0; ix1 < numlocales; ix1++) {
    theLocales[ix1].numTasks = 0;
    theLocales[ix1].userCpu = 0;
    theLocales[ix1].sysCpu = 0;
    theLocales[ix1].Cpu = 0;
    theLocales[ix1].refUserCpu = 0;
    theLocales[ix1].refSysCpu = 0;
    if (theLocales[ix1].win != NULL) {
      theLocales[ix1].win->hide();
    }
    for (ix2 = 0; ix2 < numlocales; ix2++)  {
      comms[ix1][ix2].numComms = 0;
      comms[ix1][ix2].numGets = 0;
      comms[ix1][ix2].numPuts = 0;
      comms[ix1][ix2].commSize = 0;
      if (comms[ix1][ix2].win != NULL) 
	comms[ix1][ix2].win->hide();
    }
  }
  theLocales[0].numTasks = 1;
  maxTasks = 1;
  maxComms = 0;
  maxCpu = 0.0000001;
  maxDatasize = 0;

  Event *ev;
  int stopTag;

  // Select first event to use:
  if (tagNum < 0) {
    ev = VisData.getFirstEvent();
    stopTag = (tagNum == -1 ? 0 : -1 );
  } else {
    ev = VisData.getTagNo(tagNum);
    stopTag = tagNum + 1;
  }

  while ( ev != NULL ) {

    E_task   *tp = NULL;
    E_comm   *cp = NULL;
    E_fork   *fp = NULL;
    E_start  *sp = NULL;
    E_end    *ep = NULL;
    E_tag    *gp = NULL;
    E_pause  *pp = NULL;

    bool stopProcessing = false;

    // debug:
    // ev->print();
    
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
	if (++(comms[cp->srcId()][cp->dstId()].numComms) > maxComms)
	  maxComms = comms[cp->srcId()][cp->dstId()].numComms;
	comms[cp->srcId()][cp->dstId()].commSize += cp->totalLen();
	if (comms[cp->srcId()][cp->dstId()].commSize > maxDatasize)
	  maxDatasize = comms[cp->srcId()][cp->dstId()].commSize;
	if (cp->isGet())
	  comms[cp->srcId()][cp->dstId()].numGets++;
	else
	  comms[cp->srcId()][cp->dstId()].numPuts++;
        break;

      case Ev_fork:
        fp = (E_fork *)ev;
        if (++(comms[fp->srcId()][fp->dstId()].numComms) > maxComms)
	  maxComms = comms[fp->srcId()][fp->dstId()].numComms;
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
	//printf ("end: node = %d cpu = %f\n",  ep->nodeId(), 
	//	theLocales[ep->nodeId()].Cpu);
	break;

      case Ev_pause:
	// Reset the ref time?
	pp = (E_pause *)ev;
	// Need to update times for correctness, either for tag->tag or resume
	theLocales[pp->nodeId()].userCpu += pp->user_time()
	  - theLocales[pp->nodeId()].refUserCpu;
	theLocales[pp->nodeId()].sysCpu += pp->sys_time()
	  - theLocales[pp->nodeId()].refSysCpu;
	theLocales[pp->nodeId()].Cpu = theLocales[pp->nodeId()].userCpu 
	  + theLocales[pp->nodeId()].sysCpu;
	if (maxCpu < theLocales[pp->nodeId()].Cpu)
	    maxCpu = theLocales[pp->nodeId()].Cpu;
	break;
	 
      case Ev_tag:
	// Checking out the tag ...
	gp = (E_tag *)ev;
        int tgNo = gp->tagNo();
        if (tagNum == -3 && tags[tgNo].tagNo < 0) {
	  tags[tgNo].tagNo = tgNo;
	  // printf("processing tag '%s'\n", gp->tagName().c_str());
	  tags[tgNo].tagName = new char [gp->tagName().length()+25];
	  snprintf (tags[tgNo].tagName, gp->tagName().length()+25,
		    "Tags/Tag %d (%s)", tgNo, gp->tagName().c_str());
	}
	if (tagNum == tgNo) {
	  //printf ("Tag: setting ref times\n");
	  theLocales[gp->nodeId()].refUserCpu = gp->user_time();
	  theLocales[gp->nodeId()].refSysCpu = gp->sys_time();
	} else if (tgNo == stopTag ) {
	  //printf ("Tag: updating user/sys/cpu times.\n");
	  // Need to update times for correctness, either for tag->tag or resume
	  theLocales[gp->nodeId()].userCpu += gp->user_time()
	    - theLocales[gp->nodeId()].refUserCpu;
	  theLocales[gp->nodeId()].sysCpu += gp->sys_time()
	    - theLocales[gp->nodeId()].refSysCpu;
	  theLocales[gp->nodeId()].Cpu = theLocales[gp->nodeId()].userCpu 
	    + theLocales[gp->nodeId()].sysCpu;
	  if (maxCpu < theLocales[gp->nodeId()].Cpu)
	    maxCpu = theLocales[gp->nodeId()].Cpu;
	}

	//printf ("tag: node = %d user = %f, sys = %f, cpu = %f\n",  gp->nodeId(),
 	//        theLocales[gp->nodeId()].userCpu,
	//	theLocales[gp->nodeId()].sysCpu,
	//	theLocales[gp->nodeId()].Cpu);

	// Stop here?
	stopProcessing = (gp->nodeId() == numlocales-1) &&
                         tgNo == stopTag;
	break;
    }

    if (stopProcessing)
      ev = NULL;
    else 
      ev = VisData.getNextEvent();
    //if (ev == NULL) printf ("ev is NULL.\n");

  }

  Info->setMaxes(maxTasks, maxComms, maxDatasize, maxCpu);
  //printf ("maxTasks %d, maxComms %d\n", maxTasks, maxComms);

  if (tagNum == -3)  makeTagsMenu();

 }

static void selTag(Fl_Widget *w, void *p)
{
  long ix = (long) p;
  struct tagInfo *ptr = (struct tagInfo *)p;
  if (ix == -2) {
    // printf ("selTag called on All\n");
    DbgView->processData(-2);
    Info->setTagName("All");
  } else if (ix == -1) {
    // printf ("selTag called on Start\n");
    DbgView->processData(-1);
    Info->setTagName("Start");
  } else {
    // printf ("selTag called on tag %d \"%s\"\n", ptr->tagNo, ptr->tagName);
    DbgView->processData(ptr->tagNo);
    Info->setTagName(ptr->tagName);
  }
  DbgView->redraw();
  Info->redraw();
}

void ViewField::makeTagsMenu(void)
{
  // Remove the old one if it exists
  if (tagMenu > 0) {
    MainMenuBar->remove(tagMenu);
    tagMenu = -1;
  }
  if (VisData.NumTags() >= 1) {

    // printf("Make tags menu, %d tags\n", VisData.NumTags());

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
  //printf ("draw: x,y = %d,%d, h,w = %d,%d\n", x(), y(), w(), h());

  cx = x() + w()/2;
  cy = y() + h()/2;
  rx = 0.85 * w() / 2;
  ry = 0.85 * h() / 2;

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
	com2ix = comms[iy][ix].numComms;
	com2iy = comms[ix][iy].numComms;
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

// Checks to see if click is on a comm link.
// Returns:  0 if not:  1 if closer to loc1, 2 if closer to loc2.
static int isOnCommLink ( int x, int y, localeInfo *loc1, localeInfo *loc2) {
  float slope;
  float newy;
  float ydiff;
  int ylen = loc2->y - loc1->y;
  int xlen = loc2->x - loc1->x;


  if (xlen == 0) {
    // Vertically above each other
    int xdiff = abs(loc2->x - x);
    // printf ("Vertically above each other. xdiff = %d\n", xdiff);
    if (xdiff <= 3) {
      if (abs(loc2->y - y) > abs(loc1->y - y))
	return 1;
      else
	return 2;
    }
  } else {
    slope =  (float)(ylen) / (float)(xlen);
    if (fabs(slope) < 2) {
      newy  = (float)loc2->y - slope * (loc2->x - x);
      ydiff = fabs(y - newy);
      // printf ("slope: %f ydiff = %f\n", slope, ydiff);
      if (ydiff < 3) {
	// assumed on the line
	if (abs(loc2->x - x) < abs(xlen)/2)
	  return 2;
	else
          return 1;  
      }
    } else {
      float newx  = (float)loc2->x - (loc2->y - y)/slope;
      float xdiff = fabs(x - newx);
      // printf ("slope: %f xdiff = %f\n", slope, xdiff);
      if (xdiff < 3) {
	// assumed on the line
	if (abs(loc2->y - y) < abs(ylen)/2)
	  return 2;
	else
          return 1;  
      }
    }
  }

  return 0;
}

int ViewField::handle(int event)
{
  int ix;

  int x = Fl::event_x();
  int y = Fl::event_y();

  int i, j; // search for the comm link clicked
  
  switch (event) {
  case FL_PUSH:
    //printf ("Push at (%d,%d)\n", x, y);
    break;
  case FL_RELEASE:
    //printf ("Release at (%d,%d)\n", x, y);
    // Click on a locale?
    if (numlocales > 0) {
      for (ix = 0; ix < numlocales; ix++) {
	// See if release is inside a locale
	localeInfo *loc = &theLocales[ix];
	if ( x > loc->x-loc->w/2 && x <= loc->x + loc->w/2 &&
	     y > loc->y-loc->h/2 && y <= loc->y + loc->h/2) {
	  if (theLocales[ix].win == NULL) {
	    // Create the window
	    theLocales[ix].win = make_locale_window(ix, &theLocales[ix]);
	  } else {
	    theLocales[ix].win->updateWin();
	  }
	  if (theLocales[ix].win->visible()) 
	    theLocales[ix].win->hide();
          else
	    theLocales[ix].win->show();
	  return 1;
	}
      }
    }

    // Click on a comm link?
    for (i = 0; i < numlocales; i++) {
      localeInfo *loci, *locj;
      loci = &theLocales[i];
      for (j = i+1; j < numlocales; j++) {
	locj = &theLocales[j];
	// printf ("link: %d->%d -- ", i, j);
	if (comms[i][j].numComms != 0
	    || comms[j][i].numComms != 0) {
	  int OnComm = isOnCommLink(x,y,loci,locj);
	  if (OnComm) {
	    // printf ("Link %d -> %d, nearer locale %d\n", i, j, (OnComm > 1 ? j : i ));
	    if (OnComm == 1) {
	      // j -> i, swap so both can use i->j
	      int t = j; j = i; i = t;
	    } 
	    if (comms[i][j].win == NULL) {
	      comms[i][j].win = make_comm_window(i,j,&comms[i][j]);
	    } else {
	      comms[i][j].win->updateWin();
	    }
	    if (comms[i][j].win->visible())
	      comms[i][j].win->hide();
	    else
	      comms[i][j].win->show();
	    return 1;
	  }
	}
      }
    }
    
    break;
  }
  return Fl_Box::handle(event);
}

