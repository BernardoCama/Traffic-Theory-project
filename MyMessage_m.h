//
// Generated file, do not edit! Created by nedtool 5.4 from MyMessage.msg.
//

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#ifndef __MYMESSAGE_M_H
#define __MYMESSAGE_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0504
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>MyMessage.msg:2</tt> by nedtool.
 * <pre>
 * message MyMessage
 * {
 *     simtime_t Timestamp2; //not used
 *     int color = -1; //not used
 *     int been_in_service = 0;
 * }
 * </pre>
 */
class MyMessage : public ::omnetpp::cMessage
{
  protected:
    ::omnetpp::simtime_t Timestamp2;
    int color;
    int been_in_service;

  private:
    void copy(const MyMessage& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const MyMessage&);

  public:
    MyMessage(const char *name=nullptr, short kind=0);
    MyMessage(const MyMessage& other);
    virtual ~MyMessage();
    MyMessage& operator=(const MyMessage& other);
    virtual MyMessage *dup() const override {return new MyMessage(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual ::omnetpp::simtime_t getTimestamp2() const;
    virtual void setTimestamp2(::omnetpp::simtime_t Timestamp2);
    virtual int getColor() const;
    virtual void setColor(int color);
    virtual int getBeen_in_service() const;
    virtual void setBeen_in_service(int been_in_service);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const MyMessage& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, MyMessage& obj) {obj.parsimUnpack(b);}


#endif // ifndef __MYMESSAGE_M_H
