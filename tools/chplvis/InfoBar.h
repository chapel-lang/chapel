
#ifndef INFOBAR_H
#define INFOBAR_H

#include <FL/Fl.H>
#include <FL/Fl_Box.H>

class InfoBar : public Fl_Box {

 private:
  int maxTasks;
  int maxComms;

 public:

  InfoBar (int x, int y, int w, int h, const char *label = 0)
    : Fl_Box(x,y,w,h) { maxTasks = 0; maxComms = 0; };
   
  void draw(void);

  void setMaxes(int tasks, int comms) {
    maxTasks = tasks;
    maxComms = comms;
  }

};


#endif

