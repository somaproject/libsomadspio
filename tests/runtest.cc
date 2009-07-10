#include <iostream>
#include <somanetwork/network.h>
#include <somanetwork/tspike.h>
#include <list>
#include <somadspio/dspcontrol.h>


std::string SOMAIP = "10.0.0.2"; 
int datasrc = 2; 
using namespace somanetwork; 

pNetworkInterface_t  net;  // global for callback

void sendEvent(const EventTXList_t & etl) {
  net->sendEvents(etl); 
}


int main(int argc, char ** argv)
{
   // Create the soma network object
  net = Network::createINet(SOMAIP); 
  
  //net->enableDataRX(datasrc, somanetwork::TSPIKE); 

  // start running
  net->run(); 
  somadspio::StateProxy sp(datasrc, &sendEvent ); 
  int pipe = net->getEventFifoPipe(); 
  while(1) {
    char x; 
    read(pipe, &x, 1); 
    pEventPacket_t ep = net->getNewEvents(); 
    for(EventList_t::iterator ei = ep->events->begin(); 
        ei != ep->events->end(); ++ei) {
      sp.newEvent(*ei); 
    }

  }

  net->shutdown(); 
 


}
