// include std libraries
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


using namespace std;

extern void ConvertRootToBLTrack2(string fNameList, string outDirectory, string vdetName);

// ----------------------------------------------------------------------
int main(int argc, char *argv[]) {

  cout << "Hallo" << endl;
  
  string fnameList("files.txt")
    , outdir("/psi/home/langenegger/data/data/pioneer-g4bl/bl2/")
    , vdet("DetEMuPiFromTarget") /*DetFSH41*/
    ;

  // -- command line arguments
  for (int i = 0; i < argc; i++){           
    if (!strcmp(argv[i], "-f"))  {fnameList = argv[++i];} 
    if (!strcmp(argv[i], "-o"))  {outdir = argv[++i];} 
    if (!strcmp(argv[i], "-v"))  {vdet = argv[++i];} 
  }
  
  ConvertRootToBLTrack2(fnameList, outdir, vdet);
  
  return 0; 
}
