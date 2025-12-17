#ifndef TOX_H
#define TOX_H

#include "Socket.h"

#include <cstdint>
#include <string>
#include <ostream>
#include <istream>

// A place to put toxes
class Tox {
public:
    Tox() :  mId(0), mMsg(""), mRating(0) {}
    Tox(uint8_t c, uint32_t i, const std::string &msg) :  mId(i), mMsg(msg), mRating(0) {}
    Tox(const Tox &other) :  mId(other.mId), mMsg(other.mMsg), mRating(other.mRating) {}
    Tox(Socket &client);
    void get_latest(Socket & client);
    void pull(Socket & client);
    void print();

    bool operator==(const Tox &other) const;
    bool operator!=(const Tox &other) const {
        return !(*this == other);
    }

    uint32_t getId() const { return mId; }
    std::string getMsg() const { return mMsg; }
    uint32_t getRating() const { return mRating; }

    void vote(Socket &client, bool upvote); 
    std::string id_as_hex();
    // void get_rating(Socket &client);
    uint32_t get_rating() { return mRating; } 

    void serialise(std::ostream & out);
    void deserialise(std::istream & in);

protected:

private:
    uint32_t mId;
    uint32_t mRating;
    std::string mMsg;
};


#endif // TOX_H