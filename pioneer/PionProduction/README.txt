This simulation relies on files and definition from the original PiE5 simulation such as Positions.txt, feldxxx.dat, Scalefactors.txt
All file paths need to be modified to reflect your directory structure.


to run this g4bl script
$> g4bl PIONEER_SlantedTgtE_prod.i histoFile=hist_PIONEER_SlantedTgtE_prod.root

The simulation benefits greatly from running on the computing cluster and using the mpi platform
To run this g4bl script with parallel mpi
$> g4blmpi 21 PIONEER_SlantedTgtE_prod.i histoFile=hist_PIONEER_SlantedTgtE_prod

The simulation will produce one ROOT output file the name is determined by the histoFile parameter passed in the command.
The virtual detector DetEMuPiFromTarget has 300 mm radius and is placed 150 mm downstream (in the direction of PiE5, 165 degrees relative to proton beam axis).
The virtual detector DetFSH41 has 250 mm radius and is placed at the first horizontal slit system.


The following parameters in the script define the number of events and the first event, in order to run multiple independent simulations that can be combined later.
param -unset first=1
param -unset numEvents=100000000
param -unset last=$first+$numEvents-1
