#########################################################################################
##################################### G4V7M 13072015 ####################################
#########################################################################################

#WEIGHTS are set to zero in muon beam file proton centered!!! 
#################################### Global Settings ####################################
physics QGSP_BERT disable=Decay
reference particle=mu+ referenceMomentum=28 beamX=0 beamY=0 beamZ=0
trace format=AsciiExtended coordinates=c
#profile zloop=200.0:5000.0:10.0 file=emittance.dat particle=mu+ coordinates=c
param maxStep=10
  particlecolor pi+=0,0,1 mu+=1,0.5,0 e+=0,1,0 reference=0,0,0
g4ui when=4 "/vis/scene/add/axes 0 0 0 1000 mm"
g4ui when=4 "/vis/viewer/set/background 1 1 1"
survey file=survey.txt coordinates=global
profile zloop=0:20000:10 file=emittanceqsfqskoldahswnew.dat
#########################################################################################

######################################## Beam ###########################################
beam ascii file=MuonBeamWeight.txt
#beam gaussian particle=mu+ nEvents=1000 beamZ=0 sigmaX=2.6666 sigmaY=1 sigmaXp=0.13333 sigmaYp=0.03 meanMomentum=28 sigmaP=0.84 meanT=0 sigmaT=0
#########################################################################################

################################# Elements Strengths ####################################
param -unset sign=1 		       # sign=-1 for backtracking ::: sign=1 for normal DS tracking
include "CurrentsCMBL2018.txt" 		    # currents according to setpoint
include "DeltaCurrents.txt"  # Deviations between Soll and Ist value that is corrected in the Scalefactors.txt
include "Scalefactors.txt" 	 # scaling factors for fieldmaps/generic elements/fieldexpressions
include "Positions.txt"		    # Positions generated with Mathematica Notebook pathlength from Inventor model
#########################################################################################


#########################################################################################
####################################### Elements ########################################
#########################################################################################

######################################### QSM ###########################################
fieldmap QSM41 file=feldqsm.dat
genericquad QSM41DUMMY gradient=0 kill=1 ironRadius=600 ironLength=500 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=200 coilRadius=250 coilHalfwidth=80
#########################################################################################

######################################### ASK ###########################################
fieldmap ASK41 file=feldask.dat current=$ASK41set #*1.07 # From TOSCA generated end of 2014
genericbend ASK41DUMMY fieldWidth=800 fieldHeight=240 By=0 fieldLength=1 ironWidth=1780 ironHeight=1000 ironLength=500 ironColor=0,0,1,0.6 kill=1
#########################################################################################

######################################### PILL ##########################################
virtualdetector PILL radius=200 length=1 color=1,1,0,0.6 material=Vacuum format=AsciiExtended
#########################################################################################

######################################### QSO ###########################################
fieldmap QSO file=feldqso.dat # measured fieldmap from Vjeran -     x=+-9cm    y=+-10cm
tubs QSODUMMY innerRadius=125 outerRadius=200 length=400 material=Vacuum kill=1 color=1,0,0,0.5
#########################################################################################

######################################### ASL ###########################################
fieldmap ASL41 file=feldasl.dat current=$ASL41set #*1.07 # From TOSCA generated end of 2014
genericbend ASL41DUMMY fieldWidth=800 fieldHeight=180 By=0 fieldLength=1 ironWidth=1780 ironHeight=1000 ironLength=1000 ironColor=0,0,1,0.3 kill=1
#########################################################################################

######################################### SML ###########################################
fieldexpr SML41 height=320 width=320 length=398 By=$SML41set
tubs SML41DUMMY innerRadius=160 outerRadius=162 length=398 material=Vacuum color=0,0,1,0.6
tubs MEGCOL innerRadius=60 outerRadius=159 length=50 material=Pb color=1,0,1,0.8
#########################################################################################

######################################### QSK ###########################################
#genericquad QSK kill=1 ironRadius=500 ironLength=400 ironColor=1,0,0,0.6 fieldLength=390 poleTipRadius=125 coilRadius=160 coilHalfwidth=55 fringeFactor=0.68
fieldmap QSK file=feldqsk.dat
genericquad QSK41DUMMY kill=1 ironRadius=500 ironLength=400 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=125 coilRadius=160 coilHalfwidth=55
genericquad QSK42DUMMY kill=1 ironRadius=500 ironLength=400 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=125 coilRadius=160 coilHalfwidth=55
genericquad QSK43DUMMY kill=1 ironRadius=500 ironLength=400 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=125 coilRadius=160 coilHalfwidth=55
#########################################################################################

######################################## SEP41 ##########################################
fieldmap SEP41E195 file=feldsep41E195_fine.dat
fieldmap SEP41B45 file=feldsep41B45_newfine.dat
genericbend SEP41_plates fieldWidth=400 fieldHeight=190 By=0 fieldLength=1 ironWidth=800 ironHeight=600 ironLength=800 ironColor=0,0,1,0.7 kill=1
#########################################################################################

#########################################################################################

######################################### QSB ###########################################
fieldmap QSB file=feldqsb.dat
genericquad QSB41DUMMY kill=1 ironRadius=500 ironLength=270 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=125 coilRadius=190 coilHalfwidth=55
genericquad QSB42DUMMY kill=1 ironRadius=500 ironLength=270 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=125 coilRadius=190 coilHalfwidth=55
genericquad QSB43DUMMY kill=1 ironRadius=500 ironLength=270 ironColor=1,0,0,0.6 fieldLength=1 poleTipRadius=125 coilRadius=190 coilHalfwidth=55
#########################################################################################

#################################### ASCapertOut #######################################
tubs ASCapertOut innerRadius=160 outerRadius=350 length=100 material=Fe kill=1 color=0,0,1,0.8 
#########################################################################################

######################################### ASC ###########################################
fieldmap ASCfieldmap file=feldasc.dat current=-1
box ASCdummy height=200 length=1200 width=1200 color=0,0,1,0.6
#########################################################################################

#################################### flangeASTASC #######################################
tubs flangeASTASC innerRadius=160 outerRadius=500 length=60 material=Fe kill=1 color=0,0,1,0.8 
#########################################################################################

######################################### AST ###########################################
trap ASTdummy height=853 upperWidth=38 lowerWidth=800 Xul=-19 Xur=19 Xll=-400 Xlr=400 length=200 color=0,0,1,0.8
fieldmap ASTfieldmap file=feldast.dat 
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
tubs ColPiE5 innerRadius=160 outerRadius=400 length=200 material=Fe kill=1 color=1,1,1,0.5 # this could be also 250 mm diameter (not clear from the drawing) - could be also shifted (according to drawing)
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
place FSH41L x=200+$FSH41Lset y=0 z=0 # is this right? is FSH41L the slit on the left when looking into beam direction? - this questions accounts for the other slits as well
place FSH41R x=-200-$FSH41Rset y=0 z=0
endgroup
#########################################################################################

######################################### QSF ###########################################
# No fieldmap available - Due to similarity the fieldmap of the QSM is reused with a different scaling factor
fieldmap QSF file=feldqsf.dat
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
fieldmap HSC file=feldhsc.dat
genericquad HSC41DUMMY gradient=0 kill=1 ironRadius=600 ironLength=400 ironColor=0,1,0,0.6 fieldLength=1 poleTipRadius=200 coilRadius=250 coilHalfwidth=80
genericquad HSC42DUMMY gradient=0 kill=1 ironRadius=600 ironLength=400 ironColor=0,1,0,0.6 fieldLength=1 poleTipRadius=200 coilRadius=250 coilHalfwidth=80
genericquad HSC43DUMMY gradient=0 kill=1 ironRadius=600 ironLength=400 ironColor=0,1,0,0.6 fieldLength=1 poleTipRadius=200 coilRadius=250 coilHalfwidth=80
genericquad HSC44DUMMY gradient=0 kill=1 ironRadius=600 ironLength=400 ironColor=0,1,0,0.6 fieldLength=1 poleTipRadius=200 coilRadius=250 coilHalfwidth=80
#########################################################################################

######################################### AHSW ##########################################
fieldmap AHSW41 file=feldahsw.dat current=$AHSW41set
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
place AHSW41 z=$posAHSW41 x=0 rotation=Y165
cornerarc CAHSW z=$posfrontarcAHSW41 angle=47.5 centerRadius=676.15 radiusCut=500
place QSF rename=QSF41 z=$posQSF41 current=$QSF41set
place QSF41DUMMY z=$posQSF41
#zntuple z=$posQSF41 format=asciiExtended file=pillqsf41.txt coordinates=c
zntuple z=$posQSF41 format=ascii file=pillqsf41.txt coordinates=c
place HSC rename=HSC41 z=$posHSC41 current=$HSC41set
place HSC41DUMMY z=$posHSC41
place QSF rename=QSF42 z=$posQSF42 current=$QSF42set
place QSF42DUMMY z=$posQSF42
place QSF rename=QSF43 z=$posQSF43 current=$QSF43set
place QSF43DUMMY z=$posQSF43
place FSH41 z=$posFSH41
place HSC rename=HSC42 z=$posHSC42 current=$HSC42set
place HSC42DUMMY z=$posHSC42
place QSF rename=QSF44 z=$posQSF44 current=$QSF44set
place QSF44DUMMY z=$posQSF44
place FS42H z=$posFS42H
place FS42V z=$posFS42V
place ColPiE5 z=$posColPiE5
place QSF rename=QSF45 z=$posQSF45 current=$QSF45set
place QSF45DUMMY z=$posQSF45
place HSC rename=HSC43 z=$posHSC43 current=$HSC43set
place HSC43DUMMY z=$posHSC43
place FSH43 z=$posFSH43
place QSF rename=QSF46 z=$posQSF46 current=$QSF46set
place QSF46DUMMY z=$posQSF46
place QSF rename=QSF47 z=$posQSF47 current=$QSF47set
place QSF47DUMMY z=$posQSF47
place HSC rename=HSC44 z=$posHSC44 current=$HSC44set
place HSC44DUMMY z=$posHSC44
place QSF rename=QSF48 z=$posQSF48 current=$QSF48set
place QSF48DUMMY z=$posQSF48
place ASTapertIn z=$posASTapertIn
place ASTdummy x=$posxAST41 z=$poszAST41 y=-210 rotation=X90
place ASTdummy x=$posxAST41 z=$poszAST41 y=210 rotation=X90
place ASTfieldmap x=$posxAST41 rotation=Y0 z=$poszAST41 current=$AST41set
cornerarc CAST z=$posfrontarcAST41 angle=-47.5 centerRadius=774.4 radiusCut=500
place flangeASTASC z=$posflangeASTASC
place ASCfieldmap x=$posxASC41 rotation=Y82.5 z=$poszASC41 current=$ASC41set
place ASCdummy z=$poszASC41+223.5 y=-210 rotation=Y82.5 x=-289.3
place ASCdummy z=$poszASC41+223.5 y=210 rotation=Y82.5 x=-289.3
cornerarc CASC z=$posfrontarcASC41 angle=75 centerRadius=580.6 radiusCut=500
place ASCapertOut z=$posASCapertOut
place QSB rename=QSB41 current=$QSB41set z=$posQSB41
place QSB41DUMMY z=$posQSB41
place QSB rename=QSB42 current=$QSB42set z=$posQSB42
place QSB42DUMMY z=$posQSB42
place QSB rename=QSB43 current=$QSB43set z=$posQSB43
place QSB43DUMMY z=$posQSB43
place SEP41E195 z=$posSEP41 gradient=$SEP41vol/195
place SEP41B45 z=$posSEP41 current=$SEP41cur/45
place SEP41_plates z=$posSEP41
place QSK rename=QSK41 current=$QSK41set z=$posQSK41
place QSK41DUMMY z=$posQSK41
place QSK rename=QSK42 current=$QSK42set z=$posQSK42
place QSK42DUMMY z=$posQSK42
place QSK rename=QSK43 current=$QSK43set z=$posQSK43
place QSK43DUMMY z=$posQSK43
#place QSK rename=QSK41 gradient=$QSK41set z=$posQSK41
#place QSK rename=QSK42 gradient=$QSK42set z=$posQSK42
#place QSK rename=QSK43 gradient=$QSK43set z=$posQSK43
place SML41 z=$posSML41
place SML41DUMMY z=$posSML41
place MEGCOL z=$posMEGCOL
place PILL rename=PILL1 z=$posPILL1
##########################################################################################
#printfield type=grid file=test.dat comment=ascfield1 X0=5000 Y0=0 Z0=5000 nX=201 nY=1 nZ=201 dX=20 dY=20 dZ=20

#probefield 100,80,800,0 100,90,800,0 100,95,800,0 100,100,800,0 100,105,800,0 100,109,800,0
