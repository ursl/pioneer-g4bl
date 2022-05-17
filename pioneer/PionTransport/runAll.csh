#! /bin/tcsh
setenv ST1 /psi/home/langenegger/data/slurm/pioneer-g4bl/piontransport/

# -- special
setenv BLT1 "p28-muprod0003-BLTrackFile2_PDGid211_DetEMuPiFromTarget.txt"
setenv OUT1 "$ST1/muprod0003-p28_p0001"
setenv JOB  "p28_p0001_muprod0003.i"
run -c ~/mu3e/mu3eanca/slurm/slurm-g4bl-pioneer.csh -r "STORAGE1 $OUT1%G4BLOUTPUTDIR .%G4BLTRACKFILE $BLT1"  $JOB

setenv BLT1 "p28-muprod0002-BLTrackFile2_PDGid211_DetEMuPiFromTarget.txt"
setenv OUT1 "$ST1/muprod0002-p28_p0001"
setenv JOB  "p28_p0001.i"
run -c ~/mu3e/mu3eanca/slurm/slurm-g4bl-pioneer.csh -r "STORAGE1 $OUT1%G4BLOUTPUTDIR .%G4BLTRACKFILE $BLT1"  $JOB

setenv BLT1 "p35-muprod0002-BLTrackFile2_PDGid211_DetEMuPiFromTarget.txt"
setenv OUT1 "$ST1/muprod0002-p35_p0001"
setenv JOB  "p35_p0001.i"
run -c ~/mu3e/mu3eanca/slurm/slurm-g4bl-pioneer.csh -r "STORAGE1 $OUT1%G4BLOUTPUTDIR .%G4BLTRACKFILE $BLT1"  $JOB

setenv BLT1 "p45-muprod0002-BLTrackFile2_PDGid211_DetEMuPiFromTarget.txt"
setenv OUT1 "$ST1/muprod0002-p45_p0001"
setenv JOB  "p45_p0001.i"
run -c ~/mu3e/mu3eanca/slurm/slurm-g4bl-pioneer.csh -r "STORAGE1 $OUT1%G4BLOUTPUTDIR .%G4BLTRACKFILE $BLT1"  $JOB

setenv BLT1 "p55-muprod0002-BLTrackFile2_PDGid211_DetEMuPiFromTarget.txt"
setenv OUT1 "$ST1/muprod0002-p55_p0001"
setenv JOB  "p55_p0001.i"
run -c ~/mu3e/mu3eanca/slurm/slurm-g4bl-pioneer.csh -r "STORAGE1 $OUT1%G4BLOUTPUTDIR .%G4BLTRACKFILE $BLT1"  $JOB

setenv BLT1 "p65-muprod0002-BLTrackFile2_PDGid211_DetEMuPiFromTarget.txt"
setenv OUT1 "$ST1/muprod0002-p65_p0001"
setenv JOB  "p65_p0001.i"
run -c ~/mu3e/mu3eanca/slurm/slurm-g4bl-pioneer.csh -r "STORAGE1 $OUT1%G4BLOUTPUTDIR .%G4BLTRACKFILE $BLT1"  $JOB

setenv BLT1 "p75-muprod0002-BLTrackFile2_PDGid211_DetEMuPiFromTarget.txt"
setenv OUT1 "$ST1/muprod0002-p75_p0001"
setenv JOB  "p75_p0001.i"
run -c ~/mu3e/mu3eanca/slurm/slurm-g4bl-pioneer.csh -r "STORAGE1 $OUT1%G4BLOUTPUTDIR .%G4BLTRACKFILE $BLT1"  $JOB

setenv BLT1 "p85-muprod0002-BLTrackFile2_PDGid211_DetEMuPiFromTarget.txt"
setenv OUT1 "$ST1/muprod0002-p85_p0001"
setenv JOB  "p85_p0001.i"
run -c ~/mu3e/mu3eanca/slurm/slurm-g4bl-pioneer.csh -r "STORAGE1 $OUT1%G4BLOUTPUTDIR .%G4BLTRACKFILE $BLT1"  $JOB
