#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdarg>

struct profileData {
  //Declaration of leaves types
  Double_t        Z;
  Double_t        N;
  Double_t        meanX;
  Double_t        sigmaX;
  Double_t        meanY;
  Double_t        sigmaY;
  Double_t        emitX;
  Double_t        emitY;
  Double_t        emitTrans;
  Double_t        betaX;
  Double_t        betaY;
  Double_t        betaTrans;
  Double_t        alphaX;
  Double_t        alphaY;
  Double_t        alphaTrans;
  Double_t        meanP;
};

// ----------------------------------------------------------------------
struct profileData readData(double z, TTree *profile) {

  struct profileData data;
  
  // Set branch addresses.
  profile->SetBranchAddress("Z", &data.Z);
  profile->SetBranchAddress("N", &data.N);
  profile->SetBranchAddress("meanX", &data.meanX);
  profile->SetBranchAddress("sigmaX", &data.sigmaX);
  profile->SetBranchAddress("meanY", &data.meanY);
  profile->SetBranchAddress("sigmaY", &data.sigmaY);
  profile->SetBranchAddress("emitX", &data.emitX);
  profile->SetBranchAddress("emitY", &data.emitY);
  profile->SetBranchAddress("emitTrans", &data.emitTrans);
  profile->SetBranchAddress("betaX", &data.betaX);
  profile->SetBranchAddress("betaY", &data.betaY);
  profile->SetBranchAddress("betaTrans", &data.betaTrans);
  profile->SetBranchAddress("alphaX", &data.alphaX);
  profile->SetBranchAddress("alphaY", &data.alphaY);
  profile->SetBranchAddress("alphaTrans", &data.alphaTrans);
  profile->SetBranchAddress("meanP", &data.meanP);
  
  Long64_t nentries = profile->GetEntries();
  int nbytes(0);
  
  for (Long64_t i=0; i<nentries;i++) {
    nbytes += profile->GetEntry(i);
    // cout << "data.Z = " << data.Z << " TMath::Abs(data.Z - z) = " << TMath::Abs(data.Z - z) << endl;
    if (TMath::Abs(data.Z - z) < 1e-3) {
      return data;
    }
  }
  
  return data; 
}




//----------------------------------------------------------------------
TTree* fillTree(string filename) {
  int VERBOSE(0);
  ifstream INS;
  vector<string> vlines; 
  string sline;
  INS.open(filename);
  while (getline(INS, sline)) {
    vlines.push_back(sline);
  }

  // -- parse beginning to set up tree
  int valFirstLine(1);
  string tname = vlines[0].substr(1); 
  if (string::npos != vlines[0].find("g4beamline profile")) {
    tname = vlines[0].substr(2); 
    valFirstLine = 3;
  }
  string dname = tname.substr(tname.find(" ") + 1); 

  vector<string> vars;
  map<string, double*> varDouble;
  map<string, int*> varInt;

  istringstream istring(vlines[1].substr(1)); 
  istring >> sline; 
  if (VERBOSE > 0) cout << "adding ";
  while (!istring.fail()) {
    if (VERBOSE > 0) cout << sline << " "; 
    vars.push_back(sline);
    varDouble.insert(make_pair(sline, new double));
    istring >> sline; 
  }
  if (VERBOSE > 0) cout << endl;

  // -- set up tree  
  TTree *t = new TTree(dname.c_str(), tname.c_str());
  for (unsigned int i = 0; i < vars.size(); ++i) {
    if (VERBOSE > 1) cout << "vars[" << i << "] = " << vars[i] << ": "; 
    if (varDouble.end() != varDouble.find(vars[i])) {
      if (VERBOSE > 1) cout << varDouble[vars[i]] << " (double)";
      t->Branch(vars[i].c_str(), varDouble[vars[i]], Form("%s/D", vars[i].c_str()));
    }
    if (VERBOSE > 1) cout << endl;
  }

  // -- read rest of file and fill into tree
  for (unsigned int il = valFirstLine; il < vlines.size(); ++il) {
    if (VERBOSE > 9) cout << "line " << il << endl;
    istringstream values(vlines[il]); 
    for (unsigned int i = 0; i < vars.size(); ++i) {
      if (varDouble.end() != varDouble.find(vars[i])) {
        values >> *varDouble[vars[i]]; 
        if (VERBOSE > 9) cout << "parsed " << vars[i] << " = " << *varDouble[vars[i]] << endl;
      }
    }
    t->Fill();
  }
  
  return t; 
}

// ----------------------------------------------------------------------
TH1D* anaScan(string var1 = "meanX", double z = 18000., string scan = "QSK41set", int n = 0, ...) {
  TH1D *h1 = new TH1D("h1", "", n, 0., n);
  
  va_list vl;
  va_start(vl, n);
  vector<string> vfiles;
  string as("");
  for (int i = 0; i < n; ++i) {
    char *ac = va_arg(vl, char *);
    as = string(ac);
    vfiles.push_back(ac);
  }
  va_end(vl);


  
  for (unsigned int i = 0; i < vfiles.size(); ++i) {
    cout << vfiles[i] << endl;
    // -- parse bin label from filenames based on scan
    string blabel = vfiles[i];
    rmPath(blabel);
    string::size_type foundpos = blabel.find(scan);
    if (foundpos != string::npos)  blabel.erase(blabel.begin(), blabel.begin()+foundpos);
    foundpos = blabel.find("-pi");
    if (foundpos != string::npos)  blabel.erase(blabel.begin() + foundpos, blabel.end());
    foundpos = blabel.find("-mu");
    if (foundpos != string::npos)  blabel.erase(blabel.begin() + foundpos, blabel.end());
    foundpos = blabel.find("-pr");
    if (foundpos != string::npos)  blabel.erase(blabel.begin() + foundpos, blabel.end());
    foundpos = blabel.find("-el");
    if (foundpos != string::npos)  blabel.erase(blabel.begin() + foundpos, blabel.end());
    cout << "->" << blabel << endl;
    vector<string> ssplit  = split(blabel, '=');
    h1->GetXaxis()->SetBinLabel(i+1, ssplit[1].c_str());
    h1->GetXaxis()->SetTitle(ssplit[0].c_str());

    string filename = vfiles[i];
    if (string::npos != filename.find("~")) {
      string home = gSystem->Getenv("HOME");
      filename.replace(filename.find("~"), 1, home);
    }
    string pdfname = filename; 
    if (string::npos != pdfname.rfind("/")) {
      pdfname.replace(pdfname.begin(), pdfname.begin() + pdfname.rfind("/") + 1, "");
    }
    
    if (string::npos != pdfname.find(".dat")) {
      pdfname.replace(pdfname.find(".dat"), 4, "");
    }
    if (string::npos != pdfname.find(".txt")) {
      pdfname.replace(pdfname.find(".txt"), 4, "");
    }
    cout << "pdfname: ->" << pdfname << "<-" << endl;
    
    gStyle->SetOptTitle(1);
    gStyle->SetOptStat(0);
    
    
    TTree *t = fillTree(filename);
    struct profileData a = readData(z, t);
    cout << "z = " << a.Z <<  " N =  " << a.N << endl;
    h1->SetBinContent(i+1, a.N);
  }    
  gPad->SetLeftMargin(0.15);
  //  h1->GetYaxis()->SetTitle(var1.c_str());
  h1->GetYaxis()->SetTitle((var1 + Form(" (at z = %5.0f mm)", z)).c_str());
  return h1;
}


// ----------------------------------------------------------------------
TH1D* cmpMulti(string var1 = "meanX", double z = 18000., int n = 2, ...) {
  TH1D *h1 = new TH1D("h1", "", n, 0., n);
  
  va_list vl;
  va_start(vl, n);
  vector<string> vfiles;
  string as("");
  for (int i = 0; i < n; ++i) {
    char *ac = va_arg(vl, char *);
    as = string(ac);
    vfiles.push_back(ac);
  }
  va_end(vl);


  
  for (unsigned int i = 0; i < vfiles.size(); ++i) {
    cout << vfiles[i] << endl;
    string blabel = vfiles[i];
    rmPath(blabel);
    h1->GetXaxis()->SetBinLabel(i+1, blabel.c_str());

    string filename = vfiles[i];
    if (string::npos != filename.find("~")) {
      string home = gSystem->Getenv("HOME");
      filename.replace(filename.find("~"), 1, home);
    }
    string pdfname = filename; 
    if (string::npos != pdfname.rfind("/")) {
      pdfname.replace(pdfname.begin(), pdfname.begin() + pdfname.rfind("/") + 1, "");
    }
    
    if (string::npos != pdfname.find(".dat")) {
      pdfname.replace(pdfname.find(".dat"), 4, "");
    }
    if (string::npos != pdfname.find(".txt")) {
      pdfname.replace(pdfname.find(".txt"), 4, "");
    }
    cout << "pdfname: ->" << pdfname << "<-" << endl;
    
    gStyle->SetOptTitle(1);
    gStyle->SetOptStat(0);
    
    
    TTree *t = fillTree(filename);
    struct profileData a = readData(z, t);
    cout << "z = " << a.Z <<  " N =  " << a.N << endl;
    h1->SetBinContent(i+1, a.N);
  }    
  gPad->SetLeftMargin(0.15);
  h1->GetYaxis()->SetTitle((var1 + Form(" (at z = %5.0f mm)", z)).c_str());
  return h1; 
}



// ----------------------------------------------------------------------
void qsk41scan(double z = 17999.) {
  TH1D *h1 = anaScan("N", z, "QSK41set"
                     , 3
                     , "/Users/ursl/data/pioneer/slurm/transport/p55-QSK41set=-0.64-pi0001/p55-QSK41set=-0.64-pi0001-profile-211.txt"
                     , "/Users/ursl/data/pioneer/slurm/transport/p55-QSK41set=-0.68-pi0001/p55-QSK41set=-0.68-pi0001-profile-211.txt"
                     , "/Users/ursl/data/pioneer/slurm/transport/p55-QSK41set=-0.72-pi0001/p55-QSK41set=-0.72-pi0001-profile-211.txt"
                     );
  
  h1->Draw();
  h1->SetMinimum(0.);
  c0.SaveAs("anaScan-N-QSK41scan.pdf");
}

// ----------------------------------------------------------------------
void tripletOff(double z = 17999.) {
  TH1D *h1 = cmpMulti("N", z 
                     , 2
                     , "/Users/ursl/data/pioneer/slurm/transport/p65-muprod0002-initial-pi0001/p65-muprod0002-initial-pi0001-profile-211.txt"
                     , "/Users/ursl/data/pioneer/slurm/transport/p65-muprod0002-tripletOff-pi0003/p65-muprod0002-tripletOff-pi0003-profile-211.txt"
                     );
  
  h1->Draw();
  h1->SetMinimum(0.);
  h1->GetXaxis()->SetBinLabel(1, "default");
  h1->GetXaxis()->SetBinLabel(2, "triplet off");
  c0.SaveAs("cmpTwo-N-tripletOff.pdf");
}
