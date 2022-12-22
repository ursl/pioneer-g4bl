// include std libraries
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <map>

#include "TString.h"

using namespace std;

bool gVerbose(false);
bool gHash(true);

// -- memory hog:   map<string, int> 18Gb RSS for 69M lines
//    next version: map<size_t, int> 4GB RSS for 69M lines with a hash
// ----------------------------------------------------------------------
void checkDuplicates(string filename) {
  map<string, int> allLines; 
  map<size_t, int> allLinesHash; 

  ifstream INS;
  string sline;
  INS.open(filename);
  long iLines(0), iDuplicates(0);
  while (getline(INS, sline)) {
    if (gHash) {
      allLinesHash[std::hash<std::string>{}(sline)]++;
    } else {
      allLines[sline]++;
    }
    ++iLines; 
    if (0 == iLines%1000) {
      if (gVerbose) {
        cout << Form("line %10ld", iLines) << endl;
      } else {
        cout << "\r" << Form("line %10ld", iLines) << flush;
      }
    }
  }

  if (gHash) {
    auto lastLine = allLinesHash.end();
    for (auto it = allLinesHash.begin(); it != lastLine; ++it) {
      if (it->second != 1) {
        ++iDuplicates;
        if (gVerbose) cout << it->first << " (" << it->second << ")" << endl;
      }
    }
  } else {
    auto lastLine = allLines.end();
    for (auto it = allLines.begin(); it != lastLine; ++it) {
      if (it->second != 1) {
        ++iDuplicates;
        if (gVerbose) cout << it->first << " (" << it->second << ")" << endl;
      }
    }
  } 
    
  cout << endl;
  cout << "Counted a total of " << iDuplicates << " lines that appeared at least twice among "
       << iLines << " lines (fraction = " << static_cast<double>(iDuplicates)/iLines << ")"
       << endl;
  cout << "Used " << (gHash? "hash": "no hash") << " version" << endl;
}


// ----------------------------------------------------------------------
int main(int argc, char *argv[]) {

  string filename("BLTrackFile2_PDGid211_DetEMuPiFromTarget.txt") ;

  // -- command line arguments
  for (int i = 0; i < argc; i++){           
    if (!strcmp(argv[i], "-f"))  {filename = argv[++i];} 
    if (!strcmp(argv[i], "-h0")) {gHash = false;} 
    if (!strcmp(argv[i], "-v"))  {gVerbose = true;} 
  }
  
  checkDuplicates(filename);
  
  return 0; 
}
