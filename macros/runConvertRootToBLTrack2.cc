// include std libraries
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


using namespace std;

extern void ConvertRootToBLTrack2(string fNameList, string outDirectory, int pdgid, string prefix, string vdetName);

// ----------------------------------------------------------------------
int main(int argc, char *argv[]) {

  cout << "Hallo" << endl;
  
  string dirName("nada")
    , fnameList("files.txt")
    , outdir("/psi/home/langenegger/data/data/pioneer-g4bl/bl2/")
    , name("prefix-")
    , vdet("DetEMuPiFromTarget") /*DetFSH41*/
    ;
  int pdgid(211);

  // -- command line arguments
  for (int i = 0; i < argc; i++){           
    if (!strcmp(argv[i], "-d"))  {dirName = argv[++i];} 
    if (!strcmp(argv[i], "-f"))  {fnameList = argv[++i];} 
    if (!strcmp(argv[i], "-n"))  {name = argv[++i];} 
    if (!strcmp(argv[i], "-o"))  {outdir = argv[++i];} 
    if (!strcmp(argv[i], "-p"))  {pdgid = atoi(argv[++i]);} 
    if (!strcmp(argv[i], "-v"))  {vdet = argv[++i];} 
  }

  if (string::npos == dirName.find("nada")) {
    vector<string> vfiles;
    DIR *folder;
    struct dirent *entry;
    
    folder = opendir(dirName.c_str());
    if(folder == NULL) {
      puts("Unable to read directory");
      return(1);
    } else  {
      puts("Directory is opened!");
    }
    
    while ((entry=readdir(folder))) {
      if (8 == entry->d_type) {
        vfiles.push_back(dirName + "/" + entry->d_name);
        printf("%s type %d\n", entry->d_name, entry->d_type);
      }
    }
    closedir(folder);

    sort(vfiles.begin(), vfiles.end());    

    ofstream o(fnameList);
    for (unsigned int i = 0; i < vfiles.size(); ++i) {
      cout << vfiles[i] << endl;
      o << vfiles[i] << endl;
    }
    o.close();
  
  }
  
  
  if (0 == pdgid) {
    ConvertRootToBLTrack2(fnameList, outdir, -13, name, vdet);
    ConvertRootToBLTrack2(fnameList, outdir, 211, name, vdet);
  }
  
  return 0; 
}
