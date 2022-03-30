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
// $Id: G4HIMBProtonInelasticXS.hh 76889 2013-11-18 13:01:55Z gcosmo $
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
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
// Class Description:
//
// Wrapper of proton and neutron inelastic cross-sections using Barashenkov 
// parametersation below 100 GeV and Glauber-Gribov model above
//
// -------------------------------------------------------------------
//

#ifndef G4HIMBProtonInelasticXS_h
#define G4HIMBProtonInelasticXS_h

#include "globals.hh"
#include "G4VCrossSectionDataSet.hh"
#include "G4ParticleDefinition.hh"

//class G4GlauberGribovCrossSection;
class G4ComponentGGHadronNucleusXsc;
// EV 19-Jun-2020: The G4GlauberGribovCrossSection class, no more present in Geant4, has been replaced by the available G4ComponentGGHadronNucleusXsc.
class G4NucleonNuclearCrossSection;
class G4HadronNucleonXsc;
class G4ComponentSAIDTotalXS;
class G4Material;
class G4Element;
class G4Isotope;

class G4HIMBProtonInelasticXS : public G4VCrossSectionDataSet
{
public:

  G4HIMBProtonInelasticXS (const G4ParticleDefinition*,
		  G4bool addG4PInelastic=false,
		  		G4VCrossSectionDataSet* DataSetToAdd=0);

  virtual ~G4HIMBProtonInelasticXS();
   
  virtual
  G4bool IsElementApplicable(const G4DynamicParticle*, G4int Z,
			     const G4Material* mat = 0);



  virtual
  G4double GetElementCrossSection(const G4DynamicParticle*, G4int Z,
				  const G4Material* mat = 0);

  virtual void CrossSectionDescription(std::ostream&) const;

  //This method allow to select wich model should be used from HIMB or G4 built-in models
  //It use the last computed XS.
  G4bool IsHIMBPiPlusProductionActive();



private:



  G4HIMBProtonInelasticXS & operator=(const G4HIMBProtonInelasticXS &right);
  G4HIMBProtonInelasticXS(const G4HIMBProtonInelasticXS&);

  G4double GetHIMBElementCrossSection(const G4DynamicParticle*, G4int Z,
 				  const G4Material* mat = 0);



  G4double fLowEnergy;
  G4double fHighEnergy;
  G4double fMinEnergy;
  G4double fMaxEnergy;


  const G4ParticleDefinition*     particle;
  const G4ParticleDefinition*     theProton;
  G4bool                          isProton;
  G4bool                          isInitialized;

  //G4 proton inelastic cross sectio
  G4VCrossSectionDataSet* g4_pInelastic_XS;
  G4bool add_g4_pInelastic_XS;

  //Last computed Cross SEction
  G4double lastHIMB_XS;
  G4double lastG4_XS;



};



#endif
