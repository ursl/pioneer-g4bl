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
//
// $Id: HIMBPionProduction.hh 66892 2013-01-17 10:57:59Z gunter $
//
// Geant4 Header : HIMBPionProduction
//
// Author : V.Ivanchenko 29 June 2009 (redesign old elastic model)
//  
// Modified:
//
// Class Description
// Default model for elastic scattering; GHEISHA algorithm is used 
// Class Description - End

#ifndef HIMBPionProduction_h
#define HIMBPionProduction_h 1
 
#include "globals.hh"
#include "G4HadronicInteraction.hh"
#include "G4HadProjectile.hh"
#include "G4Nucleus.hh"
#include "G4NucleiProperties.hh"
#include "G4CopyPhysicsOrderedFreeVector.hh"
class G4HIMBProtonInelasticXS;
class G4ParticleDefinition;

class HIMBPionProduction : public G4HadronicInteraction
{
public:

  HIMBPionProduction(const G4String& name = "ProtonPionProductionHIMB",
		  	  	  G4bool is_hybrid=false, G4bool is_g4model_binary=true);

  virtual ~HIMBPionProduction();
 
  // implementation of the G4HadronicInteraction interface
  virtual G4HadFinalState * ApplyYourself(const G4HadProjectile & aTrack, 
					  G4Nucleus & targetNucleus);
  G4HadFinalState * ApplyYourselfHIMBModel(const G4HadProjectile & aTrack,
  					  G4Nucleus & targetNucleus);
  G4HadFinalState * ApplyYourselfG4BuiltInModelKillingPiPlusProducts(const G4HadProjectile & aTrack,
    					  G4Nucleus & targetNucleus);


  //Re-definition of Fatal energy checl to escape energy check fatel exception
  virtual const std::pair<G4double, G4double> GetFatalEnergyCheckLevels() const;



  //Splitting factor
  inline void  SetSplittingFactor(G4int n);
  inline int GetSplittingFactor();

  //Needed for the hybrid model working mode
  inline void SetHIMBProtonInelasticXS(G4HIMBProtonInelasticXS* aXS);
  inline void SetHybridMode(G4bool cond);



  inline void SetLowestEnergyLimit(G4double value);

  inline G4double LowestEnergyLimit() const;

  inline G4double ComputeMomentumCMS(const G4ParticleDefinition* p, 
				     G4double plab, G4int Z, G4int A);
  
  virtual void Description() const;


  // Initialisation before run
  virtual void BuildPhysicsTable(const G4ParticleDefinition&);





private:
  void Initialize();
  void ComputeCumulativeProbabilityVectors(G4int Z);
  void ComputeCumulativeProbabilityVectors(G4int Z,G4double Tp);
  void SampleSecondaryPionEnergyAndTheta(G4int ZZ,G4double Tp,G4double& Tpi,G4double& theta_deg);
  G4double ComputeCumulativeProbabilityVectors(G4int Z,G4double Tp,G4double Tpi);
  G4double ComputeCumulativeProbabilityVectorsWithZandTpFixed(G4double Tpi);

  G4double DoubleDiffCrossSection(G4double Tp, G4double Tpi,G4double theta_deg,G4int ZZ);
  G4double DoubleDiffCrossSectionSinTh(G4double theta_deg); //with Tp1,Tpi1, ZZ1
  G4double DoubleDiffCrossSectionLE(G4double Tpi,G4double theta_deg);
  G4double DoubleDiffCrossSectionHE(G4double Tp, G4double Tpi,G4double thet_deg,G4int ZZ);


  G4int GetTpIndex(double Tp);
  G4int GetTpiIndex(double Tpi);



  G4ParticleDefinition* theProton;
  G4ParticleDefinition* thePionPlus;


  G4double lowestEnergyLimit;  


  //Discretization of integral dfferential cross section;
  G4double Tp_min,Tp_max,dTp; //linear discretization of proton kinetic energy
  G4double Tpi_min1,Tpi_min2,Tpi_min3,Tpi_max,dTpi1,dTpi2,dTpi3;  //linear discretization of pion kinetic energy
  G4double dTheta_deg; //linear discretization of Theta;

  //Vectors of cumulative probability:
  //---------------------------------
  std::vector< std::vector< G4CopyPhysicsOrderedFreeVector*> >  theTpiCumProbabilityVectors;
  std::vector< std::vector< std::vector<  G4CopyPhysicsOrderedFreeVector* > > > theThetaCumProbabilityVectors;
  std::vector<G4int> Z_indices;
  G4int Z_last_index;
  G4int Tp_last_index;
  G4int Tpi_last_index;

  //Variable that stay constant for integration of differential CS :
  //---------------------------------------------------------
  G4double Tp1,Tpi1;
  G4int ZZ1;


  //Splitting factor for HIMB pi+
  G4int splitting_factor;


  //Hybrid mode:  -it calls either Pi+HIMB production model or the G4BuiltIn model according to XS
  //				  -the XS is the sum of HIMBPi+ XS and the G4BuiltIn process XS
  //				  -When a HIMB Pi+ is produced then the primary proton is not cahnged at all

  G4HadronicInteraction* theG4Model;
  G4HIMBProtonInelasticXS* theHIMBProtonInelasticXS;
  G4bool hybrid_mode;


};

inline void HIMBPionProduction::SetLowestEnergyLimit(G4double value)
{
  lowestEnergyLimit = value;
}

inline G4double HIMBPionProduction::LowestEnergyLimit() const
{
  return lowestEnergyLimit;
}

inline G4double HIMBPionProduction::ComputeMomentumCMS(const G4ParticleDefinition* p,
				     G4double plab, G4int Z, G4int A)
{
  G4double m1 = p->GetPDGMass();
  G4double m12= m1*m1;
  G4double mass2 = G4NucleiProperties::GetNuclearMass(A, Z);
  return plab*mass2/std::sqrt(m12 + mass2*mass2 + 2.*mass2*std::sqrt(m12 + plab*plab));
}

inline void HIMBPionProduction::SetHIMBProtonInelasticXS(G4HIMBProtonInelasticXS* aXS)
{theHIMBProtonInelasticXS=aXS;
}

inline void HIMBPionProduction::SetHybridMode(G4bool aBool)
{hybrid_mode=true;
}
inline void HIMBPionProduction::SetSplittingFactor(G4int n)
{ splitting_factor = n;
}
inline int HIMBPionProduction::GetSplittingFactor()
{
  return splitting_factor;
}
#endif
