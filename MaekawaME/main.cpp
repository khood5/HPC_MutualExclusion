#include <iostream>
#include "MaekawePeer.h"

int main() {

    std::cout<< "Working with K = 2 and N = 3"<< std::endl
            << "Quorums are " << std::endl
            << "P1, P2" << std::endl
            << "P1, P3" << std::endl
            << "P2, P3" << std::endl;

    MaekawePeer P1("P1");
    MaekawePeer P2("P2");
    MaekawePeer P3("P3");


    P1.addNeighbor(P2,1);
    P1.addNeighbor(P3,1);

    P2.addNeighbor(P1,1);
    P2.addNeighbor(P3,1);

    P3.addNeighbor(P1,1);
    P3.addNeighbor(P2,1);

    MaekawePeer *requester = &P1;
    for(int rounds = 0; rounds < 1000; rounds++){
        if(rounds%2 == 0) {
            requester->makeRequest();
            if (*requester == P1) {
                requester = &P2;
            } else if (*requester == P2) {
                requester = &P3;
            } else {
                requester = &P1;
            }
        }

        P1.receive();
        P2.receive();
        P3.receive();

        P1.preformComputation();
        P2.preformComputation();
        P3.preformComputation();

        P1.transmit();
        P2.transmit();
        P3.transmit();
    }

    return 0;
}