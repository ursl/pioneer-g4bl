//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: HIMBPionProduction.cc 66892 2013-01-17 10:57:59Z gunter $
//
// Geant4 Header : HIMBPionProduction
//
// Author : V.Ivanchenko 29 June 2009 (redesign old elastic model)
//  

#include "HIMBPionProduction.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4IonTable.hh"
#include "Randomize.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"
#include "G4Deuteron.hh"
#include "G4PionPlus.hh"
#include "G4Alpha.hh"
#include "G4Pow.hh"
#include "G4Integrator.hh"
#include "G4HIMBProtonInelasticXS.hh"
#include "G4CascadeInterface.hh"
#include "G4BinaryCascade.hh"


HIMBPionProduction::HIMBPionProduction(const G4String& name,
		G4bool is_hybrid, G4bool is_g4model_binary)
  : G4HadronicInteraction(name),theG4Model(0),splitting_factor(1),theHIMBProtonInelasticXS(0),
    hybrid_mode(is_hybrid)
{
  SetMinEnergy( 0.0*GeV );
  SetMaxEnergy( 100.*TeV );
  lowestEnergyLimit= 1.e-6*eV;  

  theProton   = G4Proton::Proton();
  thePionPlus = G4PionPlus::PionPlus();


  if (hybrid_mode) {
	  if (is_g4model_binary) theG4Model= new G4BinaryCascade();
	  else theG4Model= new G4CascadeInterface();
	  theG4Model->SetMinEnergy(1.*GeV);

  }

  //Description();
}


HIMBPionProduction::~HIMBPionProduction()
{}

void HIMBPionProduction::Description() const
{
  char* dirName = getenv("G4PhysListDocDir");
  if (dirName) {
    std::ofstream outFile;
    G4String outFileName = GetModelName() + ".html";
    G4String pathName = G4String(dirName) + "/" + outFileName;
    outFile.open(pathName);
    outFile << "<html>\n";
    outFile << "<head>\n";

    outFile << "<title>Description of HIMBPionProduction Model</title>\n";
    outFile << "</head>\n";
    outFile << "<body>\n";

    outFile << "HIMBPionProduction is parametrized pion production model\n";

    outFile << "</body>\n";
    outFile << "</html>\n";
    outFile.close();
  }
}

G4HadFinalState* HIMBPionProduction::ApplyYourselfHIMBModel(
		 const G4HadProjectile& aTrack, G4Nucleus& targetNucleus)
{
  //G4cout<<"HIMBPionProduction::ApplyYourselfHIMBModel"<<std::endl;

  theParticleChange.Clear();

  //Get the information on the proton primary
  //--------------------------------------------
  const G4HadProjectile* aParticle = &aTrack;
  G4double Tp = aParticle->GetKineticEnergy();
  G4double plab = aParticle->GetTotalMomentum();



  //Info on the target nucleus
  //-------------------------------------------
  G4int A = targetNucleus.GetA_asInt();
  G4int Z = targetNucleus.GetZ_asInt();

  //Kill the proton in non hybrid mode, keep it in hybrid mode
  //---------------------------------------------
  if (!hybrid_mode) theParticleChange.SetStatusChange(stopAndKill);

  G4double edep=Tp+theProton->GetPDGMass();

  G4double parent_weight=theParticleChange.GetWeightChange();


  //Loop for splitting of the secondaries
  for (G4int i=0;i<splitting_factor;i++){
	  //Sample energy and direction of pion
	  //--------------------------------
	  G4double Tpi,theta_deg;
	  SampleSecondaryPionEnergyAndTheta(Z, Tp, Tpi,theta_deg);
	  G4double theta=theta_deg*degree;
	  G4double phi = G4UniformRand()*2.*pi;

	  //Set the secondary
	  //--------------
	  G4ThreeVector dir = G4ThreeVector(0.,0.,1.);
	  dir.setTheta(theta);
	  dir.setPhi(phi);

	  //Rotate the direction of the secondary according to the direction of the projectile
	  dir.rotateUz(aParticle->Get4Momentum().vect()/plab);
	  G4DynamicParticle * aSec = new G4DynamicParticle(thePionPlus, dir,Tpi*MeV);
	  theParticleChange.AddSecondary(aSec);
	  theParticleChange.GetSecondary(i)->SetWeight(parent_weight/splitting_factor);
	  /*No longer neeeded
	  //Need to add the difference of total energy between primary and seconday as energy deposit to avoid
	  //fatal exception of Geant4 relative to non conservation of energy.
	  edep=edep-Tpi-thePionPlus->GetPDGMass();
	  */

  }

  //theParticleChange.SetLocalEnergyDeposit(edep);

  /*Non longer needed
  //Set the target nucleus as secondary to avoid non conservation of energy bug
  //------------------------------------
  G4ParticleDefinition* theNucDef = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(Z,A,0.0);
  G4DynamicParticle * aSec1 = new G4DynamicParticle(theNucDef, G4ThreeVector(0.,0.,1.),0.000001);
  theParticleChange.AddSecondary(aSec1);
  */


  return &theParticleChange;
}

G4HadFinalState* HIMBPionProduction::ApplyYourself(
		 const G4HadProjectile& aTrack, G4Nucleus& targetNucleus)
{
  if (hybrid_mode && theHIMBProtonInelasticXS){
	  //If kinetic energy <350 MeV, the G4model should is used
	  const G4HadProjectile* aParticle = &aTrack;
	  G4double Tp = aParticle->GetKineticEnergy();
	  if (Tp<350*MeV) return theG4Model->ApplyYourself(aTrack,targetNucleus);

	  //Recompute the XS for Z element that has been selected
	  G4int ZZ=  targetNucleus.GetZ_asInt();

	  //Need to create a dynamic particle to call theHIMBProtonInelasticXS->GetElementCrossSection
	  G4DynamicParticle* aDynParticle = new G4DynamicParticle (aTrack.GetDefinition(), G4ThreeVector(0.,0.,1.),Tp);
	  G4double XS =theHIMBProtonInelasticXS->GetElementCrossSection(aDynParticle,
			  	  	  	  	  	  	  	  ZZ, aTrack.GetMaterial());
	  delete aDynParticle;
	  //Select randomly if HIMB or G4 models should be used and call them
	  if (theHIMBProtonInelasticXS->IsHIMBPiPlusProductionActive()) return ApplyYourselfHIMBModel(aTrack, targetNucleus);
	  else return ApplyYourselfG4BuiltInModelKillingPiPlusProducts(aTrack, targetNucleus);
   }
  else return ApplyYourselfHIMBModel(aTrack, targetNucleus);
}

//Call G4BuiltIn model but kill the pi+ that will be produced by the HIMB model

G4HadFinalState* HIMBPionProduction::ApplyYourselfG4BuiltInModelKillingPiPlusProducts(
		 const G4HadProjectile& aTrack, G4Nucleus& targetNucleus)
{
	if (!theG4Model){
		G4ExceptionDescription ed;
		ed << "Object theG4Model not initialized.";
		G4Exception("HIMBPionProduction::ApplyYourselfG4BuiltInModelKillingPiPlusProducts()","himb001",
			FatalException, ed);
	}
	 G4HadFinalState* theG4ModelParticleChange=theG4Model->ApplyYourself(aTrack,targetNucleus);
	//now remove all pi+ secondary that may occur
	G4int n = theG4ModelParticleChange->GetNumberOfSecondaries();
	std::vector<G4HadSecondary*> addSecs;
	G4HadFinalState* aHadFinalState  = new G4HadFinalState();
	for (G4int i=0;i<n;i++) {
		G4HadSecondary* aSecondary= theG4ModelParticleChange->GetSecondary(i);
		if (aSecondary->GetParticle()->GetParticleDefinition() != G4PionPlus::PionPlus()) addSecs.push_back(aSecondary);
	}
	theG4ModelParticleChange->ClearSecondaries();
	for (size_t it=0;it<addSecs.size();it++) {
		theG4ModelParticleChange->AddSecondary(*addSecs[it]);
	}
	return theG4ModelParticleChange;
}

//A very high fatal energy check levels are set to avoid fatal exception during energy conservation check
const std::pair<G4double, G4double> HIMBPionProduction::GetFatalEnergyCheckLevels() const
{
	// Set very high level checks to avoid non e-conservation fatal exception
	return std::pair<G4double, G4double>(1.e10*perCent, 1.e10 * GeV);
}


/*
// sample momentum transfer in the CMS system 
G4double 
HIMBPionProduction::SampleInvariantT(const G4ParticleDefinition* p,
				  G4double plab,
				  G4int Z, G4int A)
{
  static const G4double GeV2 = GeV*GeV;
  G4double momentumCMS = ComputeMomentumCMS(p,plab,Z,A);
  G4double tmax = 4.0*momentumCMS*momentumCMS/GeV2;
  G4double aa, bb, cc;
  G4double dd = 10.;
  G4Pow* g4pow = G4Pow::GetInstance();
  if (A <= 62) {
    bb = 14.5*g4pow->Z23(A);
    aa = g4pow->powZ(A, 1.63)/bb;
    cc = 1.4*g4pow->Z13(A)/dd;
  } else {
    bb = 60.*g4pow->Z13(A);
    aa = g4pow->powZ(A, 1.33)/bb;
    cc = 0.4*g4pow->powZ(A, 0.4)/dd;
  }
  G4double q1 = 1.0 - std::exp(-bb*tmax);
  G4double q2 = 1.0 - std::exp(-dd*tmax);
  G4double s1 = q1*aa;
  G4double s2 = q2*cc;
  if((s1 + s2)*G4UniformRand() < s2) {
    q1 = q2;
    bb = dd;
  }
  return -GeV2*std::log(1.0 - G4UniformRand()*q1)/bb;
}
*/

void HIMBPionProduction::SampleSecondaryPionEnergyAndTheta(G4int ZZ, G4double Tp,
													G4double& Tpi,G4double& theta_deg)
{
 //first check if cumulative probability should computed for the given ZZ
 G4int Z_index=Z_indices[ZZ-1];
 if (Z_index<0) {
	 ComputeCumulativeProbabilityVectors(ZZ);
	 Z_index=Z_indices[ZZ-1];
 }

 //Get index of Tp discretization
 //------------------------------
 G4int Tp_index = GetTpIndex(Tp);

 //Sampling of Tpi
 //---------------
 G4double  rand= G4UniformRand();
 //theTpiCumProbabilityVectors[Z_index][Tp_index]->DumpValues();
 Tpi =theTpiCumProbabilityVectors[Z_index][Tp_index]->CopyFindLinearEnergy(rand);
 //G4cout<<rand<<" "<<Tpi<<std::endl;
 //Sampling of Theta_deg
 //---------------
 rand= G4UniformRand();
 G4int Tpi_index= GetTpiIndex(Tpi);
 theta_deg = theThetaCumProbabilityVectors[Z_index][Tp_index][Tpi_index]->CopyFindLinearEnergy(rand);

}

void HIMBPionProduction::Initialize()
{
    G4cout<<"Initializing HIMB Model"<<std::endl;
  theTpiCumProbabilityVectors.clear();
  theThetaCumProbabilityVectors.clear();
  Z_indices.clear();
  Z_last_index=-1;
  for (size_t i=0; i<92;i++){
	  Z_indices.push_back(-1);
  }

  Tpi_min1=0.;
  Tpi_min2=50;
  Tpi_min3=300;
  Tpi_max=800;


  dTpi1=1.;
  dTpi2=5.;
  dTpi3=10.;


  //back to old stepping
  /*dTpi1=10.;
  dTpi2=10.;
  dTpi3=10.;*/

  Tp_min=350.;
  Tp_max=1010.;
  dTp=10.;
    
  dTheta_deg=3.;


  G4int numEle = G4Element::GetNumberOfElements();
  for (G4int i=0; i<numEle; i++){
	  G4int Z= int((*G4Element::GetElementTable())[i]->GetZ());
	  G4int Z_index=Z_indices[Z-1];
	  if (Z_index<0) {
                G4cout<<"HiMB model: Computing cumulative probabilities for element with Z = " << Z << std::endl;
	  	ComputeCumulativeProbabilityVectors(Z);
	  }
  }
}

void HIMBPionProduction::ComputeCumulativeProbabilityVectors(G4int Z)
{ Z_last_index++;
  Z_indices[Z-1]=Z_last_index;
  Tp_last_index=-1;
  theThetaCumProbabilityVectors.push_back(std::vector< std::vector<G4CopyPhysicsOrderedFreeVector*> >());
  theTpiCumProbabilityVectors.push_back(std::vector<G4CopyPhysicsOrderedFreeVector*>());
  theTpiCumProbabilityVectors[Z_last_index].clear();
  for (G4double Tp=Tp_min; Tp<=Tp_max;Tp+=dTp){
	  ComputeCumulativeProbabilityVectors(Z,Tp);
  }
}

void HIMBPionProduction::ComputeCumulativeProbabilityVectors(G4int Z,G4double Tp)
{G4double int_CS=0.;
 G4Integrator<HIMBPionProduction, double(HIMBPionProduction::*)(double)> anIntegrator;
 G4CopyPhysicsOrderedFreeVector* aProbabilityVector = new G4CopyPhysicsOrderedFreeVector();
 aProbabilityVector->InsertValues(Tpi_min1,int_CS);
 ZZ1=Z;
 Tp1=Tp;
 Tp_last_index++;
 theThetaCumProbabilityVectors[Z_last_index].push_back(std::vector<G4CopyPhysicsOrderedFreeVector*>());
 theThetaCumProbabilityVectors[Z_last_index][Tp_last_index].clear();
 G4double last_int_CS_theta=ComputeCumulativeProbabilityVectors( Z, Tp, Tpi_min1);
 G4double dTpi=dTpi1;
 for (G4double Tpi=Tpi_min1+dTpi; Tpi<=Tpi_max;Tpi+=dTpi){
	 G4double int_CS_theta=ComputeCumulativeProbabilityVectors( Z, Tp, Tpi);
	 int_CS+=0.5*(last_int_CS_theta+int_CS_theta)*dTpi;
	 last_int_CS_theta=int_CS_theta;
	 aProbabilityVector->InsertValues(Tpi,int_CS);
	 if(Tpi>=Tpi_min3){
		 dTpi=dTpi3;
	 }
	 else if (Tpi>=Tpi_min2){
		 dTpi=dTpi2;
	 }
 }
 if (int_CS>0.) aProbabilityVector->ScaleVector(1., 1./int_CS);
 theTpiCumProbabilityVectors[Z_last_index].push_back(aProbabilityVector);
}

G4double HIMBPionProduction::ComputeCumulativeProbabilityVectorsWithZandTpFixed(G4double Tpi)
{return ComputeCumulativeProbabilityVectors(ZZ1,Tp1,Tpi);
}

G4double HIMBPionProduction::ComputeCumulativeProbabilityVectors(G4int Z,G4double Tp,G4double Tpi)
{G4double theta_min=0;
 G4double int_CS=0.;
    //G4int nint=10;
 G4Integrator<HIMBPionProduction, double(HIMBPionProduction::*)(double)> anIntegrator;
 G4CopyPhysicsOrderedFreeVector* aProbabilityVector = new G4CopyPhysicsOrderedFreeVector();
 aProbabilityVector->InsertValues(theta_min,int_CS);
 ZZ1=Z;
 Tp1=Tp;
 Tpi1=Tpi;
 for (G4double theta=dTheta_deg; theta<=180.;theta+=dTheta_deg){
	 int_CS+=anIntegrator.Simpson(this,&HIMBPionProduction::DoubleDiffCrossSectionSinTh,theta_min,theta,5);
	 //G4cout<<Tp<<" "<<Tpi<<" "<<theta<<" "<<int_CS<<std::endl;
	 aProbabilityVector->InsertValues(theta,int_CS);
	 theta_min=theta;
 }
 if (int_CS>0.) aProbabilityVector->ScaleVector(1., 1./int_CS);
 theThetaCumProbabilityVectors[Z_last_index][Tp_last_index].push_back(aProbabilityVector);
 return int_CS;
}

G4double HIMBPionProduction::DoubleDiffCrossSection(G4double Tp, G4double Tpi,G4double theta_deg, G4int ZZ)
{  G4double theta = theta_deg;
   G4double Z = 1.*ZZ;

   G4double SigSlope = 10.;
   G4double SigCenter = 40.;
   G4double frac = 1/(1+std::exp(-(Tpi-SigCenter)/SigSlope));

   G4double TpiRef = 40.;
   G4double TpRef=580.;
   G4double ScalingFactor = DoubleDiffCrossSectionHE(Tp,TpiRef,90.,ZZ)/DoubleDiffCrossSectionHE(TpRef,TpiRef,90.,6);


   // G4double TpiShift = 1+4./28.*Z;
   G4double TpiShift = 0.696*Z/std::pow(2*Z,1./3.); // Coulomb repulsion scaled to 5 MeV for Ni; use 2*Z as a proxy for A
   G4double TpiLE = Tpi-TpiShift;

   G4double DiffCrossSec = (1-frac)*ScalingFactor*DoubleDiffCrossSectionLE(TpiLE,theta) + frac*DoubleDiffCrossSectionHE(Tp,Tpi,theta,ZZ);

   if (DiffCrossSec<0) {
	   DiffCrossSec=0;
   }
   return DiffCrossSec;;
}

G4double HIMBPionProduction::DoubleDiffCrossSectionSinTh(G4double theta_deg)
{return DoubleDiffCrossSection(Tp1,Tpi1,theta_deg, ZZ1)*std::sin(theta_deg*degree);
}

G4double HIMBPionProduction::DoubleDiffCrossSectionHE(G4double Tp, G4double Tpi,G4double theta_deg, G4int ZZ)
{ G4double theta=theta_deg;
 G4double Z=ZZ*1.;
 G4double knots[] = {0,0,0,30,70,180,180,180};
 G4int ideg = 3;
 G4int length = 8;
 G4double nums[] = {0,0,0,0,0,0,0,0};

 for (G4int i=0; i<(length-1); i++) {
   if (theta>=knots[i] && theta<knots[i+1]) {
       nums[i] = 1.;
   }
   else {
       nums[i] = 0.;
   }
 }

for (G4int k = 2; k<ideg+1; ++k) {
   for (G4int i=0; i<(length-k); ++i) {
       G4double a1 = theta-knots[i];
       G4double a2 = knots[i+k-1]-knots[i];
       G4double b1 = knots[i+k]-theta;
       G4double b2 = knots[i+k]-knots[i+1];
       G4double a = 0;
       G4double b = 0;
       if (a2!=0) {
           a = nums[i]*a1/a2;
       }
       if (b2!=0) {
           b = nums[i+1]*b1/b2;
       }
       nums[i] = a+b;
   }
}

G4double A1 = std::min(27.-4*((730-Tp)/(730.-585.))*((730-Tp)/(730.-585.)),27.0);
G4double A2 = 18.2;
G4double A3 = 8.0;
G4double A4 = 13+(Z-12)/10.0;
G4double A5 = 9+(Z-12)/10.0-(Tp-685)/20.0;
G4double logZ=std::log(Z);

G4double Normalization = std::pow(Z,1./3.)*(0.8851+logZ*(-0.1015+logZ*(0.1459-logZ*0.0265)));
G4double Amplitude = Normalization*(A1*nums[0]+A2*nums[1]+A3*nums[2]+A4*nums[3]+A5*nums[4]);

G4double numTA585 = 0;
G4double numTA730 = 0;
G4double numsigmaA585 = 0;
G4double numsigmaA730 = 0;
    // removed special case of Z=1 to better match hydrogen
    /*
if (ZZ==1) {
   numTA585 = 64.5;
   numTA730 = 53.0;
   numsigmaA585 = 155.0;
   numsigmaA730 = 127.0;
}
     */
    if (ZZ>=1 && ZZ < 9){
   numTA585 = 28.9;
   numTA730 = 34.2;
   numsigmaA585 = 130.0;
   numsigmaA730 = 150.0;
}
else if (ZZ>=9 && ZZ < 92){
   numTA585 = 26.0;
   numTA730 = 29.9;
   numsigmaA585 = 135.0;
   numsigmaA730 = 160.0;
}
G4double TA = (numTA730*(Tp-585)-numTA585*(Tp-730))/(730.-585.);
G4double sigmaA = (numsigmaA730*(Tp-585)-numsigmaA585*(Tp-730))/(730.-585.);

G4double Tbar = 48.+330.*std::exp(-theta/TA);
G4double sigma = sigmaA*std::exp(-theta/85.);
G4double B = 50.; // changed from 25, somewhat better behavior
G4double TF = Tp-140.-2*B;

//G4double DiffCrossSecHE = Amplitude*std::exp(-std::pow((Tbar-Tpi)/(std::sqrt(2)*sigma),2))*1/(1+std::exp((Tpi-TF)/B));

//new shape of XS at high pion energies: exponential instead of gaussian
G4double DiffCrossSecHE = 0.;

if (Tpi < (Tbar+sigma)) {
    DiffCrossSecHE = Amplitude*std::exp(-std::pow((Tbar-Tpi)/(std::sqrt(2)*sigma),2))*1/(1+std::exp((Tpi-TF)/B));
}
else {
    DiffCrossSecHE = Amplitude*std::exp(-1./2.+1/(0.4+0.7/140.*theta))*std::exp(-(Tpi-Tbar)/((0.4+0.7/140.*theta)*sigma))*1/(1+std::exp((Tpi-TF)/B));
}

return DiffCrossSecHE;
}

G4double HIMBPionProduction::DoubleDiffCrossSectionLE(G4double Tpi,G4double theta_deg)
{  G4double S01 = 15.3;
   G4double T01 = 49.4;
   G4double S02 = 5.6;
   G4double T02 = 32.4;
   G4double DiffCrossSecLE = S01*std::sin(pi*Tpi/2/T01) - S02*std::sin(pi*Tpi/2/T02)*std::cos(theta_deg*pi/180.);
   return DiffCrossSecLE;
}

G4int HIMBPionProduction::GetTpIndex(double Tp)
{ double val =std::max((Tp-Tp_min)/dTp,0.);
  return int(val);
}

G4int HIMBPionProduction::GetTpiIndex(double Tpi)
{G4double val=0.;
 if(Tpi>=Tpi_min3){
	 val =(Tpi-Tpi_min3)/dTpi3+(Tpi_min3-Tpi_min2)/dTpi2+(Tpi_min2-Tpi_min1)/dTpi1;
 }
 else if (Tpi>=Tpi_min2){
	 val =(Tpi-Tpi_min2)/dTpi2+(Tpi_min2-Tpi_min1)/dTpi1;
 }
 else val =std::max((Tpi-Tpi_min1)/dTpi1,0.);
 return int(val);
}

void HIMBPionProduction::BuildPhysicsTable(const G4ParticleDefinition& aParticle)
{ Initialize();
  if (hybrid_mode) {
	  theG4Model->BuildPhysicsTable(aParticle);
  }
}

