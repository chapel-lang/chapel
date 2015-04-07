#ifndef _EVENT_H_
#define _EVENT_H_



class Event {  // Base class for events .....

  private:
    long sec, usec;   //  Time of the event

  public:
    Event(long esec, long eusec): sec(esec), usec(eusec) {};

    friend bool operator < (Event &lh, Event &rh);
    friend bool operator == (Event &lh, Event &rh);

    long tsec () { return sec; }
    long tusec () { return usec; }
     
};


class  e_task : Event {

  private:
    int nodeid;
  
  public:
    e_task (long esec, long eusec, int nid) : Event(esec,eusec),
      nodeid(nid) {};

    int localId() { return nodeid; }

};

class e_comm : Event {

   private:
     int  srcid, dstid;

   public:
     e_comm (long esec, long eusec, int esrcid, int edstid) : Event(esec,eusec),
       srcid(esrcid), dstid(edstid) {};

     int srcId() { return srcid; }
     int dstId() { return dstid; }

};

#endif
