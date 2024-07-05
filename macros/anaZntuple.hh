#ifndef ANAZNTUPLE_h
#define ANAZNTUPLE_h

#include <vector>

#include "TNtuple.h"

// ----------------------------------------------------------------------
class anaZntuple {

public :
  anaZntuple(std::string file = "/Users/ursl/data/pioneer/test/g4beamline.root");
  ~anaZntuple();

  void load(std::string afiles);
  void fillGoodTracks(int location);


  std::vector<TNtuple*> fvTrees;
  std::map<int, TNtuple*> fmTrees;

  std::vector<int> fvGoodEntries;


};

#endif
