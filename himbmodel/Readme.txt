This includes the parametrized model for the pion production cross-sections into G4beamline. The corresponding physics list is called QGSP_BIC_HIMB_HP (_EMX, _EMV modifiers are available).

The code also adds a named argument “splittingFactor” to g4bl’s physics command. This sets the number of pions that are created at each pion production vertex. A typical value is 100. At the same time this also splits the muon decay by the same amount.

Use the CopyToDest script to copy the files into the G4beamline source directory and compile G4beamline. If you have already compiled G4beamline and do not want to recompile everything simply remove the “.complete” file in the directory build/geant4).

Example for calling the physics model in g4bl:
physics QGSP_BIC_HIMB_HP

Example for calling the physics model with splitting in g4bl:
physics QGSP_BIC_HIMB_HP_BIAS splittingFactor=100

Example for calling the physics model using hybrid physics list and splitting in g4bl:
physics QGSP_BIC_HIMB_HYBRID_HP_BIAS splittingFactor=100
