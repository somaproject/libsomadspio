#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/home/phoenix/function/function.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <somadspio/acqdatasource.h>
#include <somadspio/eventcodec.h>
#include <somadspio/dspcontrol.h>
#include <somadspio/logging.h>

namespace somadspio { 
  namespace ads =  codec::AcqDataSource; 
  using namespace boost::phoenix;

  AcqDataSource::AcqDataSource(StateProxy & parent) :
    parent_(parent)
  {

    // initialize to sane values
    linkStatus_ = false; 
    mode_  = 0; 
    for (int i = 0; i < CHANCNT; i++) {
      gains_[i] = 0; 
      hpfens_[i] = false; 
    }
    chansel_ = 0; 
    DSPIOL_(info) << "AcqDataSource: sending initialization query set"; 

    // send queries
    sn::EventTX_t etx = ads::queryLinkStatus(); 
    parent_.setETXDest(etx); 
    parent_.submit(createList(etx), val(true)); 
    
    etx = ads::queryMode(); 
    parent_.setETXDest(etx); 
    parent_.submit(createList(etx), val(true)); 

    for (int i = 0; i < CHANCNT; i++) {
      etx = ads::queryGain(i); 
      parent_.setETXDest(etx); 
      parent_.submit(createList(etx), val(true)); 

      etx = ads::queryHPF(i); 
      parent_.setETXDest(etx); 
      parent_.submit(createList(etx), val(true)); 

      etx = ads::queryChanRangeMin(i); 
      parent_.setETXDest(etx); 
      parent_.submit(createList(etx), val(true)); 

      etx = ads::queryChanRangeMax(i); 
      parent_.setETXDest(etx); 
      parent_.submit(createList(etx), val(true)); 
    }

    etx = ads::queryChanSel(); 
    parent_.setETXDest(etx); 
    parent_.submit(createList(etx), val(true)); 
  
    
  }
  
  bool AcqDataSource::newEvent(const sn::Event_t & event)
  {
    switch(event.cmd) {
    case codec::AcqDataSource::RESPBCAST :
      parseEvent(event); 
      return true; 
    default:
      return false; 
    }
       
  }
  
  void AcqDataSource::parseEvent(const sn::Event_t & event) {

    ads::PARAMETERS p = ads::whichParam(event); 
    switch(p) {
    case ads::LINKSTATUS: 
      {
	bool linkstatus = ads::linkStatus(event); 
	DSPIOL_(info) << "AcqDataSource: received link status =  "
		 << linkstatus; 

	if (linkStatus_ != linkstatus) {
	  linkStatus_ = linkstatus; 
	  linkStatusSignal_.emit(linkStatus_); 
	}
      }
      break;

    case ads::MODE:
      {
	int mode = ads::mode(event); 
	DSPIOL_(info) << "AcqDataSource: received mode update, mode =" 
		 << mode; 	  

	if (mode_ != mode) {
	  mode_ = mode; 
	  modeSignal_.emit(mode); 
	}
      }
      break; 
      
    case ads::CHANGAIN:
      {
	ads::changain_t gain = ads::changeGain(event); 
	if (gain.first > CHANCNT) {
	  DSPIOL_(error) << "AcqDataSource: received channel gain event with incorrect channel,"
		    << gain.first; 
	}
	
	DSPIOL_(info) << "AcqDataSource: received gain for chan "
		 << gain.first << " = " << gain.second; 

	if (gains_[gain.first] != gain.second) {
	  gains_[gain.first] = gain.second; 
	  gainSignal_.emit(gain.first, gain.second); 
	}
      }
      break; 

    case ads::CHANHPF:
      {
	ads::chanhpf_t hpf = ads::changeHPF(event); 
	if (hpf.first > CHANCNT) {
	  DSPIOL_(error) << "AcqDataSource:: received channel hpf event with incorrect channel"
		    << hpf.first; 
	}

	DSPIOL_(info) << "AcqDataSource: received hpf for chan " 
		 << hpf.first << " hpen = " << hpf.second; 

	if (hpfens_[hpf.first] != hpf.second) {
	  hpfens_[hpf.first] = hpf.second; 
	  hpfenSignal_.emit(hpf.first, hpf.second); 
	}
      }
      break; 

    case ads::CHANSEL:
      {
	int chan = ads::chanSel(event); 

	DSPIOL_(info) << "AcqDataSource: received chan sel, new chan ="
		 << chan;  
	
	if (chansel_ != chan) {
	  chansel_ = chan; 
	  chanselSignal_.emit(chan); 
	}
      }
      break; 

    case ads::CHANRANGEMIN:
      {
	ads::chanrange_t range = ads::chanRangeMin(event); 

	DSPIOL_(info) << "AcqDataSource: received updated range min for chan " 
		 << range.first << " = " << range.second; 
	
	if (ranges_[range.first].first != range.second) {
	  ranges_[range.first].first = range.second; 
	  rangeSignal_.emit(range.first, ranges_[range.first]); 
	}
      }
      break; 

    case ads::CHANRANGEMAX:
      {
	ads::chanrange_t range = ads::chanRangeMax(event); 

	DSPIOL_(info) << "AcqDataSource: received updated range max for chan " 
		 << range.first << " = " << range.second; 
	
	if (ranges_[range.first].second != range.second) {
	  ranges_[range.first].second = range.second; 
	  rangeSignal_.emit(range.first, ranges_[range.first]); 
	}
      }
      break; 

    default:
      DSPIOL_(error)  << "AcqDataSource: Unknown "; 
      // FIXME Add the rest here!!!
      break;
    }
  }

  bool AcqDataSource::getLinkStatus()
  {
    return linkStatus_; 
  }
  
  sigc::signal<void, bool> & AcqDataSource::linkStatus()
  {
    return linkStatusSignal_; 
  }
  
  int AcqDataSource::getMode()
  {
    return mode_; 
  }
  
  void AcqDataSource::setMode(int mode)
  {
    
    sn::EventTX_t etx = ads::mode(mode); 
    parent_.setETXDest(etx); 
    parent_.submit(createList(etx), val(true)); 

  }

  sigc::signal<void, int> & AcqDataSource::mode()
  {
    return modeSignal_; 
  }


  int AcqDataSource::getGain(int chan) {
    return gains_[chan]; 

  }
  
  void AcqDataSource::setGain(int chan, int gain) {
    ads::changain_t cg; 
    cg.first = chan; 
    cg.second = gain; 
    sn::EventTX_t etx = ads::changeGain(cg); 
    parent_.setETXDest(etx); 
    parent_.submit(createList(etx),  ref(gains_[chan]) == gain); 

  }

  sigc::signal<void, int, int> & AcqDataSource::gain()
  {
    return gainSignal_; 

  }

  bool AcqDataSource::getHPFen(int chan)
  {
    return hpfens_[chan];
  }
  
  void AcqDataSource::setHPFen(int chan, bool bval)
  {
    ads::chanhpf_t hpf; 
    hpf.first = chan; 
    hpf.second = bval; 

    sn::EventTX_t etx = ads::changeHPF(hpf); 
    parent_.setETXDest(etx); 
    parent_.submit(createList(etx), ref(hpfens_[chan]) == bval); 

  }

  sigc::signal<void, int, bool> & AcqDataSource::hpfen()
  {
    return hpfenSignal_; 
  }

  int AcqDataSource::getChanSel()
  {
    return chansel_; 
  }

  void AcqDataSource::setChanSel(int chan)
  {
    sn::EventTX_t etx = ads::chanSel(chan); 
    parent_.setETXDest(etx); 
    parent_.submit(createList(etx), val(true)); 
  }

  sigc::signal<void, int> & AcqDataSource::chansel()
  {
    return chanselSignal_; 
  }
  

  range_t AcqDataSource::getRange(int chan) {
    return ranges_[chan]; 
  }
  
  sigc::signal<void, int, range_t> & AcqDataSource::range()
  {
    return rangeSignal_; 
  }

}
