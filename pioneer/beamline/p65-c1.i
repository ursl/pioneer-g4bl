# ------------------------------------------------------------------
# -- PIONEER_65MeVc_plus_20220530_2000_good_tune_with_flipped_QSK.sp
# -- https://maxwell.npl.washington.edu/elog/pienuxe/Run+2022/132
# -- raw currents, modified to match G4V7M_piE5_pions.i
# ------------------------------------------------------------------
param AHSW41curraw=226.4
param QSF41curraw=-216.473
param HSC41curraw=-43.0     #-39.457
param QSF42curraw=194.964
param QSF43curraw=-150.865
param HSC42curraw=68.3      #0
param QSF44curraw=190.309
param QSF45curraw=199.606
param HSC43curraw=107.0     #113.729
param QSF46curraw=-189.246
param QSF47curraw=219.545
param HSC44curraw=-22.0     #0
param QSF48curraw=-119.494
param AST41curraw=-227.5
param ASC41curraw=253.95
#param SSL41curraw=0
param QSB41curraw=87.641
param QSB42curraw=-111.408
param QSB43curraw=34.815
param SEP41curraw=-47
param QSK41curraw=-29.67
param QSK42curraw=74.86
param QSK43curraw=-58.5

##################################################################################
param -unset momentum=65.
param scaleMom=$momentum/28.
##################################################################################

physics QGSP_BERT_EMY disable=Decay

param eventTimeLimit=1000
param M=938.272 KE=585.00
param P=sqrt(($M+$KE)*($M+$KE)-$M*$M)

trackcuts kineticEnergyCut=0.0
randomseed Set 7330117                                                
param -unset first=1
param -unset numEvents=10000
param -unset lastEvent=$first+$numEvents-1

  
######################## Reference Beam ##########################
reference particle=mu+ referenceMomentum=$momentum beamX=0 beamY=0 beamZ=150 meanXp=-0.0167 meanYp=0

######################## Visualiztion helpers ##########################
g4ui when=4 "/vis/viewer/set/viewpointThetaPhi 180 0."
g4ui when=4 "/vis/viewer/set/background 1 1 1"
g4ui when=4 "/vis/viewer/addCutawayPlane 0 0 0 m 0 0 -1"
g4ui when=4 "/vis/viewer/addCutawayPlane 0 0 0 m -1 0 0"
g4ui when=4 "/vis/viewer/addCutawayPlane 0 0 0 m 0 -1 0"

box dummy height=2000 length=1 width=6000 color=invisible

######################## Target E ##########################
material TargetEGraphite a=12 z=6 density=1.8
param targetMaterial=TargetEGraphite
param targetYRotation=45.-15.
param targetZ=0.
param targetX=0.
param targetY=0.
include "$G4BLPIONEER/pioneer/TargetE/polyconeTarget.txt"
place TargetCone z=($targetZ+$Ravg*sin($targetYRotation*deg)) x=-1*($targetX+$Ravg*cos($targetYRotation*deg)) y=$targetY rotation=Y(180+$targetYRotation) coordinates=global color=0,0,0

tubs killCyl innerRadius=500 radius=510 length=750 kill=1 color=1,1,1,0.2
tubs killCap radius=500 length=1 kill=1 color=1,1,1,0.2
place killCyl z=125
place killCap z=-250



#######################################################################################################################
# -- pencil beam
#param ZPOS=200
#beam particle=pi+ nEvents=10000 x=0.0 y=0.0 z=$ZPOS P=65.0 sigmaP=10.1 sigmaX=10.1 sigmaY=10.1
#beam particle=mu+ nEvents=10000 x=0.0 y=0.0 z=$ZPOS P=65.0 sigmaP=10.1 sigmaX=10.1 sigmaY=10.1
#beam particle=e+  nEvents=10000 x=0.0 y=0.0 z=$ZPOS P=65.0 sigmaP=10.1 sigmaX=10.1 sigmaY=10.1


################################# Elements Strengths ####################################
param -unset sign=1 		       # sign=-1 for backtracking ::: sign=1 for normal DS tracking
# currents according to setpoint
include "$G4BLPIONEER/pie5/CurrentsCMBL2018.txt" 		    
# Deviations between Soll and Ist value that is corrected in the Scalefactors.txt
include "$G4BLPIONEER/pie5/DeltaCurrents.txt"  
# scaling factors for fieldmaps/generic elements/fieldexpressions
include "$G4BLPIONEER/pie5/Scalefactors.txt" 	 
# Positions generated with Mathematica Notebook pathlength from Inventor model
include "$G4BLPIONEER/pie5/Positions.txt"		    
include "$G4BLPIONEER/pioneer/positions/PositionsPIONEER.txt"
#########################################################################################
									

#########################################################################################
####################################### Elements ########################################
#########################################################################################
									
######################################### AHSW ##########################################
fieldmap AHSW41 file="$G4BLPIONEER/pie5/feldahsw.dat"
#########################################################################################

######################################### QSM ###########################################
fieldmap QSM41 file="$G4BLPIONEER/pie5/feldqsm.dat"
genericquad QSM41DUMMY gradient=0 kill=1 ironRadius=600 ironLength=500 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=200 coilRadius=250 coilHalfwidth=80
#########################################################################################

######################################### ASK ###########################################
fieldmap ASK41 file="$G4BLPIONEER/pie5/feldask.dat" current=$ASK41set #*1.07 # From TOSCA generated end of 2014
genericbend ASK41DUMMY fieldWidth=800 fieldHeight=240 By=0 fieldLength=1 ironWidth=1780 ironHeight=1000 ironLength=500 ironColor=0,0,1,0.6 kill=1
#########################################################################################

######################################### QSO ###########################################
fieldmap QSO file="$G4BLPIONEER/pie5/feldqso.dat" # measured fieldmap from Vjeran -     x=+-9cm    y=+-10cm
tubs QSODUMMY innerRadius=125 outerRadius=200 length=400 material=Vacuum kill=1 color=1,0,0,0.5
#########################################################################################

######################################### ASL ###########################################
fieldmap ASL41 file="$G4BLPIONEER/pie5/feldasl.dat" current=$ASL41set #*1.07 # From TOSCA generated end of 2014
genericbend ASL41DUMMY fieldWidth=800 fieldHeight=180 By=0 fieldLength=1 ironWidth=1780 ironHeight=1000 ironLength=1000 ironColor=0,0,1,0.3 kill=1
#########################################################################################

######################################### SML ###########################################
fieldexpr SML41 height=320 width=320 length=398 By=$SML41set
tubs SML41DUMMY innerRadius=160 outerRadius=162 length=398 material=Vacuum color=0,0,1,0.6
tubs MEGCOL innerRadius=60 outerRadius=159 length=50 material=Pb color=1,0,1,0.8
#########################################################################################

######################################### QSK ###########################################
#genericquad QSK kill=1 ironRadius=500 ironLength=400 ironColor=1,0,0,0.6 fieldLength=390 poleTipRadius=125 coilRadius=160 coilHalfwidth=55 fringeFactor=0.68
fieldmap QSK file="$G4BLPIONEER/pie5/feldqsk.dat"
genericquad QSK41DUMMY kill=1 ironRadius=500 ironLength=400 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=125 coilRadius=160 coilHalfwidth=55
genericquad QSK42DUMMY kill=1 ironRadius=500 ironLength=400 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=125 coilRadius=160 coilHalfwidth=55
genericquad QSK43DUMMY kill=1 ironRadius=500 ironLength=400 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=125 coilRadius=160 coilHalfwidth=55
#########################################################################################

######################################## SEP41 ##########################################
fieldmap SEP41E195 file="$G4BLPIONEER/pie5/feldsep41E195_fine.dat"
fieldmap SEP41B45 file="$G4BLPIONEER/pie5/feldsep41B45_newfine.dat"
genericbend SEP41_plates fieldWidth=400 fieldHeight=190 By=0 fieldLength=1 ironWidth=800 ironHeight=600 ironLength=800 ironColor=0,0,1,0.7 kill=1
#########################################################################################

#########################################################################################

######################################### QSB ###########################################
fieldmap QSB file="$G4BLPIONEER/pie5/feldqsb.dat"
genericquad QSB41DUMMY kill=1 ironRadius=500 ironLength=270 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=125 coilRadius=190 coilHalfwidth=55
genericquad QSB42DUMMY kill=1 ironRadius=500 ironLength=270 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=125 coilRadius=190 coilHalfwidth=55
genericquad QSB43DUMMY kill=1 ironRadius=500 ironLength=270 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=125 coilRadius=190 coilHalfwidth=55
#########################################################################################

#################################### ASCapertOut #######################################
tubs ASCapertOut innerRadius=160 outerRadius=350 length=100 material=Fe kill=1 color=0,0,1,0.8 
#########################################################################################

######################################### ASC ###########################################
fieldmap ASCfieldmap file="$G4BLPIONEER/pie5/feldasc.dat" current=-1
box ASCdummy height=200 length=1200 width=1200 color=0,0,1,0.6 kill=1
#########################################################################################

#################################### flangeASTASC #######################################
tubs flangeASTASC innerRadius=160 outerRadius=500 length=60 material=Fe kill=1 color=0,0,1,0.8 
#########################################################################################

######################################### AST ###########################################
trap ASTdummy height=853 upperWidth=38 lowerWidth=800 Xul=-19 Xur=19 Xll=-400 Xlr=400 length=200 color=0,0,1,0.8 kill=1
fieldmap ASTfieldmap file="$G4BLPIONEER/pie5/feldast.dat"
tubs ASTapertIn innerRadius=225 outerRadius=350 length=150 material=Fe kill=1 color=0,0,1,0.8 
#########################################################################################

####################################### FSH43 ###########################################
box FSH43L height=600 length=50 width=400 kill=1 material=Cu color=0.8,0.8,0.8
box FSH43R height=600 length=50 width=400 kill=1 material=Cu color=0.8,0.8,0.8
group FSH43 length=50
place FSH43L x=200+$FSH43Lset y=0 z=0
place FSH43R x=-200-$FSH43Rset y=0 z=0
endgroup
#########################################################################################

######################################## ColPiE5 ########################################
tubs ColPiE5 innerRadius=160 outerRadius=400 length=200 material=Fe kill=1 color=1,1,1,0.5
# this could be also 250 mm diameter (not clear from the drawing) - could be also shifted (according to drawing)
#########################################################################################

######################################### FS42V #########################################
box FS42Vsing height=400 length=50 width=600 kill=1 material=Cu color=0.8,0.8,0.8
group FS42V length=50
place FS42Vsing x=0 y=200+$FS42Vset z=0
place FS42Vsing x=0 y=-200-$FS42Vset z=0
endgroup
#########################################################################################

######################################### FS42H #########################################
box FS42L height=600 length=50 width=400 kill=1 material=Cu color=0.8,0.8,0.8
box FS42R height=600 length=50 width=400 kill=1 material=Cu color=0.8,0.8,0.8
group FS42H length=50
place FS42L x=200+$FS42Lset y=0 z=0
place FS42R x=-200-$FS42Rset y=0 z=0
endgroup
#########################################################################################

######################################### FSH41 #########################################
box FSH41L height=600 length=50 width=400 kill=1 material=Cu color=0.8,0.8,0.8
box FSH41R height=600 length=50 width=400 kill=1 material=Cu color=0.8,0.8,0.8
group FSH41 length=50
place FSH41L x=200+$FSH41Lset y=0 z=0
# is this right? is FSH41L the slit on the left when looking into beam direction?
# - this questions accounts for the other slits as well
place FSH41R x=-200-$FSH41Rset y=0 z=0
endgroup
#########################################################################################

######################################### QSF ###########################################
# No fieldmap available - Due to similarity the fieldmap of the QSM is reused with a different scaling factor
fieldmap QSF file="$G4BLPIONEER/pie5/feldqsf.dat"
genericquad QSF41DUMMY gradient=0 kill=1 ironRadius=600 ironLength=500 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=200 coilRadius=250 coilHalfwidth=80
genericquad QSF42DUMMY gradient=0 kill=1 ironRadius=600 ironLength=500 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=200 coilRadius=250 coilHalfwidth=80
genericquad QSF43DUMMY gradient=0 kill=1 ironRadius=600 ironLength=500 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=200 coilRadius=250 coilHalfwidth=80
genericquad QSF44DUMMY gradient=0 kill=1 ironRadius=600 ironLength=500 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=200 coilRadius=250 coilHalfwidth=80
genericquad QSF45DUMMY gradient=0 kill=1 ironRadius=600 ironLength=500 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=200 coilRadius=250 coilHalfwidth=80
genericquad QSF46DUMMY gradient=0 kill=1 ironRadius=600 ironLength=500 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=200 coilRadius=250 coilHalfwidth=80
genericquad QSF47DUMMY gradient=0 kill=1 ironRadius=600 ironLength=500 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=200 coilRadius=250 coilHalfwidth=80
genericquad QSF48DUMMY gradient=0 kill=1 ironRadius=600 ironLength=500 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=200 coilRadius=250 coilHalfwidth=80
#########################################################################################

######################################### HSC ###########################################
fieldmap HSC file="$G4BLPIONEER/pie5/feldhsc.dat"
genericquad HSC41DUMMY gradient=0 kill=1 ironRadius=600 ironLength=400 ironColor=0,1,0,0.6 fieldLength=1 poleTipRadius=200 coilRadius=250 coilHalfwidth=80
genericquad HSC42DUMMY gradient=0 kill=1 ironRadius=600 ironLength=400 ironColor=0,1,0,0.6 fieldLength=1 poleTipRadius=200 coilRadius=250 coilHalfwidth=80
genericquad HSC43DUMMY gradient=0 kill=1 ironRadius=600 ironLength=400 ironColor=0,1,0,0.6 fieldLength=1 poleTipRadius=200 coilRadius=250 coilHalfwidth=80
genericquad HSC44DUMMY gradient=0 kill=1 ironRadius=600 ironLength=400 ironColor=0,1,0,0.6 fieldLength=1 poleTipRadius=200 coilRadius=250 coilHalfwidth=80
#########################################################################################
									
######################################## Calo ###########################################
tubs CALOCyl outerRadius=50 length=400 material=Vacuum color=1,1,1 kill=1								
sphere CALOSphere innerRadius=50 outerRadius=800 initialTheta=17 finalTheta=163 material=Vacuum color=1,1,1 kill=1
boolean op=subtraction CALO CALOSphere CALOCyl kill=1
#########################################################################################
									
##################################### CALOCNTR ##########################################
virtualdetector CALOCNTR radius=49.5 length=0.1 color=1,1,0,0.6 material=Vacuum format=asciiExtended file=$G4BLOUTPUTDIR/CALOCNTR.txt
virtualdetector CALOCNTRr radius=49.5 length=0.1 color=1,1,0,0.6 material=Vacuum format=rootExtended file=$G4BLOUTPUTDIR/CALOCNTR.root
#########################################################################################
									
######################################## KILL ###########################################
tubs KILLCYL radius=200 length=1 material=Vacuum color=0,0,0 kill=1								
#########################################################################################
									
####################################### BEAMLINE #########################################
######################## AHSW aperture - dimensions taken from 10002.24.560a & 10002.24.561a ##########################
# front flange
box frontfront height=650 length=1 width=650 kill=1
box frontvcut height=208 length=10 width=400
boolean op=subtraction vslitfront frontfront frontvcut
tubs frontflange innerRadius=200 outerRadius=325 length=20 material=Vacuum kill=1
boolean op=union frontflangeaperture frontflange vslitfront color=0.6,0.6,0.6 z=10.5
place frontflangeaperture z=439.45 x=1.9 rotation=Y-3 kill=1 # origin ~center of the frontflange of the AHSW vacuumchamber

# step in vacuum chamber: height=208 --> height=408
box centerfrontpart height=400 length=1 width=156.54 kill=1
boolean op=union center1staperture centerfrontpart centerfrontpart y=608 color=0.6,0.6,0.6
place center1staperture y=-304 z=654.88 x=249.72 rotation=Y180-142.42 kill=1

box centercenterpart height=400 length=1 width=289.01 kill=1
boolean op=union center2ndaperture centercenterpart centercenterpart y=608 color=0.6,0.6,0.6
place center2ndaperture y=-304 z=842.32 x=150.75 rotation=Y90-14.81 kill=1

box centerrearpart height=400 length=1 width=273.03 kill=1
boolean op=union center3rdaperture centerrearpart centerrearpart y=608 color=0.6,0.6,0.6
place center3rdaperture y=-304 z=1114 x=148.71 rotation=Y90+90-75.18 kill=1

# exit flange
box exitfront height=800 length=1 width=800 kill=1
box exitvcut height=408 length=10 width=400
boolean op=subtraction vslitexit exitfront exitvcut
tubs exitflange innerRadius=200 outerRadius=325 length=20 material=Vacuum kill=1
boolean op=union exitflangeaperture exitflange vslitexit color=0.6,0.6,0.6 z=10.5
place exitflangeaperture z=1069.8 x=364.48 rotation=Y180-132.25+180 kill=1 # origin ~center of the frontflange of the AHSW vacuumchamber

#######################################################################################################################
place AHSW41 z=$posAHSW41 x=0 rotation=Y165 current=$AHSW41curraw*$scaleAHSW
cornerarc CAHSW z=$posfrontarcAHSW41 angle=47.5 centerRadius=676.15 radiusCut=500
#######################################################################################################################
##  BEAM LOCATION 1
beam ascii file="$G4BLPIONEER/project-g4bl/bl2/$G4BLTRACKFILE" # lastEvent=$lastEvent
#######################################################################################################################
place QSF rename=QSF41 z=$posQSF41 current=$QSF41curraw*$scaleQSF
place QSF41DUMMY z=$posQSF41
###############################################
place HSC rename=HSC41 z=$posHSC41 current=$HSC41curraw*$scaleHSC
place HSC41DUMMY z=$posHSC41
#######################################################################################################################
place QSF rename=QSF42 z=$posQSF42 current=$QSF42curraw*$scaleQSF
place QSF42DUMMY z=$posQSF42
#######################################################################################################################
place QSF rename=QSF43 z=$posQSF43 current=$QSF43curraw*$scaleQSF
place QSF43DUMMY z=$posQSF43
#######################################################################################################################
place FSH41 z=$posFSH41
#######################################################################################################################
place HSC rename=HSC42 z=$posHSC42 current=$HSC42curraw*$scaleHSC
place HSC42DUMMY z=$posHSC42
#######################################################################################################################
place QSF rename=QSF44 z=$posQSF44 current=$QSF44curraw*$scaleQSF
place QSF44DUMMY z=$posQSF44
#######################################################################################################################
place FS42H z=$posFS42H
place FS42V z=$posFS42V
#######################################################################################################################
place ColPiE5 z=$posColPiE5
#######################################################################################################################
place QSF rename=QSF45 z=$posQSF45 current=$QSF45curraw*$scaleQSF
place QSF45DUMMY z=$posQSF45
#######################################################################################################################
place HSC rename=HSC43 z=$posHSC43 current=$HSC43curraw*$scaleHSC
place HSC43DUMMY z=$posHSC43
#######################################################################################################################
place FSH43 z=$posFSH43
#######################################################################################################################
place QSF rename=QSF46 z=$posQSF46 current=$QSF46curraw*$scaleQSF
place QSF46DUMMY z=$posQSF46
#######################################################################################################################
place QSF rename=QSF47 z=$posQSF47 current=$QSF47curraw*$scaleQSF
place QSF47DUMMY z=$posQSF47
#######################################################################################################################
place HSC rename=HSC44 z=$posHSC44 current=$HSC44curraw*$scaleHSC
place HSC44DUMMY z=$posHSC44
#######################################################################################################################
place QSF rename=QSF48 z=$posQSF48 current=$QSF48curraw*$scaleQSF
place QSF48DUMMY z=$posQSF48
#######################################################################################################################
place ASTapertIn z=$posASTapertIn
place ASTdummy x=$posxAST41 z=$poszAST41 y=-210 rotation=X90
place ASTdummy x=$posxAST41 z=$poszAST41 y=210 rotation=X90
place ASTfieldmap x=$posxAST41 rotation=Y0 z=$poszAST41 current=$AST41curraw*$scaleAST
cornerarc CAST z=$posfrontarcAST41 angle=-47.5 centerRadius=774.4 radiusCut=500
#######################################################################################################################
place flangeASTASC z=$posflangeASTASC
place ASCfieldmap x=$posxASC41 rotation=Y82.5 z=$poszASC41 current=$ASC41curraw*$scaleASC
place ASCdummy z=$poszASC41+223.5 y=-210 rotation=Y82.5 x=-289.3
place ASCdummy z=$poszASC41+223.5 y=210 rotation=Y82.5 x=-289.3
cornerarc CASC z=$posfrontarcASC41 angle=75 centerRadius=580.6 radiusCut=500
place ASCapertOut z=$posASCapertOut
#######################################################################################################################
place QSB rename=QSB41 current=$QSB41curraw*$scaleQSB z=$posQSB41
place QSB41DUMMY z=$posQSB41
#######################################################################################################################
place QSB rename=QSB42 current=$QSB42curraw*$scaleQSB z=$posQSB42
place QSB42DUMMY z=$posQSB42
#######################################################################################################################
place QSB rename=QSB43 current=$QSB43curraw*$scaleQSB z=$posQSB43
place QSB43DUMMY z=$posQSB43
#######################################################################################################################
place SEP41E195 z=$posSEP41 gradient=$SEP41vol/195
#ul place SEP41B45 z=$posSEP41 current=$SEP41cur/45
place SEP41_plates z=$posSEP41
#######################################################################################################################
place QSK rename=QSK41 current=$QSK41curraw*$scaleQSK z=$posQSK41
place QSK41DUMMY z=$posQSK41
#######################################################################################################################
place QSK rename=QSK42 current=$QSK42curraw*$scaleQSK z=$posQSK42
place QSK42DUMMY z=$posQSK42
#######################################################################################################################
place QSK rename=QSK43 current=$QSK43curraw*$scaleQSK z=$posQSK43
place QSK43DUMMY z=$posQSK43
#######################################################################################################################
#place SML41 z=$posSML41
#place SML41DUMMY z=$posSML41
#######################################################################################################################
#place CALO z=$posCALO
#######################################################################################################################
#profile zloop=1300,$posATAR,10 filename=$G4BLOUTPUTDIR/$G4BLOUTPUTDIR-211.prof particle=pi+
#profile zloop=1300,$posATAR,10 filename=$G4BLOUTPUTDIR/$G4BLOUTPUTDIR-11.prof particle=e+
#profile zloop=1300,$posATAR,10 filename=$G4BLOUTPUTDIR/$G4BLOUTPUTDIR-13.prof particle=mu+

#######################################################################################################################
include "$G4BLPIONEER/pioneer/beamline/zntuple.txt"
