#include "anaZntuple.hh"



#include <fstream>
#include <iostream>
#include <sstream>

#include "TROOT.h"
#include "TKey.h"
#include "TFile.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

using namespace std;

// ----------------------------------------------------------------------
anaZntuple::anaZntuple(string file) {
  load(file);
  fillGoodTracks(16600);
}


// ----------------------------------------------------------------------
anaZntuple::~anaZntuple() {
}


// ----------------------------------------------------------------------
void anaZntuple::load(string rootfile) {
  TFile *f = TFile::Open(rootfile.c_str());
  f->cd("NTuple");
  TIter next(gDirectory->GetListOfKeys());
  TKey *key(0);
  int z(0);
  while ((key = (TKey*)next())) {
    int prevCycle(0);
    if (gROOT->GetClass(key->GetClassName())->InheritsFrom("TDirectory")) continue;
    TNtuple *nt = (TNtuple*)key->ReadObj();
    int version = key->GetCycle();
    TString hname(nt->GetName());
    sscanf(hname.Data(), "Z%d", &z);
    // -- skip backup version (appears later)
    if (fmTrees.size() > 0) {
      if (fmTrees.count(z)) continue;
    }
    // cout << hname << " z = " << z << " " << version << endl;
    fmTrees.insert(make_pair(z, nt));
  }

  for (auto it: fmTrees) {
    //    cout << "adding z = " << it.first << " it.second = " << it.second->GetName() << endl;
    fvTrees.push_back(it.second);
  }
  cout << "added " << fmTrees.size() << " trees" << endl;
}


// ----------------------------------------------------------------------
void anaZntuple::fillGoodTracks(int location) {
  TTree *t = fmTrees[location];
  if (t) {
    cout << location << " entries: " << t->GetEntries() << endl;
    TTreeReader reader(t);
    TTreeReaderValue<float> evtid(reader, "EventID");
    while (reader.Next()) {
      fvGoodEntries.push_back(static_cast<int>(*evtid));
    }
    cout << "event ids reaching location " << location << " total: " << fvGoodEntries.size() << endl;
  }
}
