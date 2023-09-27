# pioneer-g4bl
```
./bin/convertRootToBLTrack2
./bin/convertRootToBLTrack2 -p -13 
./bin/convertRootToBLTrack2 -p -13  -f p0000-files.txt

ls -1 ~/data/slurm/pioneer-g4bl/muprod0001/PIONEERMU_SlantedTgtE_muprod0001-400* > muprod0001-files.txt
./bin/convertRootToBLTrack2 -n muprod0001- -f muprod0001-files.txt -p -13

g4bl G4V7M_piE5_pions.i G4BLTRACKFILE=p0000-BLTrackFile2_PDGid211_DetEMuPiFromTarget.txt
g4bl G4V7M_piE5_pions.i G4BLTRACKFILE=p0000-BLTrackFile2_PDGid211_DetEMuPiFromTarget.txt HSC42cur=0.0

merlin-l-002>cd /psi/home/langenegger/data/g4bl/pioneer-g4bl/pioneer/MuonTransport
merlin-l-002>g4bl m0000.i G4BLTRACKFILE=p0002-BLTrackFile2_PDGid-13_DetEMuPiFromTarget.txt G4BLOUTPUTDIR=m0000
```
