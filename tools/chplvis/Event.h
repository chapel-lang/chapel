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
