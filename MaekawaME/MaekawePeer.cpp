//
// Created by khood on 9/19/19.
//

#include "MaekawePeer.h"

MaekawePeer::MaekawePeer() : Peer<MaekaweMessage>(){
    _requestQueue = std::deque<MaekaweMessage>();
    _currentLockingPeer = "";
    _currentInquiry = MaekaweMessage();
    _currentLockCount = 0;
};

MaekawePeer::MaekawePeer(std::string id) : Peer<MaekaweMessage>(id){
    _requestQueue = std::deque<MaekaweMessage>();
    _currentLockingPeer = "";
    _currentInquiry = MaekaweMessage();
    _currentLockCount = 0;
}

MaekawePeer::MaekawePeer(const MaekawePeer &rhs){
    _requestQueue = rhs._requestQueue;
    _currentLockingPeer = rhs._currentLockingPeer;
    _currentInquiry = rhs._currentInquiry;
    _currentLockCount = rhs._currentLockCount;
};

MaekawePeer::~MaekawePeer(){

};

void MaekawePeer::makeRequest(){
    _requestQueue.push_back(MaekaweMessage(id(), MESSAGE_TYPE::REQUEST));
    for(auto n = _neighbors.begin(); n != _neighbors.end(); n++){
        Packet<MaekaweMessage> r(n->second->id());
        r.setTarget(n->second->id());
        r.setSource(id());
        r.setBody(MaekaweMessage(id(), MESSAGE_TYPE::REQUEST));
        _outStream.push_back(r);
    }
    if(_currentLockingPeer == ""){
        _currentLockingPeer = _requestQueue.front()._source;
        _requestQueue.pop_front();
    }
};

void MaekawePeer::preformComputation (){

    for(auto m = _inStream.begin(); m != _inStream.end(); m++){
        switch (m->getMessage()._msg) {
            case MESSAGE_TYPE::REQUEST: {
                handleRequest(m->getMessage());
                break;
            }
            case MESSAGE_TYPE::LOCK: {
                handleLock(m->getMessage());
                break;
            }
            case MESSAGE_TYPE::RELEASE:{
                handleRelease(m->getMessage());
                break;
            }
            defulte:
            {
                assert(false);
            };
        }
    }
    _inStream.clear();
    if(_currentLockingPeer != "") {
        *_log << id() << " is locked on " << _currentLockingPeer << " and request queue is..." << std::endl;
        for (auto r = _requestQueue.begin(); r != _requestQueue.end(); r++) {
            *_log << r->_source << ", ";
        }
        *_log << std::endl;
    }else{
        *_log << id()<< " has no requests"<< std::endl;
    }
};

void MaekawePeer::handleRequest(MaekaweMessage m){
    if(m._source == _currentLockingPeer) {
        // do nothing
    } else if (m._source != _currentLockingPeer && _currentLockingPeer != ""){
        _requestQueue.push_back(m);
    } else if(m._source != _currentLockingPeer && _currentLockingPeer == ""){
        _currentLockingPeer = m._source;
        Packet<MaekaweMessage> l(id());
        l.setSource(id());
        l.setTarget(_currentLockingPeer);
        l.setBody(MaekaweMessage(id(),MESSAGE_TYPE::LOCK));
        _outStream.push_back(l);
        *_log << id() << " has been locked by " << _currentLockingPeer << std::endl;
    }else{
        assert(false);
    }
};

void MaekawePeer::handleLock(MaekaweMessage){
    _currentLockCount++;
    *_log << id() << " has "<< _currentLockCount << " locking messages"<< std::endl;
    // if peer gets enough locking messages
    if(_currentLockCount == _neighbors.size()){
        *_log << id() << " is entering the critical section" << std::endl;
        *_log << id() << " is releasing the critical section" << std::endl;
        _currentLockCount = 0;
        for(auto n = _neighbors.begin(); n != _neighbors.end(); n++){
            Packet<MaekaweMessage> r(id());
            r.setTarget(n->second->id());
            r.setSource(id());
            r.setBody(MaekaweMessage(id(),MESSAGE_TYPE::RELEASE));
            _outStream.push_back(r);
            if(!_requestQueue.empty()){
                _currentLockingPeer = _requestQueue.front()._source;
                _requestQueue.pop_front();
            }else{
                _currentLockingPeer = "";
            }

        }
    }
};

void MaekawePeer::handleRelease(MaekaweMessage m){
  if(m._source == _currentLockingPeer){
      if(!_requestQueue.empty()){
          _currentLockingPeer = _requestQueue.front()._source;
          _requestQueue.pop_front();
      }else{
          _currentLockingPeer = "";
      }
      if(_currentLockingPeer != ""){
          *_log<< id() << " is now released now locked on "<< _currentLockingPeer << std::endl;
          Packet<MaekaweMessage> l(id());
          l.setSource(id());
          l.setTarget(_currentLockingPeer);
          l.setBody(MaekaweMessage(id(),MESSAGE_TYPE::LOCK));
          _outStream.push_back(l);
          *_log << id() << " has been locked by " << _currentLockingPeer << std::endl;
      }else{
          *_log<< id() << " is now released"<< std::endl;
      }

  }
};

MaekawePeer& MaekawePeer::operator=(const MaekawePeer &rhs){
    if(this == &rhs){
        return *this;
    }

    _requestQueue = rhs._requestQueue;
    _currentLockingPeer = rhs._currentLockingPeer;
    _currentInquiry = rhs._currentInquiry;
    _currentLockCount = rhs._currentLockCount;

    return *this;
};