#include "anaZntuple.hh"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <math.h>

#include "TROOT.h"
#include "TRint.h"
#include "TChain.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TString.h"
#include "TRandom.h"
#include "TUnixSystem.h"
#include "TSystem.h"
#include "TKey.h"

using namespace std;


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %% Usage examples:
// %% bin/runAna -t anaZntuple -f /Users/ursl/data/pioneer/test/g4beamline.root
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int main(int argc, char *argv[]) {

  int processID = gSystem->GetPid();
  cout << "Running under process ID  " << processID << endl;

  // Change the MaxTreeSize to 100 GB (default since root v5.26)
  TTree::SetMaxTreeSize(100000000000ll); // 100 GB

  string fileName("unset"), className("anaZntuple");

  // -- command line arguments
  for (int i = 0; i < argc; i++){
    if (!strcmp(argv[i],"-h")) {
        cout << "List of arguments:" << endl;
        cout << "-f filename   single file instead of chain" << endl;
        cout << "-t name       tree analysis class" << endl;
        cout << "-h            prints this message and exits" << endl;
        return 0;
    }
    if (!strcmp(argv[i],"-f"))  {fileName   = string(argv[++i]); }    // single file instead of chain
    if (!strcmp(argv[i],"-t"))  {className  = string(argv[++i]); }    // set output file
  }


  // -- Now instantiate the tree-analysis class object, initialize, and run it ...
  if (string::npos != className.find("anaZntuple")) {
    cout << "instantiating analysis class  " << className << endl;
    anaZntuple *a = new anaZntuple(fileName);
    delete a; // so we can dump some information in the destructor
  }

  return 0;
}
