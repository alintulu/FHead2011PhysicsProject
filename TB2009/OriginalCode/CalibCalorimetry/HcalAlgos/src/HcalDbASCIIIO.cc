
//
// F.Ratnikov (UMd), Oct 28, 2005
// $Id: HcalDbASCIIIO.cc,v 1.38 2008/03/12 13:11:45 rofierzy Exp $
//
#include <vector>
#include <string>

#include "DataFormats/HcalDetId/interface/HcalGenericDetId.h"
#include "DataFormats/HcalDetId/interface/HcalElectronicsId.h"
#include "CalibFormats/HcalObjects/interface/HcalText2DetIdConverter.h"

#include "CondFormats/HcalObjects/interface/AllObjects.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalDbASCIIIO.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

namespace {
  class DetIdLess {
  public:
    bool operator () (DetId fFirst, DetId fSecond) const {
      HcalGenericDetId first (fFirst);
      HcalGenericDetId second (fSecond);
      if (first.genericSubdet () != second.genericSubdet ()) return first.genericSubdet () < second.genericSubdet ();
      if (first.isHcalDetId ()) {
	HcalDetId f1 (first);
	HcalDetId s1 (second);
	return	f1.zside () != s1.zside () ? f1.zside () < s1.zside () :
	  f1.iphi () != s1.iphi () ? f1.iphi () < s1.iphi () :
	  f1.ietaAbs () != s1.ietaAbs () ? f1.ietaAbs () < s1.ietaAbs () :
	  f1.depth () < s1.depth ();
      }
      else {
	return first.rawId() < second.rawId();
      }
    }
  };
  class HcalElectronicsIdLess {
  public:
    bool operator () (HcalElectronicsId first, HcalElectronicsId second) const {
      return
	first.readoutVMECrateId () != second.readoutVMECrateId () ? first.readoutVMECrateId () < second.readoutVMECrateId () :
	first.htrSlot () != second.htrSlot () ? first.htrSlot () < second.htrSlot () :
	first.htrTopBottom () != second.htrTopBottom () ? first.htrTopBottom () < second.htrTopBottom () :
	first.fiberIndex () != second.fiberIndex () ? first.fiberIndex () < second.fiberIndex () :
	first.fiberChanId () < second.fiberChanId ();
    }
  };
}

std::vector <std::string> splitString (const std::string& fLine) {
  std::vector <std::string> result;
  int start = 0;
  bool empty = true;
  for (unsigned i = 0; i <= fLine.size (); i++) {
    if (fLine [i] == ' ' || i == fLine.size ()) {
      if (!empty) {
	std::string item (fLine, start, i-start);
	result.push_back (item);
	empty = true;
      }
      start = i+1;
    }
    else {
      if (empty) empty = false;
    }
  }
  return result;
}

DetId getId (const std::vector <std::string> & items) {
  HcalText2DetIdConverter converter (items [3], items [0], items [1], items [2]);
  return converter.getId ();
}

void dumpId (std::ostream& fOutput, DetId id) {
  HcalText2DetIdConverter converter (id);
  char buffer [1024];
  sprintf (buffer, "  %15s %15s %15s %15s",
	   converter.getField1 ().c_str (), converter.getField2 ().c_str (), converter.getField3 ().c_str (),converter.getFlavor ().c_str ());  
  fOutput << buffer;
}

template <class T,class S> 
bool getHcalObject (std::istream& fInput, T* fObject, S* fCondObject) {
  if (!fObject) fObject = new T;
  char buffer [1024];
  while (fInput.getline(buffer, 1024)) {
    if (buffer [0] == '#') continue; //ignore comment
    std::vector <std::string> items = splitString (std::string (buffer));
    if (items.size()==0) continue; // blank line
    if (items.size () < 8) {
      edm::LogWarning("Format Error") << "Bad line: " << buffer << "\n line must contain 8 items: eta, phi, depth, subdet, 4x values" << std::endl;
      continue;
    }
    DetId id = getId (items);
    
//    if (fObject->exists(id) )
//      edm::LogWarning("Redefining Channel") << "line: " << buffer << "\n attempts to redefine data. Ignored" << std::endl;
//    else
//      {
      double shift = 0;
      if(items.size() == 10)
         shift = atof(items[9].c_str()) / 4;

	fCondObject = new S(id, atof (items [4].c_str()) - shift, atof (items [5].c_str()) - shift, 
			   atof (items [6].c_str()) - shift, atof (items [7].c_str()) - shift);
	fObject->addValues(*fCondObject);
	delete fCondObject;
	//      }
  }

  return true;
}

template <class T>
bool dumpHcalObject (std::ostream& fOutput, const T& fObject) {
  char buffer [1024];
  sprintf (buffer, "# %15s %15s %15s %15s %8s %8s %8s %8s %10s\n", "eta", "phi", "dep", "det", "cap0", "cap1", "cap2", "cap3", "DetId");
  fOutput << buffer;
  std::vector<DetId> channels = fObject.getAllChannels ();
  std::sort (channels.begin(), channels.end(), DetIdLess ());
  for (std::vector<DetId>::iterator channel = channels.begin ();
       channel !=  channels.end ();
       channel++) {
    const float* values = fObject.getValues (*channel)->getValues ();
    if (values) {
      dumpId (fOutput, *channel);
      sprintf (buffer, " %8.5f %8.5f %8.5f %8.5f %10X\n",
	       values[0], values[1], values[2], values[3], channel->rawId ());
      fOutput << buffer;
    }
  }
  return true;
}

template <class T,class S> 
bool getHcalSingleFloatObject (std::istream& fInput, T* fObject, S* fCondObject) {
  if (!fObject) fObject = new T;
  char buffer [1024];
  while (fInput.getline(buffer, 1024)) {
    if (buffer [0] == '#') continue; //ignore comment
    std::vector <std::string> items = splitString (std::string (buffer));
    if (items.size()==0) continue; // blank line
    if (items.size () < 5) {
      edm::LogWarning("Format Error") << "Bad line: " << buffer << "\n line must contain 8 items: eta, phi, depth, subdet, value" << std::endl;
      continue;
    }
    DetId id = getId (items);
    
//    if (fObject->exists(id) )
//      edm::LogWarning("Redefining Channel") << "line: " << buffer << "\n attempts to redefine data. Ignored" << std::endl;
//    else
//      {
	fCondObject = new S(id, atof (items [4].c_str()) );
	fObject->addValues(*fCondObject);
	delete fCondObject;
	//      }
  }
  return true;
}

template <class T>
bool dumpHcalSingleFloatObject (std::ostream& fOutput, const T& fObject) {
  char buffer [1024];
  sprintf (buffer, "# %15s %15s %15s %15s %8s %10s\n", "eta", "phi", "dep", "det", "value", "DetId");
  fOutput << buffer;
  std::vector<DetId> channels = fObject.getAllChannels ();
  std::sort (channels.begin(), channels.end(), DetIdLess ());
  for (std::vector<DetId>::iterator channel = channels.begin ();
       channel !=  channels.end ();
       channel++) {
    const float value = fObject.getValues (*channel)->getValue ();
    dumpId (fOutput, *channel);
    sprintf (buffer, " %8.5f %10X\n",
	     value, channel->rawId ());
    fOutput << buffer;
  }
  return true;
}

template <class T,class S> 
bool getHcalSingleIntObject (std::istream& fInput, T* fObject, S* fCondObject) {
  if (!fObject) fObject = new T;
  char buffer [1024];
  while (fInput.getline(buffer, 1024)) {
    if (buffer [0] == '#') continue; //ignore comment
    std::vector <std::string> items = splitString (std::string (buffer));
    if (items.size()==0) continue; // blank line
    if (items.size () < 5) {
      edm::LogWarning("Format Error") << "Bad line: " << buffer << "\n line must contain 8 items: eta, phi, depth, subdet, value" << std::endl;
      continue;
    }
    DetId id = getId (items);
    
//    if (fObject->exists(id) )
//      edm::LogWarning("Redefining Channel") << "line: " << buffer << "\n attempts to redefine data. Ignored" << std::endl;
//    else
//      {
	fCondObject = new S(id, atoi (items [4].c_str()) );
	fObject->addValues(*fCondObject);
	delete fCondObject;
	//      }
  }
  return true;
}

template <class T>
bool dumpHcalSingleIntObject (std::ostream& fOutput, const T& fObject) {
  char buffer [1024];
  sprintf (buffer, "# %15s %15s %15s %15s %8s %10s\n", "eta", "phi", "dep", "det", "value", "DetId");
  fOutput << buffer;
  std::vector<DetId> channels = fObject.getAllChannels ();
  std::sort (channels.begin(), channels.end(), DetIdLess ());
  for (std::vector<DetId>::iterator channel = channels.begin ();
       channel !=  channels.end ();
       channel++) {
    const int value = fObject.getValues (*channel)->getValue ();
    dumpId (fOutput, *channel);
    sprintf (buffer, " %15d %10X\n",
	     value, channel->rawId ());
    fOutput << buffer;
  }
  return true;
}


bool HcalDbASCIIIO::getObject (std::istream& fInput, HcalPedestals* fObject) {return getHcalObject (fInput, fObject, new HcalPedestal);}
bool HcalDbASCIIIO::dumpObject (std::ostream& fOutput, const HcalPedestals& fObject) {return dumpHcalObject (fOutput, fObject);}
bool HcalDbASCIIIO::getObject (std::istream& fInput, HcalGains* fObject) {return getHcalObject (fInput, fObject, new HcalGain);}
bool HcalDbASCIIIO::dumpObject (std::ostream& fOutput, const HcalGains& fObject) {return dumpHcalObject (fOutput, fObject);}
bool HcalDbASCIIIO::getObject (std::istream& fInput, HcalGainWidths* fObject) {return getHcalObject (fInput, fObject, new HcalGainWidth);}
bool HcalDbASCIIIO::dumpObject (std::ostream& fOutput, const HcalGainWidths& fObject) {return dumpHcalObject (fOutput, fObject);}

bool HcalDbASCIIIO::getObject (std::istream& fInput, HcalRespCorrs* fObject) {return getHcalSingleFloatObject (fInput, fObject, new HcalRespCorr); }
bool HcalDbASCIIIO::dumpObject (std::ostream& fOutput, const HcalRespCorrs& fObject) {return dumpHcalSingleFloatObject (fOutput, fObject); }
bool HcalDbASCIIIO::getObject (std::istream& fInput, HcalZSThresholds* fObject) {return getHcalSingleIntObject (fInput, fObject, new HcalZSThreshold); }
bool HcalDbASCIIIO::dumpObject (std::ostream& fOutput, const HcalZSThresholds& fObject) {return dumpHcalSingleIntObject (fOutput, fObject); }
bool HcalDbASCIIIO::getObject (std::istream& fInput, HcalChannelQuality* fObject) {return getHcalSingleIntObject (fInput, fObject, new HcalChannelStatus); }
bool HcalDbASCIIIO::dumpObject (std::ostream& fOutput, const HcalChannelQuality& fObject) {return dumpHcalSingleIntObject (fOutput, fObject); }


bool HcalDbASCIIIO::getObject (std::istream& fInput, HcalPedestalWidths* fObject) {
  if (!fObject) fObject = new HcalPedestalWidths;
  char buffer [1024];
  while (fInput.getline(buffer, 1024)) {
    if (buffer [0] == '#') continue; //ignore comment
    std::vector <std::string> items = splitString (std::string (buffer));
    if (items.size () < 14) {
      edm::LogWarning("Format Error") << "Bad line: " << buffer << "\n line must contain 14 items: eta, phi, depth, subdet, 10x correlations" << std::endl;
      continue;
    }
    DetId id = getId (items);

//    if (fObject->exists(id) )
//      edm::LogWarning("Redefining Channel") << "line: " << buffer << "\n attempts to redefine data. Ignored" << std::endl;
//    else
//      {
	HcalPedestalWidth values(id);
	values.setSigma (0, 0, atof (items [4].c_str()));
	values.setSigma (1, 0, atof (items [5].c_str()));
	values.setSigma (1, 1, atof (items [6].c_str()));
	values.setSigma (2, 0, atof (items [7].c_str()));
	values.setSigma (2, 1, atof (items [8].c_str()));
	values.setSigma (2, 2, atof (items [9].c_str()));
	values.setSigma (3, 0, atof (items [10].c_str()));
	values.setSigma (3, 1, atof (items [11].c_str()));
	values.setSigma (3, 2, atof (items [12].c_str()));
	values.setSigma (3, 3, atof (items [13].c_str()));
	fObject->addValues(values);
	//      }
  }
  return true;
}

bool HcalDbASCIIIO::dumpObject (std::ostream& fOutput, const HcalPedestalWidths& fObject) {
  char buffer [1024];
  sprintf (buffer, "# %15s %15s %15s %15s %8s %8s %8s %8s %8s %8s %8s %8s %8s %8s %10s\n", 
	   "eta", "phi", "dep", "det", 
	   "sig_0_o", "sig_1_0", "sig_1_1", "sig_2_0", "sig_2_1", "sig_2_2", "sig_3_0", "sig_3_1", "sig_3_2", "sig_3_3", 
	   "DetId");
  fOutput << buffer;
  std::vector<DetId> channels = fObject.getAllChannels ();
  std::sort (channels.begin(), channels.end(), DetIdLess ());
  for (std::vector<DetId>::iterator channel = channels.begin ();
       channel !=  channels.end ();
       channel++) {
    const HcalPedestalWidth* item = fObject.getValues (*channel);
    if (item) {
      dumpId (fOutput, *channel);
      sprintf (buffer, " %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %10X\n",
	       item->getSigma (0,0), item->getSigma (1,0), item->getSigma (1,1), item->getSigma (2,0), item->getSigma (2,1), item->getSigma (2,2), 
	       item->getSigma (3,0), item->getSigma (3,1), item->getSigma (3,2), item->getSigma (3,3), channel->rawId ());
      fOutput << buffer;
    }
  }
  return true;
}

bool HcalDbASCIIIO::getObject (std::istream& fInput, HcalQIEData* fObject) {
  char buffer [1024];
  while (fInput.getline(buffer, 1024)) {
    if (buffer [0] == '#') continue; //ignore comment
    std::vector <std::string> items = splitString (std::string (buffer));
    if (items.size()<1) continue;
    if (items [0] == "SHAPE") { // basic shape
      if (items.size () < 33) {
	edm::LogWarning("Format Error") << "Bad line: " << buffer << "\n line must contain 33 items: SHAPE  32 x low QIE edges for first 32 bins" << std::endl;
	continue;
      }
      float lowEdges [32];
      int i = 32;
      while (--i >= 0) lowEdges [i] = atof (items [i+1].c_str ());
      //      fObject->setShape (lowEdges);
    }
    else { // QIE parameters
      if (items.size () < 36) {
	edm::LogWarning("Format Error") << "Bad line: " << buffer << "\n line must contain 36 items: eta, phi, depth, subdet, 4 capId x 4 Ranges x offsets, 4 capId x 4 Ranges x slopes" << std::endl;
	continue;
      }
      DetId id = getId (items);
      fObject->sort ();
      //      try {
      //      fObject->getCoder (id);
      //      edm::LogWarning("Redefining Channel") << "line: " << buffer << "\n attempts to redefine data. Ignored" << std::endl;
	//      }
//      catch (cms::Exception& e) {
	HcalQIECoder coder (id.rawId ());
	int index = 4;
	for (unsigned capid = 0; capid < 4; capid++) {
	  for (unsigned range = 0; range < 4; range++) {
	    coder.setOffset (capid, range, atof (items [index++].c_str ()));
	  }
	}
	for (unsigned capid = 0; capid < 4; capid++) {
	  for (unsigned range = 0; range < 4; range++) {
	    coder.setSlope (capid, range, atof (items [index++].c_str ()));
	  }
	}
	fObject->addCoder (coder);
//      }
    }
  }
  fObject->sort ();
  return true;
}

bool HcalDbASCIIIO::dumpObject (std::ostream& fOutput, const HcalQIEData& fObject) {
  char buffer [1024];
  fOutput << "# QIE basic shape: SHAPE 32 x low edge values for first 32 channels" << std::endl;
  sprintf (buffer, "SHAPE ");
  fOutput << buffer;
  for (unsigned bin = 0; bin < 32; bin++) {
    sprintf (buffer, " %8.5f", fObject.getShape ().lowEdge (bin));
    fOutput << buffer;
  }
  fOutput << std::endl;

  fOutput << "# QIE data" << std::endl;
  sprintf (buffer, "# %15s %15s %15s %15s %36s %36s %36s %36s %36s %36s %36s %36s\n", 
	   "eta", "phi", "dep", "det", 
	   "4 x offsets cap0", "4 x offsets cap1", "4 x offsets cap2", "4 x offsets cap3",
	   "4 x slopes cap0", "4 x slopes cap1", "4 x slopes cap2", "4 x slopes cap3");
  fOutput << buffer;
  std::vector<DetId> channels = fObject.getAllChannels ();
  std::sort (channels.begin(), channels.end(), DetIdLess ());
  for (std::vector<DetId>::iterator channel = channels.begin ();
       channel !=  channels.end ();
       channel++) {
    const HcalQIECoder* coder = fObject.getCoder (*channel);
    dumpId (fOutput, *channel);
    for (unsigned capid = 0; capid < 4; capid++) {
      for (unsigned range = 0; range < 4; range++) {
	sprintf (buffer, " %8.5f", coder->offset (capid, range));
	fOutput << buffer;
      }
    }
    for (unsigned capid = 0; capid < 4; capid++) {
      for (unsigned range = 0; range < 4; range++) {
	sprintf (buffer, " %8.5f", coder->slope (capid, range));
	fOutput << buffer;
      }
    }
    fOutput << std::endl;
  }
  return true;
}

bool HcalDbASCIIIO::getObject (std::istream& fInput, HcalCalibrationQIEData* fObject) {
  char buffer [1024];
  while (fInput.getline(buffer, 1024)) {
    if (buffer [0] == '#') continue; //ignore comment
    std::vector <std::string> items = splitString (std::string (buffer));
    if (items.size () < 36) {
      edm::LogWarning("Format Error") << "Bad line: " << buffer << "\n line must contain 36 items: eta, phi, depth, subdet, 32 bin values" << std::endl;
      continue;
    }
    DetId id = getId (items);
    fObject->sort ();
    //    try {
    //    fObject->getCoder (id);
    //    edm::LogWarning("Redefining Channel") << "line: " << buffer << "\n attempts to redefine data. Ignored" << std::endl;
      //    }
//    catch (cms::Exception& e) {
      HcalCalibrationQIECoder coder (id.rawId ());
      int index = 4;
      float values [32];
      for (unsigned bin = 0; bin < 32; bin++) {
	values[bin] = atof (items [index++].c_str ());
      }
      coder.setMinCharges (values);
      fObject->addCoder (coder);
//    }
  }
  fObject->sort ();
  return true;
}

bool HcalDbASCIIIO::dumpObject (std::ostream& fOutput, const HcalCalibrationQIEData& fObject) {
  char buffer [1024];
  fOutput << "# QIE data in calibration mode" << std::endl;
  sprintf (buffer, "# %15s %15s %15s %15s %288s\n", 
	   "eta", "phi", "dep", "det", "32 x charges");
  fOutput << buffer;
  std::vector<DetId> channels = fObject.getAllChannels ();
  std::sort (channels.begin(), channels.end(), DetIdLess ());
  for (std::vector<DetId>::iterator channel = channels.begin ();
       channel !=  channels.end ();
       channel++) {
    const HcalCalibrationQIECoder* coder = fObject.getCoder (*channel);
    if (coder) {
      dumpId (fOutput, *channel);
      const float* lowEdge = coder->minCharges ();
      for (unsigned bin = 0; bin < 32; bin++) {
	sprintf (buffer, " %8.5f", lowEdge [bin]);
	fOutput << buffer;
      }
      fOutput << std::endl;
    }
  }
  return true;
}

//bool HcalDbASCIIIO::getObject (std::istream& fInput, HcalChannelQuality* fObject) {
//  char buffer [1024];
//  while (fInput.getline(buffer, 1024)) {
//    if (buffer [0] == '#') continue; //ignore comment
//    std::vector <std::string> items = splitString (std::string (buffer));
//    if (items.size () < 5) {
//      edm::LogWarning("Format Error") << "Bad line: " << buffer << "\n line must contain 5 items: eta, phi, depth, subdet, GOOD/BAD/HOT/DEAD" << std::endl;
//      continue;
//    }
//    HcalChannelQuality::Quality value (HcalChannelQuality::UNKNOWN);
//    for (int i = 0; i < (int) HcalChannelQuality::END; i++) {
//      if (items [4] == std::string (HcalChannelQuality::str ((HcalChannelQuality::Quality) i))) {
//	value = (HcalChannelQuality::Quality) i;
//      }
//    }
//    fObject->setChannel (getId (items).rawId (), value);
//  }
//  fObject->sort ();
//  return true;
//}

//bool HcalDbASCIIIO::dumpObject (std::ostream& fOutput, const HcalChannelQuality& fObject) {
//  char buffer [1024];
//  sprintf (buffer, "# %15s %15s %15s %15s %8s\n", 
//	   "eta", "phi", "dep", "det", 
//	   "quality");
//  fOutput << buffer;
//  std::vector<unsigned long> channels = fObject.getAllChannels ();
//  for (std::vector<unsigned long>::iterator channel = channels.begin ();
//       channel !=  channels.end ();
//       channel++) {
//    DetId id ((uint32_t) *channel);
//    dumpId (fOutput, id);
//    sprintf (buffer, " %8s\n", HcalChannelQuality::str (fObject.quality (*channel)));
//  }
//  return true;
//}

bool HcalDbASCIIIO::getObject (std::istream& fInput, HcalElectronicsMap* fObject) {
  char buffer [1024];
  while (fInput.getline(buffer, 1024)) {
    if (buffer [0] == '#') continue; //ignore comment
    std::vector <std::string> items = splitString (std::string (buffer));
    if (items.size () < 12) {
      if (items.size()==0) continue; // no warning here
      if (items.size()<9) {
	edm::LogError("MapFormat") << "HcalElectronicsMap-> line too short: " << buffer;
	continue;
      }
      if (items[8]=="NA" || items[8]=="NT") {
	while (items.size()<12) items.push_back(""); // don't worry here
      } else if (items[8]=="HT") {
	if (items.size()==11) items.push_back("");
	else {
	  edm::LogError("MapFormat") << "HcalElectronicsMap-> Bad line: " << buffer 
				     << "\n HT line must contain at least 11 items: i  cr sl tb dcc spigot fiber fiberchan subdet=HT ieta iphi";
	  continue;
	}
      } else {
	edm::LogError("MapFormat") << "HcalElectronicsMap-> Bad line: " << buffer 
				   << "\n line must contain 12 items: i  cr sl tb dcc spigot fiber fiberchan subdet ieta iphi depth";
	continue;
      }
    }
    //    std::cout << "HcalElectronicsMap-> processing line: " << buffer << std::endl;
    int crate = atoi (items [1].c_str());
    int slot = atoi (items [2].c_str());
    int top = 1;
    if (items [3] == "b") top = 0;
    int dcc = atoi (items [4].c_str());
    int spigot = atoi (items [5].c_str());
    HcalElectronicsId elId;
    if (items[8] == "HT" || items[8] == "NT") {
      int slb = atoi (items [6].c_str());
      int slbCh = atoi (items [7].c_str());
      elId=HcalElectronicsId(slbCh, slb, spigot, dcc,crate,slot,top);
    } else {
      int fiber = atoi (items [6].c_str());
      int fiberCh = atoi (items [7].c_str());

      elId=HcalElectronicsId(fiberCh, fiber, spigot, dcc);
      elId.setHTR (crate, slot, top);
    }

    // first, handle undefined cases
    if (items [8] == "NA") { // undefined channel
      fObject->mapEId2chId (elId, DetId (HcalDetId::Undefined));
    } else if (items [8] == "NT") { // undefined trigger channel
      fObject->mapEId2tId (elId, DetId (HcalTrigTowerDetId::Undefined));
    } else {
      HcalText2DetIdConverter converter (items [8], items [9], items [10], items [11]);
      if (converter.isHcalDetId ()) { 
	fObject->mapEId2chId (elId, converter.getId ());
      }
      else if (converter.isHcalTrigTowerDetId ()) {
	fObject->mapEId2tId (elId, converter.getId ());
      }
      else if (converter.isHcalCalibDetId ()) {
	fObject->mapEId2chId (elId, converter.getId ());
      }
      else if (converter.isHcalZDCDetId ()) {
	fObject->mapEId2chId (elId, converter.getId ());
      }
      else {
	edm::LogWarning("Format Error") << "HcalElectronicsMap-> Unknown subdetector: " 
		  << items [8] << '/' << items [9] << '/' << items [10] << '/' << items [11] << std::endl; 
      }
    }
  }
  fObject->sort ();
  return true;
}

bool HcalDbASCIIIO::dumpObject (std::ostream& fOutput, const HcalElectronicsMap& fObject) {
  std::vector<HcalElectronicsId> eids = fObject.allElectronicsId ();
  char buf [1024];
  sprintf (buf, "#%10s %6s %6s %6s %6s %6s %6s %6s %15s %15s %15s %15s",
	   "i", "cr", "sl", "tb", "dcc", "spigot", "fiber/slb", "fibcha/slbcha", "subdet", "ieta", "iphi", "depth");
  fOutput << buf << std::endl;

  for (unsigned i = 0; i < eids.size (); i++) {
    HcalElectronicsId eid = eids[i];
    if (eid.isTriggerChainId()) {
      DetId trigger = fObject.lookupTrigger (eid);
      if (trigger.rawId ()) {
	HcalText2DetIdConverter converter (trigger);
	sprintf (buf, " %10X %6d %6d %6c %6d %6d %6d %6d %15s %15s %15s %15s",
		 //		 i,
		 converter.getId().rawId(),
		 eid.readoutVMECrateId(), eid.htrSlot(), eid.htrTopBottom()>0?'t':'b', eid.dccid(), eid.spigot(), eid.fiberIndex(), eid.fiberChanId(),
		 converter.getFlavor ().c_str (), converter.getField1 ().c_str (), converter.getField2 ().c_str (), converter.getField3 ().c_str ()
		 );
	fOutput << buf << std::endl;
      }
    } else {
      DetId channel = fObject.lookup (eid);
      if (channel.rawId()) {
	HcalText2DetIdConverter converter (channel);
	sprintf (buf, " %10X %6d %6d %6c %6d %6d %6d %6d %15s %15s %15s %15s",
		 //		 i,
		 converter.getId().rawId(),
		 eid.readoutVMECrateId(), eid.htrSlot(), eid.htrTopBottom()>0?'t':'b', eid.dccid(), eid.spigot(), eid.fiberIndex(), eid.fiberChanId(),
		 converter.getFlavor ().c_str (), converter.getField1 ().c_str (), converter.getField2 ().c_str (), converter.getField3 ().c_str ()
	       );
	fOutput << buf << std::endl;
      }
    }
  }
  return true;
}
