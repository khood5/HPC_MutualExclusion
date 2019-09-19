//
// Created by khood on 9/19/19.
//

#ifndef MAEKAWAME_MAEKAWEPEER_H
#define MAEKAWAME_MAEKAWEPEER_H

#include "Peer.hpp"
#include "Packet.hpp"
#include <queue>
#include <string>
#include <assert.h>

enum MESSAGE_TYPE{REQUEST, RELEASE, LOCK, INQUIRY, FAIL};

struct MaekaweMessage{
    std::string     _source;
    MESSAGE_TYPE    _msg;

    MaekaweMessage(std::string s, MESSAGE_TYPE t){ _source = s; _msg = t;};
};

class MaekawePeer : public Peer<MaekaweMessage>{

protected:
    std::queue<MaekaweMessage>  _requestQueue;
    std::string                 _currentLockingPeer;

    void            handleRequest       (MaekaweMessage);

public:
    MaekawePeer                         ();
    MaekawePeer                         (std::string);
    MaekawePeer                         (const MaekawePeer&);
    ~MaekawePeer                        ();

    void            makeRequest         ();
    void            preformComputation  ();

    MaekawePeer&    operator=           (const MaekawePeer&);
};


#endif //MAEKAWAME_MAEKAWEPEER_H
