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

#include <string>

#ifndef _EVENT_H_
#define _EVENT_H_

// Order important, all before Ev_end are grouped together, after, inserted by time
enum Event_kind {Ev_start, Ev_tag, Ev_resume, Ev_end, Ev_task, Ev_comm, Ev_fork};

class Event {  // Base class for events .....

  protected:
    long sec, usec;   //  Time of the event
    int  nodeid;      //  Which node generated the event

  public:
    Event(long esec, long eusec, int id)
      : sec(esec), usec(eusec), nodeid(id) {};

    long tsec () { return sec; }
    long tusec () { return usec; }

    virtual int Ekind() = 0;
    int nodeId() { return nodeid; }

    virtual void print() = 0;
};

bool operator < (Event &lh, Event &rh);
bool operator == (Event &lh, Event &rh);

class  E_start : public Event {
  // base class esec and eusec is USER time from getrusage()
  // at the call to startVdebug()
  private:
    long s_sec, s_usec;
     
  public:
    E_start (long u_sec, long u_usec, long s_sec, long s_usec, int nid)
      : Event(u_sec, u_usec, nid), s_sec(s_sec), s_usec(s_usec) {};

    double cpu_time()  { return sec+s_sec + (usec+s_usec)/1000000; }
    double user_time() { return sec+usec/1000000; }
    double sys_time() { return s_sec+s_usec/1000000; }
    virtual int Ekind() {return Ev_start;}

    virtual void print() { printf ("Start: id %d user %ld.%06ld sys %ld.%06ld\n",
				   nodeid, sec, usec, s_sec, s_usec); }
};


class  E_task : public Event {

  private:
    int nodeid;
  
  public:
    E_task (long esec, long eusec, int nid) : Event(esec,eusec, nid) {};

    virtual int Ekind() {return Ev_task;}
    virtual void print() { printf ("Task: id %d time %ld.%06ld\n",
				   nodeid, sec, usec); }

};

class E_comm : public Event {

   private:
     int  dstid;
     int  elemsize, datalen;

   public:
     E_comm (long esec, long eusec, int esrcid, int edstid, int elSize,
	     int dLen) : Event(esec, eusec, esrcid), dstid(edstid),
                         elemsize(elSize), datalen(dLen) {};

     int srcId() { return nodeId(); }
     int dstId() { return dstid; }
     int elemSize() { return elemsize; }
     int dataLen() { return datalen; }
     int totalLen() { return elemsize * datalen; }

     virtual int Ekind() {return Ev_comm;}
     virtual void print() { 
       printf ("Comm: id %d time %ld.%06ld to %d size %d\n",
	       nodeid, sec, usec, dstid, elemsize * datalen); }
};

class E_fork : public Event {

   private:
     int  dstid;
     int  argsize;
     bool isFast;

   public:
       E_fork (long esec, long eusec, int esrcid, int edstid, int argsize,
	       bool fast) : Event(esec,eusec, esrcid), dstid(edstid),
                            argsize(argsize), isFast(fast) {};

     int srcId() { return nodeId(); }
     int dstId() { return dstid; }
     bool fast() { return isFast; }
     int argSize() { return argsize; }

     virtual int Ekind() {return Ev_fork;}
     virtual void print() {
       printf ("Fork%s: id %d time %ld.%06ld to %d datasize %d\n",
	       (isFast ? "(fast)" : ""), nodeid, sec, usec, dstid, argsize);
     }
};

class E_tag : public Event {

   private:
     int tag_num;
     std::string tag_name;
     long s_sec, s_usec;

   public:
     E_tag (long u_sec, long u_usec, int nodeid, long s_sec, long s_usec,
	    long tagno, char *tag)
       : Event(u_sec, u_usec, nodeid), tag_num(tagno), tag_name(tag),
       s_sec(s_sec), s_usec(s_usec)
       { }

     int tagNo() { return tag_num; }
     std::string tagName() { return tag_name; }

     double cpu_time()  { return sec+s_sec + (usec+s_usec)/1000000; }
     double user_time() { return sec+usec/1000000; }
	 
     virtual int Ekind() {return Ev_tag;}
     virtual void print() {
       printf ("Tag: id %d user %ld.%06ld sys %ld.%06ld tagNo %d, Tag='%s'\n",
	       nodeid, sec, usec, s_sec, s_usec, tag_num, tag_name.c_str());
     }

};

class E_resume : public Event {

  private:
    long s_sec, s_usec;
    int tagid;

  public:
    E_resume (long u_sec, long u_usec, int nodeid, long s_sec, long s_usec, int tagid)
      : Event(u_sec, u_usec, nodeid), s_sec(s_sec), s_usec(s_usec), tagid(tagid) {};

    double cpu_time()  { return sec+s_sec + (usec+s_usec)/1000000; }
    double user_time() { return sec+usec/1000000; }
    int tagId() { return tagid; }
    virtual int Ekind() { return Ev_resume; }
    virtual void print() { printf ("Resume: id %d user %ld.%06ld sys %ld.%06ld tagNo %d\n",
				   nodeid, sec, usec, s_sec, s_usec, tagid); }
};

class E_end : public Event {

  private:
    long s_sec, s_usec;

  public:
    E_end (long u_sec, long u_usec, int nodeid, long s_sec, long s_usec)
      : Event(u_sec, u_usec, nodeid), s_sec(s_sec), s_usec(s_usec) {};

    double cpu_time()  { return sec+s_sec + (usec+s_usec)/1000000; }
    double user_time() { return sec+usec/1000000; }
    virtual int Ekind() { return Ev_end; }
    virtual void print() { printf ("End: id %d user %ld.%06ld sys %ld.%06ld\n",
				   nodeid, sec, usec, s_sec, s_usec); }
};

#endif
