#include "TFile.h"
#include "TNtuple.h"
#include "TString.h"
#include "TChain.h"
#include "TROOT.h"

// include std libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <limits>

using namespace std;

// ----------------------------------------------------------------------
void ConvertRootToBLTrack2(string fNameList, string outDirectory, int pdgid, string prefix, string vdetName) {
  int cut_pdgid = pdgid;
  double ptot_cut_low = 1;
  double ptot_cut_high = 100;

  TChain* globChain = new TChain(Form("VirtualDetector/%s",vdetName.c_str()));
  
  std::cout << ">>> Load Chain from file: " << fNameList << std::endl;
    
  ifstream fList(fNameList.c_str());
  if (!fList)    {
    std::cout << "!!! Can't open file " << fNameList << std::endl;
    return;
  }
        
  char lineFromFile[255];
  while(fList.getline(lineFromFile, 250)) {
    TString fileName = lineFromFile;
    if(globChain->Add(fileName))
      std::cout << ">> File '" << fileName << "' has been loaded" << std::endl;
    else
      std::cout << ">> Can't load file '" << fileName << "'" << std::endl;
  }
    
  std::cout << ">> Total number of entries: " << globChain->GetEntries() <<std::endl;
  fList.close();
    
  TString outFilename = Form("%sBLTrackFile2_PDGid%d_%s.txt", prefix.c_str(), cut_pdgid, vdetName.c_str());

  ofstream outfile;
  std::string txtfilename = Form("%s/%s", outDirectory.c_str(), outFilename.Data());
  outfile.open(txtfilename, fstream::app);
  if (outfile) {
    std::cout<<"Created file: " << txtfilename << std::endl;
  } else {
    std::cout<<"FILE CREATION FAILED!"<<std::endl;
    exit(1);
  }
    
  outfile <<"#BLTrackFile2"<<std::endl;
  
  outfile <<"#x y z Px Py Pz t PDGid EventID TrackID ParentID Weight Bx By Bz Ex Ey Ez ProperTime PathLength PolX PolY PolZ InitX InitY InitZ InitT InitKE"<<std::endl;

  std::string line;
    
  float Ptot;
  float det_TrackID, det_ParentID, det_PDGid;
  float det_Weight;
  float det_Pz,det_Px, det_Py, det_x, det_y, det_z, det_t;
  float det_PolX, det_PolY, det_PolZ, det_ProperTime, det_PathLength;
  float det_Ex, det_Ey, det_Ez, det_Bx, det_By, det_Bz;
  float det_InitX, det_InitY, det_InitZ, det_InitT, det_InitKE;
  long long int det_EventID;

    
  globChain->SetBranchAddress("Pz", &det_Pz);
  globChain->SetBranchAddress("Px", &det_Px);
  globChain->SetBranchAddress("Py", &det_Py);
  globChain->SetBranchAddress("x", &det_x);
  globChain->SetBranchAddress("y", &det_y);
  globChain->SetBranchAddress("z", &det_z);
  globChain->SetBranchAddress("t", &det_t);
  globChain->SetBranchAddress("PDGid", &det_PDGid);
  // not implemented
  //globChain->SetBranchAddress("EventID", &det_EventID);
  globChain->SetBranchAddress("TrackID", &det_TrackID);
  globChain->SetBranchAddress("ParentID", &det_ParentID);
  globChain->SetBranchAddress("Weight", &det_Weight);
  globChain->SetBranchAddress("Bx", &det_Bx);
  globChain->SetBranchAddress("By", &det_By);
  globChain->SetBranchAddress("Bz", &det_Bz);
  globChain->SetBranchAddress("Ex", &det_Ex);
  globChain->SetBranchAddress("Ey", &det_Ey);
  globChain->SetBranchAddress("Ez", &det_Ez);
  globChain->SetBranchAddress("ProperTime", &det_ProperTime);
  globChain->SetBranchAddress("PathLength", &det_PathLength);
  globChain->SetBranchAddress("PolX", &det_PolX);
  globChain->SetBranchAddress("PolY", &det_PolY);
  globChain->SetBranchAddress("PolZ", &det_PolZ);
  globChain->SetBranchAddress("InitX", &det_InitX);
  globChain->SetBranchAddress("InitY", &det_InitY);
  globChain->SetBranchAddress("InitZ", &det_InitZ);
  // not implemented
  //globChain->SetBranchAddress("InitPx", &det_InitPx);
  //globChain->SetBranchAddress("InitPy", &det_InitPy);
  //globChain->SetBranchAddress("InitPz", &det_InitPz);
  globChain->SetBranchAddress("InitT", &det_InitT);
  globChain->SetBranchAddress("InitKE", &det_InitKE);
    
  long long int n_entries = globChain->GetEntries();
  long long int i =0;
    
  for (long long int entry = 0; entry < n_entries; ++entry)
    {
      globChain->GetEntry(entry);
      if(entry%1000000 == 0){
        float percdone = 100.*(float)entry/n_entries;
        std::cout<<" Processing: "<<percdone<<"% \r"<<std::flush;
      }
        
      Ptot=sqrt(pow(det_Px,2)+pow(det_Py,2)+pow(det_Pz,2));
      if ((TMath::Abs(cut_pdgid) > 0.1) && (det_PDGid != cut_pdgid)) continue;
      if (Ptot>ptot_cut_low && Ptot<ptot_cut_high) {
        det_TrackID=1;
        det_EventID=++i;
        
        outfile << (float)det_x<<" "
                << (float)det_y<<" "
                << (float)det_z<<" "
                << (float)det_Px<<" "
                << (float)det_Py<<" "
                << (float)det_Pz<<" "
                << (float)det_t<<" "
                << (float)det_PDGid<<" "
                << (long long int)(det_EventID)<<" "
                << (float)det_TrackID<<" "
                << (float)det_ParentID<<" "
                << (float)det_Weight<<" "
                << (float)det_Bx<<" "
                << (float)det_By<<" "
                << (float)det_Bz<<" "
                << (float)det_Ex<<" "
                << (float)det_Ey<<" "
                << (float)det_Ez<<" "
                << (float)det_ProperTime<<" "
                << (float)det_PathLength<<" "
                << (float)det_PolX<<" "
                << (float)det_PolY<<" "
                << (float)det_PolZ<<" "
                << (float)det_InitX<<" "
                << (float)det_InitY<<" "
                << (float)det_InitZ<<" "
                << (float)det_InitT<<" "
                << (float)det_InitKE
                << endl;
      } else continue;
    }
  outfile.close();
  std::cout<<"\n"<<std::endl;
  std::cout<<" File Closed"<<std::endl;
  //  gROOT->ProcessLine(".qqq");
}

