physics QGSP_BERT_EMY

trackcuts kineticEnergyCut=0.0 kill=gamma,nu_mu,anti_nu_mu,nu_e,anti_nu_e
randomseed Set     5010243
param -unset first=1
param -unset numEvents=100000000
param -unset last=$first+$numEvents-1

######################## Primary Proton Beam ##########################
param eventTimeLimit=1000
param M=938.272 KE=585.00
param P=sqrt(($M+$KE)*($M+$KE)-$M*$M)
param beamZpos=100
beam gaussian particle=proton firstEvent=$first nEvents=$numEvents beamX=-1*$beamZpos*sin(15*deg) beamY=0 beamZ=$beamZpos*cos(15*deg) sigmaX=0.75 sigmaY=1.25 meanMomentum=$P rotation=Y165
  
######################## Reference Beam ##########################
reference particle=pi+ referenceMomentum=75 beamX=0 beamY=0 beamZ=150 meanXp=-0.0167 meanYp=0

particlecolor proton=1,0,0 pi+=0,0,1 mu+=0,1,0 reference=1,0,1
g4ui when=4 "/vis/scene/add/axes 0 0 0 100 mm"
g4ui when=4 "/vis/viewer/set/viewpointThetaPhi 45 45."
#g4ui when=4 "/vis/viewer/panTo 300 0 mm"
#g4ui when=4 "/vis/viewer/zoom 1"
g4ui when=4 "/vis/viewer/set/background 1 1 1"
virtualdetector DetPiE5 radius=250 length=0.1 require=PDGid==-11||PDGid==11||PDGid==-13||PDGid==13||PDGid==211||PDGid==-211 format=rootExtended color=0.5,0.5,0,0.5
virtualdetector DetEMuPiFromTarget radius=300 length=0.1 require=PDGid==-11||PDGid==11||PDGid==-13||PDGid==13||PDGid==211||PDGid==-211 format=rootExtended color=0.5,0.5,0,0.5

####################################### Target E ########################################
#place TargetBlock x=0. y=0. z=0 rotation=Y165 coordinates=global
material TargetEGraphite a=12 z=6 density=1.8
param targetMaterial=TargetEGraphite
param targetYRotation=45.-15.
param targetZ=0.
param targetX=0.
param targetY=0.
param slabLength=83.5
param AngP=8.0
include "../TargetE/polyconeTargetRotated.txt"
#include "../TargetE/polyconeTarget.txt"
place TargetCone z=($targetZ+$Ravg*sin($targetYRotation*deg)) x=-1*($targetX+$Ravg*cos($targetYRotation*deg)) y=$targetY rotation=Y(180+$targetYRotation) coordinates=global color=0,0,0
