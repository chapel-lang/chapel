#ifndef _EVENT_H_
#define _EVENT_H_

enum Event_kind {Ev_task, Ev_comm, Ev_fork};

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

class E_fork : public Event {

   private:
     int  srcid, dstid;
     bool isFast;

   public:
     E_fork (long esec, long eusec, int esrcid, int edstid, bool fast)
       : Event(esec,eusec), srcid(esrcid), dstid(edstid), isFast(fast) {};

     int srcId() { return srcid; }
     int dstId() { return dstid; }
     bool fast() { return isFast; }

     virtual int Ekind() {return Ev_fork;}

};

#endif
