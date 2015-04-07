#ifndef _EVENT_H_
#define _EVENT_H_

enum Event_kind {Ev_task, Ev_comm};

class Event {  // Base class for events .....

  private:
    long sec, usec;   //  Time of the event

  public:
    Event(long esec, long eusec): sec(esec), usec(eusec) {};

    friend bool operator < (Event &lh, Event &rh);
    friend bool operator == (Event &lh, Event &rh);

    long tsec () { return sec; }
    long tusec () { return usec; }

    virtual int Ekind() = 0;
};


class  E_task : public Event {

  private:
    int nodeid;
  
  public:
    E_task (long esec, long eusec, int nid) : Event(esec,eusec),
      nodeid(nid) {};

    int localId() { return nodeid; }

    virtual int Ekind() {return Ev_task;}

};

class E_comm : public Event {

   private:
     int  srcid, dstid;

   public:
     E_comm (long esec, long eusec, int esrcid, int edstid) : Event(esec,eusec),
       srcid(esrcid), dstid(edstid) {};

     int srcId() { return srcid; }
     int dstId() { return dstid; }

     virtual int Ekind() {return Ev_comm;}

};

#endif
