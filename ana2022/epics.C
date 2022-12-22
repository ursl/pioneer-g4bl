#include <fstream>

// ----------------------------------------------------------------------
//
// epics convert the logfiles of Dieter's epics bridge into a root tree
// =====
//
//
// Usage: epics(filename)
//
// History 2022/09/01 first shot
//         
// ----------------------------------------------------------------------
// Send all questions, wishes and complaints to the 
//
// Author    Urs Langenegger <urslangenegger@gmail.com>
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
string time2Str(time_t temp) {
  // stringstream strm;
  // strm << t;
  // return strm.str();
  
  std::tm* t = std::gmtime(&temp);
  std::stringstream ss; // or if you're going to print, just input directly into the output stream
  ss << std::put_time(t, "%Y-%m-%d %I:%M:%S %p");
  std::string output = ss.str();
  return output;
}

// ----------------------------------------------------------------------
time_t str2Time(string stime) {
  struct tm tm1;
  time_t epoch;
  if (strptime(stime.c_str(), "%Y-%m-%d_%H:%M:%S", &tm1) != NULL ) {
    epoch = mktime(&tm1);
  } else {
    cout << "something went wrong" << endl;
  }
  return epoch;
}


// ----------------------------------------------------------------------
void epics(string filename = "magnets.log") {

  std::string stime("blah");
  ULong64_t utime(0);
  
  TFile *f = TFile::Open("epics.root", "RECREATE");
  TTree *T = new TTree("epics", "epics log 2022");
  T->Branch("stime", &stime);
  T->Branch("utime", &utime, "utime/l");
  vector<string> treeVars;

  vector<string> varNames; 
  vector<double> varValues;
  
  T->Branch("varn", &varNames);
  T->Branch("varv", &varValues);
  
  ifstream INS;
  string sline;
  INS.open(filename);
  int MAX(-1), cnt(0);
  
  while (getline(INS, sline)) {
    ++cnt;
    if (MAX > 0 && cnt == MAX) break;

    // -- magic line splitting on whitespace
    std::stringstream ss(sline);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> vline(begin, end);

    // -- check whether this is a var declaration line
    cout << "vline[2] = " << vline[2] << endl;
    if ("AHSW41" == vline[2]) {
      varNames.clear();
      for (unsigned int i = 3; i < vline.size(); ++i) {
        if (treeVars.end() ==  find(treeVars.begin(), treeVars.end(), vline[i])) {
          replaceAll(vline[i], ":", "_");
          varNames.push_back(vline[i]);
          cout << "adding " << vline[i] << " at varNames.size() = " << varNames.size()
               << " as " <<  varNames[varNames.size()-1]
               << endl;
        }
      }
      continue;
    }
    
    varValues.clear();
    for (unsigned int i = 3; i < vline.size(); ++i) {
      if (string::npos == vline[i].find("None")) {
        varValues.push_back(stod(vline[i]));
      } else {
        varValues.push_back(nan(""));
      }
    }
    
    
    // -- debug printout
    if (1) {
      stime = vline[0];
      utime = str2Time(stime); 
      string bla = time2Str(static_cast<time_t>(utime));
      cout << "vline.size() = " << vline.size() << " for date = " << vline[0]
           << " stime = " << stime
           << " utime = " << utime
           << " bla = " << bla
           << endl;
      for (unsigned int i = 0; i < vline.size(); ++i) {
        cout << vline[i] << " "; 
      }
      cout << endl;
    }

    // -- Fill the tree
    T->Fill();
  }

  T->Write();
  f->Write();
  f->Close();
}



// ----------------------------------------------------------------------
void readTree(int ievt = -1) {
  TTree *T = (TTree*)gDirectory->Get("epics");
  vector<string> *varNames = 0;
  vector<double> *varValues = 0;
  string *stime = 0;
  ULong64_t *utime = 0;
  
  TBranch *b_utime = 0; 
  TBranch *b_stime = 0;
  TBranch *b_varNames = 0;
  TBranch *b_varValues = 0;

  T->SetBranchAddress("utime", &utime,     &b_utime);
  T->SetBranchAddress("stime", &stime,     &b_stime);
  T->SetBranchAddress("varn",  &varNames,  &b_varNames);
  T->SetBranchAddress("varn",  &varNames,  &b_varNames);
  T->SetBranchAddress("varv",  &varValues, &b_varValues);

  Long64_t nentries(T->GetEntries());
  Long64_t istart(0);
  
  if (ievt > -1) {
    istart = ievt;
    nentries = 1;
  }

  for (Long64_t i = istart; i < nentries; ++i) {
    b_varNames->GetEntry(i);
    b_varValues->GetEntry(i);
    b_utime->GetEntry(i);
    b_stime->GetEntry(i);

    cout << "----------------------------------------------------------------------" << endl;
    //    cout << "Event " << i << " " << *stime << " " << *utime << endl;
    cout << "Event " << i << " " << *stime << endl;

    for (unsigned int i = 0; i < varNames->size(); ++i) {
      cout << varNames->at(i) << ": " << varValues->at(i) << endl;
    }

    
  }
  
}
