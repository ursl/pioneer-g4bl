#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>

// ----------------------------------------------------------------------
// cd pioneer-g4bl/macros
// root
// root [0] .L anaZntuple.C 
// root [1] load("some rootfile")
// root [2] mkGif("y:x", "PDGid==211", double xmin, double xmax, double ymin, double ymax, zmin = 0, zmax = 18000)
// root [n] mkGifs("g4beamline.root", 16700.)

// ----------------------------------------------------------------------

vector<TNtuple*> vTrees;
map<int, TNtuple*> mTrees;

// ----------------------------------------------------------------------
void load(string rootfile) {
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
    if (mTrees.size() > 0) {
      if (mTrees.count(z)) continue;
    }
    // cout << hname << " z = " << z << " " << version << endl;
    mTrees.insert(make_pair(z, nt));
  }

  for (auto it: mTrees) {
    cout << "adding z = " << it.first << " it.second = " << it.second->GetName() << endl;
    vTrees.push_back(it.second);
  }
}

// ----------------------------------------------------------------------
void draw2d(TNtuple *t, string varx, string vary, string cuts, double xmin, double xmax, double ymin, double ymax, string opt = "colz") {
  TH2D *h2 = new TH2D("h2", "bla", 200, xmin, xmax, 200, ymin, ymax);
  h2->SetXTitle(varx.c_str()); 
  string svary = vary; 
  replaceAll(svary, "TMath::Sqrt(Px*Px+Py*Py+Pz*Pz)", "P");

  h2->SetYTitle(vary.c_str()); 
  if (cuts == "") h2->SetTitle("(no cut)");
  
  gPad->SetLeftMargin(0.15);
  gPad->SetRightMargin(0.15);

  string drawVar = vary + ":" + varx + ">>h2";
  t->Draw(drawVar.c_str(), cuts.c_str(), "goff");
  h2->SetTitle((t->GetName() + string(" ") + cuts + string("(") + std::to_string(static_cast<int>(h2->GetEntries())) + string(")")).c_str());
  h2->DrawCopy(opt.c_str());
}


// ----------------------------------------------------------------------
void mkGif(string varx, string vary, string cuts, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax) {

  gStyle->SetOptStat(0);

  string svary = vary; 
  replaceAll(svary, "/", "DIV");
  replaceAll(svary, "TMath::Sqrt(Px*Px+Py*Py+Pz*Pz)", "P");

  string scuts = cuts;
  replaceAll(scuts, "PDGid==211", "pions");
  replaceAll(scuts, "PDGid!=0", "all");
  
  string gifname = Form("anaz-%s-%s-%s.gif", varx.c_str(), svary.c_str(), scuts.c_str());
  if (remove(gifname.c_str()) != 0) {
    cout <<  "Error deleting file ->" << gifname << "<-" << endl;
  } else {
    cout <<  "Success deleting file ->" << gifname << "<-" << endl;
  }
  gifname += "+";
  
  gPad->SetGridx(1); 
  gPad->SetGridy(1); 
  int z(-1);
  for (unsigned int i = 0; i < vTrees.size(); ++i) {
    TString hname(vTrees[i]->GetName());
    sscanf(hname.Data(), "Z%d", &z);
    if (z > zmax) continue;
    draw2d(vTrees[i], varx, vary, cuts, xmin, xmax, ymin, ymax);
    gPad->Modified(); 
    gPad->Update(); 
    gPad->Print(gifname.c_str());
  }

  gifname += "+";
  cout << "Printing ->" << gifname << "<-" << endl;
  gPad->Print(gifname.c_str());
  
}


// ----------------------------------------------------------------------
void mkGifs(string file = "g4beamline.root", double zmax = 16700.) {
  load(file);
  mkGif("x", "y", "PDGid==211", -250., 250.,-250., 250., 0., zmax);
  mkGif("x", "Px/TMath::Sqrt(Px*Px+Py*Py+Pz*Pz)", "PDGid==211", -250., 250.,-0.2, 0.2, 0., zmax);
  mkGif("x", "Pz", "PDGid==211", -250., 250.,40., 100., 0., zmax);
}
