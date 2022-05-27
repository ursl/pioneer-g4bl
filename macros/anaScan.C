#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdarg>


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
void graphExtrema(TGraph *gr, double& xmin, double& xmax, double& ymin, double& ymax) {
  xmin = ymin = 99999.;
  xmax = ymax = -99999.;
  for (int i = 0; i < gr->GetN(); ++i) {
    double x = gr->GetPointX(i); 
    double y = gr->GetPointY(i); 
    if (x > xmax) xmax = x;
    if (y > ymax) ymax = y;
    if (x < xmin) xmin = x;
    if (y < ymin) ymin = y;
    //    cout << "i = " << i << " ymin = " << ymin << " ymax = " << ymax << endl;
  }
  cout << "graphExtrema ymin = " << ymin <<  " ymax = " << ymax << endl;
}

// ----------------------------------------------------------------------
TGraph* t2g(TTree *t, string sy, string sx, double offsetX = 0.) {
  double valx, valy;
  t->SetBranchAddress(sy.c_str(), &valy);
  t->SetBranchAddress(sx.c_str(), &valx);

  long long int n_entries = t->GetEntries();
  long long int i(0);

  TGraph *gr = new TGraph();
  
  for (long long int entry = 0; entry < n_entries; ++entry) {
    t->GetEntry(entry);
    gr->AddPoint(valx + offsetX, valy);
  }

  cout << "DBX gr: " << gr->GetPointX(0) << "  " << gr->GetPointY(0) << endl;
  
  return gr;
}

// ----------------------------------------------------------------------
void anaScan(string var1 = "meanX", double z = 18000., int n = 0, ...) {
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
    
    gStyle->SetOptTitle(0);
    
    
    TTree *t = fillTree(filename);
    t->Print();

  }    
}

