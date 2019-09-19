//
// Created by khood on 9/19/19.
//

#include "MaekawePeer.h"

MaekawePeer::MaekawePeer() : Peer<MaekaweMessage>(){
    _requestQueue = std::queue<MaekaweMessage>();
    _currentLockingPeer = "";
};

MaekawePeer::MaekawePeer(std::string id) : Peer<MaekaweMessage>(id){
    _requestQueue = std::queue<MaekaweMessage>();
    _currentLockingPeer = "";
}

MaekawePeer::MaekawePeer(const MaekawePeer &rhs){
    _requestQueue = rhs._requestQueue;
    _currentLockingPeer = rhs._currentLockingPeer;
};

MaekawePeer::~MaekawePeer(){

};

void MaekawePeer::makeRequest(){
    for(auto n = _neighbors.begin(); n != _neighbors.end(); n++){
        Packet<MaekaweMessage> r(n->second->id());
        r.setTarget(n->second->id());
        r.setSource(id());
        r.setBody(MaekaweMessage(n->second->id(), MESSAGE_TYPE::REQUEST));
        _outStream.push_back(r);
    }
};

void MaekawePeer::preformComputation (){
    auto m = _inStream.begin();
    while( m != _inStream.end()){
        switch (m->getMessage()._msg)
            case MESSAGE_TYPE::REQUEST: {
                handleRequest(m->getMessage());
                _inStream.erase(m);
                break;
            }

    }
};

void MaekawePeer::handleRequest(MaekaweMessage m){
    if(m._source == _currentLockingPeer) {
        // do nothing
    } else if (m._source != _currentLockingPeer && _currentLockingPeer != ""){
        _requestQueue.push(m);
    } else if(m._source != _currentLockingPeer && _currentLockingPeer == ""){
        _currentLockingPeer = m._source;
        *_log << id() << " has been locked by " << _currentLockingPeer << std::endl;
    }else{
        assert(false);
    }

};

MaekawePeer& MaekawePeer::operator=(const MaekawePeer &rhs){
    if(this == &rhs){
        return *this;
    }

    _requestQueue = rhs._requestQueue;
    _currentLockingPeer = rhs._currentLockingPeer;

    return *this;
};