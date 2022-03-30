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
// $Id: G4HIMBProtonInelasticXS.cc 76889 2013-11-18 13:01:55Z gcosmo $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:     G4HIMBProtonInelasticXS
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 13.03.2007
// Modifications:
//
//
// -------------------------------------------------------------------
//

#include "G4HIMBProtonInelasticXS.hh"
#include "G4SystemOfUnits.hh"
//#include "G4GlauberGribovCrossSection.hh"
#include "G4ComponentGGHadronNucleusXsc.hh"
// EV 19-Jun-2020: The G4GlauberGribovCrossSection class, no more present in Geant4, has been replaced by the available G4ComponentGGHadronNucleusXsc.
#include "G4NucleonNuclearCrossSection.hh"
#include "G4HadronNucleonXsc.hh"
#include "G4ComponentSAIDTotalXS.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4Isotope.hh"


#include "G4CrossSectionDataSetRegistry.hh"
#include "G4CrossSectionDataSetRegistry.hh"
#include "G4ProtonInelasticCrossSection.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const G4double llog10 = G4Log(10.);

G4HIMBProtonInelasticXS::G4HIMBProtonInelasticXS(const G4ParticleDefinition* p,G4bool addG4PInelastic,
		G4VCrossSectionDataSet* DataSetToAdd)
 : G4VCrossSectionDataSet("HIMBProtonPionProduction")
{
  verboseLevel = 0;
  fLowEnergy = 325.*MeV;
  fHighEnergy = 1.*GeV;
  particle = p;
  theProton= G4Proton::Proton();
  isProton = false;
  isInitialized = false;

  if(DataSetToAdd) g4_pInelastic_XS =  DataSetToAdd;
  else g4_pInelastic_XS=new G4ProtonInelasticCrossSection();

  add_g4_pInelastic_XS=addG4PInelastic;

  lastHIMB_XS=0.;
  lastG4_XS=0.;


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4HIMBProtonInelasticXS::~G4HIMBProtonInelasticXS()
{;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool G4HIMBProtonInelasticXS::IsElementApplicable(const G4DynamicParticle* dp,
						    G4int, const G4Material*)
{ G4double ekin = dp->GetKineticEnergy();

  return ( ekin<fHighEnergy);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double
G4HIMBProtonInelasticXS::GetElementCrossSection(const G4DynamicParticle* dp,
						G4int ZZ, const G4Material* aMat)
{
 lastHIMB_XS= GetHIMBElementCrossSection(dp,ZZ, aMat);
 lastG4_XS=0.;
 if (add_g4_pInelastic_XS) lastG4_XS=g4_pInelastic_XS->GetElementCrossSection(dp, ZZ, aMat);

 return lastHIMB_XS+lastG4_XS;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double
G4HIMBProtonInelasticXS::GetHIMBElementCrossSection(const G4DynamicParticle* dp,
						G4int ZZ, const G4Material*)
{
  // this method should be called only for Z > 1

  G4double tot_cross = 0.0;
  G4double ekin = dp->GetKineticEnergy();
  G4double ekin_MeV =ekin/MeV;
  G4int Z = ZZ;

  //The cross section formula from Andreas Knecht
  G4double sig585 = 9.70*millibarn;
  G4double sig730 = 13.50*millibarn;

  if (Z>1 && Z<12){
    sig585 = 28.5*millibarn*std::pow(Z/6.,1./3.)*(0.77 + 0.039*Z);
    sig730 = 35.0*millibarn*std::pow(Z/6.,1./3.)*(0.77 + 0.039*Z);
  }
  else if (Z>=12) {
    sig585 = 19.65*millibarn*std::pow(Z,1./3.);
    sig730 = 24.50*millibarn*std::pow(Z,1./3.);
  }

  tot_cross = 0;

  if (ekin_MeV>325. && ekin_MeV<=585.) {
     tot_cross = sig585*(ekin_MeV - 325.)/(585. - 325.);
  }
  else if (ekin_MeV>585. && ekin_MeV<1000.) {
    tot_cross= sig585 + (sig730 - sig585)*(ekin - 585.)/(730- 585.);
  }
  return tot_cross;
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool G4HIMBProtonInelasticXS::IsHIMBPiPlusProductionActive()
{ G4double  rand= G4UniformRand();
  return  (rand<= lastHIMB_XS/(lastHIMB_XS+lastG4_XS));
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4HIMBProtonInelasticXS::CrossSectionDescription(std::ostream& outFile) const
{
  outFile << "The HIMB cross section is a parameterized total cross section for the proton induced pi production\n"
          << "on different targets.\n";

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
