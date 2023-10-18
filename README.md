# pioneer-g4bl
```
./bin/convertRootToBLTrack2
./bin/convertRootToBLTrack2 -p -13 
./bin/convertRootToBLTrack2 -p -13  -f p0000-files.txt

*Muon production*
ls -1 ~/data/slurm/pioneer-g4bl/muprod0001/PIONEERMU_SlantedTgtE_muprod0001-400* > muprod0001-files.txt
./bin/convertRootToBLTrack2 -n muprod0001- -f muprod0001-files.txt -p -13

*How to set a parameter from the command line*
g4bl G4V7M_piE5_pions.i G4BLTRACKFILE=p0000-BLTrackFile2_PDGid211_DetEMuPiFromTarget.txt HSC42cur=0.0

merlin-l-002>cd /data/user/langenegger/g4bl/pioneer-g4bl
merlin-l-002>source setup.csh

*Pion transport*
merlin-l-002>cd /data/user/langenegger/g4bl/pioneer-g4bl/pioneer/beamline
merlin-l-l002>g4bl G4V7M_piE5_pions.i G4BLTRACKFILE=p0002-BLTrackFile2_PDGid211_DetEMuPiFromTarget.txt G4BLOUTPUTDIR=d0002  >& d0002/g4bl.log

```

https://github.com/ursl/mu3eanca/tree/master/slurm

replace /psi/home/langenegger/data with /data/user/langenegger

*Batch submission*
```
replicate -f 40000 -l 40020 -p randomseed -t PIONEER_SlantedTgtE_prod-40000.i
run -c ~/mu3e/mu3eanca/slurm/slurm-g4bl-pioneer.csh -r 'STORAGE1 /psi/home/langenegger/data/slurm/pioneer-g4bl/prod0000%SITE T3_CH_PSI' PIONEER_SlantedTgtE_prod0000-4000*

run -c ~/mu3e/mu3eanca/slurm/slurm-g4bl-pioneer.csh -r 'STORAGE1 /psi/home/langenegger/data/slurm/pioneer-g4bl/muontransport/m0000%G4BLOUTPUTDIR .%G4BLTRACKFILE p0000-BLTrackFile2_PDGid211_DetEMuPiFromTarget.txt' m0000.i

run -c ~/mu3e/mu3eanca/slurm/slurm-g4bl-pioneer.csh -r 'STORAGE1 /psi/home/langenegger/data/slurm/pioneer-g4bl/transport/mu0001%G4BLOUTPUTDIR .%G4BLTRACKFILE p28-muprod0003-BLTrackFile2_PDGid-13_DetEMuPiFromTarget.txt%MOMENTUM 28' mu0001.i
```
