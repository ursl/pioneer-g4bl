physics QGSP_BERT_EMY

trackcuts kineticEnergyCut=0.0 kill=gamma,nu_mu,anti_nu_mu,nu_e,anti_nu_e

randomseed set 40000 
param -unset first=1
param -unset numEvents=200000000

param momentum=28.
  
######################## Primary Proton Beam ##########################
param eventTimeLimit=1000
param M=938.272 KE=585.00
param P=sqrt(($M+$KE)*($M+$KE)-$M*$M)
param beamZpos=100
beam gaussian particle=proton firstEvent=$first nEvents=$numEvents beamX=-1*$beamZpos*sin(15*deg) beamY=0 beamZ=$beamZpos*cos(15*deg) sigmaX=0.75 sigmaY=1.25 meanMomentum=$P rotation=Y165
  
######################## Reference Beam ##########################
reference particle=mu+ referenceMomentum=$momentum beamX=0 beamY=0 beamZ=150 meanXp=-0.0167 meanYp=0
  
#trace format=AsciiExtended coordinates=c
particlecolor proton=1,0,0 pi+=0,0,1 mu+=0,1,0 reference=1,0,1
g4ui when=4 "/vis/scene/add/axes 0 0 0 1200 mm"
g4ui when=4 "/vis/viewer/set/viewpointThetaPhi 180 0."
g4ui when=4 "/vis/viewer/set/background 1 1 1"
virtualdetector DetPiE5 radius=250 length=0.1 material=Vacuum require=PDGid==-11||PDGid==11||PDGid==-13||PDGid==13||PDGid==211||PDGid==-211||PDGid==2212 format=rootExtended color=0.5,0.5,0,0.5
virtualdetector DetEMuPiFromTarget radius=300 length=0.1 require=PDGid==-11||PDGid==11||PDGid==-13||PDGid==13||PDGid==211||PDGid==-211||PDGid==2212 format=rootExtended color=0.5,0.5,0,0.5

################################# Elements Strengths ####################################
param scaleMom=$momentum/28.0
param -unset sign=1
# currents according to setpoint
include "$G4BLPIONEER/pie5/CurrentsCMBL2018.txt"
# Deviations between Soll and Ist value that is corrected in the Scalefactors.txt
include "$G4BLPIONEER/pie5/DeltaCurrents.txt" 
# scaling factors for fieldmaps/generic elements/fieldexpressions
include "$G4BLPIONEER/pie5/Scalefactors.txt" 	 
# Positions generated with Mathematica Notebook pathlength from Inventor model
include "$G4BLPIONEER/pie5/Positions.txt"
#########################################################################################

#########################################################################################
####################################### Elements ########################################
#########################################################################################
									
######################################### AHSW ##########################################
fieldmap AHSW41 file="$G4BLPIONEER/pie5/feldahsw.dat"
#########################################################################################
  
######################################### QSF ###########################################
# No fieldmap available - Due to similarity the fieldmap of the QSM is reused with a different scaling factor
fieldmap QSF file="$G4BLPIONEER/pie5/feldqsf.dat"
genericquad QSF41DUMMY gradient=0 kill=1 ironRadius=600 ironLength=500 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=200 coilRadius=250 coilHalfwidth=80
genericquad QSF42DUMMY gradient=0 kill=1 ironRadius=600 ironLength=500 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=200 coilRadius=250 coilHalfwidth=80
genericquad QSF43DUMMY gradient=0 kill=1 ironRadius=600 ironLength=500 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=200 coilRadius=250 coilHalfwidth=80
#########################################################################################

######################################### HSC ###########################################
fieldmap HSC file="$G4BLPIONEER/pie5/feldhsc.dat"
genericquad HSC41DUMMY gradient=0 kill=1 ironRadius=600 ironLength=400 ironColor=0,1,0,0.6 fieldLength=1 poleTipRadius=200 coilRadius=250 coilHalfwidth=80
#########################################################################################

######################################### FSH41 #########################################
box FSH41L height=600 length=50 width=400 kill=1 material=Cu color=0.8,0.8,0.8
box FSH41R height=600 length=50 width=400 kill=1 material=Cu color=0.8,0.8,0.8
group FSH41 length=50
place FSH41L x=200+$FSH41Lset y=0 z=0
place FSH41R x=-200-$FSH41Rset y=0 z=0
endgroup
#########################################################################################

####################################### Target E ########################################
#place TargetBlock x=0. y=0. z=0 rotation=Y165 coordinates=global
material TargetEGraphite a=12 z=6 density=1.8
param targetMaterial=TargetEGraphite
param targetYRotation=45.-15.
param targetZ=0.
param targetX=0.
param targetY=0.
param slabLength=60.0
param AngP=5.0
include "$G4BLPIONEER/pioneer/TargetE/polyconeTargetSlanted.txt"
place TargetCone z=($targetZ+$Ravg*sin($targetYRotation*deg)) x=-1*($targetX+$Ravg*cos($targetYRotation*deg)) y=$targetY rotation=Y(180+$targetYRotation) coordinates=global color=0,0,0

tubs killCyl innerRadius=500 radius=510 length=500 kill=1 color=1,1,1,0.2
tubs killCap radius=500 length=1 kill=1 color=1,1,1,0.2
place killCyl z=0
place killCap z=-250
									
####################################### BEAMLINE #########################################
######################## AHSW aperture - dimensions taken from 10002.24.560a & 10002.24.561a ##########################
# front flange
box frontfront height=650 length=1 width=650 kill=1
box frontvcut height=208 length=10 width=400
boolean op=subtraction vslitfront frontfront frontvcut
tubs frontflange innerRadius=200 outerRadius=325 length=20 material=Vacuum kill=1
boolean op=union frontflangeaperture frontflange vslitfront color=0.6,0.6,0.6 z=10.5
place frontflangeaperture z=439.45 x=1.9 rotation=Y-3 kill=1

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
place exitflangeaperture z=1069.8 x=364.48 rotation=Y180-132.25+180 kill=1 

#######################################################################################################################			  
place DetEMuPiFromTarget x=0 y=0 z=150 coordinates=global
#######################################################################################################################
place AHSW41 z=$posAHSW41 x=0 rotation=Y165 current=$scaleMom*$AHSW41set
cornerarc CAHSW z=$posfrontarcAHSW41 angle=47.5 centerRadius=676.15 radiusCut=500
#######################################################################################################################
place QSF rename=QSF41 z=$posQSF41 current=$scaleMom*$QSF41set									
place QSF41DUMMY z=$posQSF41
#######################################################################################################################
place HSC rename=HSC41 z=$posHSC41 current=$scaleMom*$HSC41set
place HSC41DUMMY z=$posHSC41
#######################################################################################################################
place QSF rename=QSF42 z=$posQSF42 current=$scaleMom*$QSF42set			 
place QSF42DUMMY z=$posQSF42
#######################################################################################################################
place QSF rename=QSF43 z=$posQSF43 current=$scaleMom*$QSF43set								
place QSF43DUMMY z=$posQSF43
#######################################################################################################################
place FSH41 z=$posFSH41
#######################################################################################################################
place DetPiE5 z=4500
#######################################################################################################################

