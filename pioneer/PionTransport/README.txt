This simulation relies on files and definition from the original PiE5 simulation such as Positions.txt, feldxxx.dat, Scalefactors.txt
All file paths need to be modified to reflect your directory structure.

This simulations requires one of two BLTrackFile2 beam files to be generated prior to running.
Either particles just downstream of Target E labeled BEAM LOCATION 1, or downstream of the ASC41 exit aperture labeled BEAM LOCATION 2. Search "BEAM LOCATION" in the g4bl script to find where these are defined.

The AST41,ASC41, QSB and QSK triplets have optimized values defined at the top of the g4bl script. These should be removed if you plan to tune the magnet current parameters as they are defined and not modifiable.

to run this g4bl script
$> g4bl G4V7M_piE5_pions.i

the simulation will produce three output files
CALOENTR.txt
## a 200 mm radius virtual detector placed at the entrance to the calorimeter z=17199.0
CALOCNTR.txt
## a 49.5 mm radius virutal detector place at the center of the calorimeter z=180000.0
profile.txt
## a g4bl profile product, each line is 10 mm along the centerline coordinates with calculated beam size and positions

To produce a singular ROOT file instead of ascii files, append an 'r' on the place commands for these detectors.
Example:
place CALOCNTR z=$posATAR
becomes
place CALOCNTRr z=$posATAR
