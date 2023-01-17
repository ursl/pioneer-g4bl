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
void epics(int mode = 0) {

  string filename = "nada";
  vector<string> fileNames;
  if (0 == mode) {
    fileNames.push_back("magnets_before_20220525.log");
    fileNames.push_back("magnets_until20220528.log");
    fileNames.push_back("magnets.log");
  } else if (1 == mode) {
    fileNames.push_back("magnets.log");
  }
  
  std::string stime("blah");
  ULong64_t utime(0);
  
  TFile *f = TFile::Open("epics2022.root", "RECREATE");
  TTree *T = new TTree("epics", "epics log 2022");
  T->Branch("stime", &stime);
  T->Branch("utime", &utime, "utime/l");
  vector<string> treeVars;

  vector<string> varNames; 
  vector<double> varValues;
  
  T->Branch("varn", &varNames);
  T->Branch("varv", &varValues);
  
  vector<string> allLines;
  string sline, ltime, otime("2012-05-22_16:55:45.167");
  ifstream INS;
  for (unsigned ifile = 0; ifile < fileNames.size(); ++ifile) {
    INS.open(fileNames[ifile]);
    while (getline(INS, sline)) {
      // -- check whether this is already present in allLines
      ltime = sline.substr(0, sline.find(' '));
      if (ltime <= otime) continue;
      allLines.push_back(sline);
      otime = ltime;
    }
    INS.close();
  }

  // -- debug all lines
  if (0) {
    for (unsigned int iline = 0; iline < allLines.size(); ++iline) {
      sline = allLines[iline];
      ltime = sline.substr(0, sline.find(' '));
      cout << ltime << endl;
    }
    return;
  }
  
  int iMAX(-1), cnt(0);
  
  for (unsigned int iline = 0; iline < allLines.size(); ++iline) {
    sline = allLines[iline];
    ++cnt;
    if (iMAX > 0 && cnt == iMAX) break;

    // -- magic line splitting on whitespace
    std::stringstream ss(sline);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> vline(begin, end);

    stime = vline[0];
    
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
      ltime = vline[0];
      utime = str2Time(ltime); 
      string bla = time2Str(static_cast<time_t>(utime));
      cout << "vline.size() = " << vline.size() << " for date = " << vline[0]
           << " ltime = " << ltime
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
void readTree(int ievt = -1, int nevt = 0) {
  TTree *T = (TTree*)gDirectory->Get("epics");
  vector<string> *varNames = 0;
  vector<double> *varValues = 0;
  string *stime = 0;
  ULong64_t utime = 0;
  
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
    nentries = 0;
  }
  if (nevt > 0) {
    nentries = nevt; 
  }
  
  for (Long64_t i = istart; i <= istart + nentries; ++i) {
    b_varNames->GetEntry(i);
    b_varValues->GetEntry(i);
    b_utime->GetEntry(i);
    b_stime->GetEntry(i);

    cout << "----------------------------------------------------------------------" << endl;
    //    cout << "Event " << i << " " << *stime << " " << *utime << endl;
    cout << "Event " << i << " " << *stime << " utime = " << utime << endl;

    for (unsigned int i = 0; i < varNames->size(); ++i) {
      cout << varNames->at(i) << ": " << varValues->at(i) << endl;
    }
  }
}


// ----------------------------------------------------------------------
void findConditions(Long64_t timeStamp,
                    bool tex = false, bool header = true,
                    bool prtSoll = false,
                    string tname  = "epics") {
  TTree *T = (TTree*)gDirectory->Get(tname.c_str());
  if (0 == T) {
    cout << "no tree " << tname << " found" << endl;
    return;
  }
  vector<string> *varNames = 0;
  vector<double> *varValues = 0;
  string *stime = 0;
  ULong64_t utime = 0;
  
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

  bool found(false);
  Long64_t deltaT(0), bestDeltaT(9999), bestIdx(-1);
  // -- get closest match
  for (Long64_t i = 0; i <= nentries; ++i) {
    b_utime->GetEntry(i);
    Long64_t ttime = utime;
    deltaT = TMath::Abs(ttime - timeStamp);
    if (deltaT < bestDeltaT) {
      bestIdx = i;
      bestDeltaT = deltaT;
      found = true;
    }
  }
  
  // -- print it
  b_varNames->GetEntry(bestIdx);
  b_varValues->GetEntry(bestIdx);
  b_utime->GetEntry(bestIdx);
  b_stime->GetEntry(bestIdx);
  cout << "----------------------------------------------------------------------" << endl;
  cout << "Event " << bestIdx << " " << *stime << " utime = " << utime << endl;
  for (unsigned int i = 0; i < varNames->size(); ++i) {
    if (!prtSoll && (string::npos != varNames->at(i).find("_SOL_"))) continue;
    if (tex) {
      string tvar = varNames->at(i);
      tvar = tvar.substr(0, tvar.find('_'));
      if (header) cout << tvar;
      double x = varValues->at(i);
      cout << " &" <<  (TMath::Abs(x)<0.02? "0": Form("%.2f", x));
      if (header) {
        cout << "\\\\" << endl;
      } else {
        cout << endl;
      }
    } else {
      cout << varNames->at(i) << ": " << varValues->at(i) << endl;
    }
  }
  cout << "----------------------------------------------------------------------" << endl;

  if (!found) {
    cout << "timestamp " << timeStamp << " (= "
         << time2Str(static_cast<time_t>(timeStamp))
         << ") not found"
         << endl;
  }
}


// ----------------------------------------------------------------------
void findConditions(string stime,
                    bool tex = false, bool header = true,
                    bool prtSoll = false,
                    string tname  = "epics") {
  
  time_t ts = str2Time(stime);
  findConditions(ts, tex, header, prtSoll, tname);
}
