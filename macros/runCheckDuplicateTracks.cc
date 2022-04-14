// include std libraries
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>


using namespace std;

// ----------------------------------------------------------------------
void checkDuplicates(string filename) {
  map<string, int> allLines; 

  ifstream INS;
  string sline;
  INS.open(filename);
  while (getline(INS, sline)) {
    if (allLines.find(sline) != allLines.end()) {
      allLines[sline]++;
      cout << "duplicate line: " << endl;
      cout << sline << endl; 
    } else {
      allLines.insert(make_pair(sline, 1));
    }
  }

  // FIXME print duplicate lines 

}


// ----------------------------------------------------------------------
int main(int argc, char *argv[]) {

  string filename("BLTrackFile2_PDGid211_DetEMuPiFromTarget.txt") ;

  // -- command line arguments
  for (int i = 0; i < argc; i++){           
    if (!strcmp(argv[i], "-f"))  {filename = argv[++i];} 
  }
  
  checkDuplicates(filename);
  
  return 0; 
}
