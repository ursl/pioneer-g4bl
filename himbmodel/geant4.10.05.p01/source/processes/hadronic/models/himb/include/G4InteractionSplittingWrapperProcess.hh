//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software.                                *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This code implementation is the intellectual property of the ESA.*
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
// $Id: G4InteractionSplittingWrapperProcess.hh 621 2007-02-12 19:36:06Z vnivanch $
// GRAS tag $Name$
//
//---------------------------------------------------------------------------
//
// ClassName:   G4InteractionSplittingWrapperProcess
//
// Author:     L. Desorgher

//---------------------------------------------------------------------------
//

#ifndef G4InteractionSplittingWrapperProcess_h
#define G4InteractionSplittingWrapperProcess_h 1

#include "globals.hh"
#include "G4WrapperProcess.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4InteractionSplittingWrapperProcess  :public G4WrapperProcess
{
public:

  G4InteractionSplittingWrapperProcess();
  virtual ~G4InteractionSplittingWrapperProcess();

  virtual G4VParticleChange* PostStepDoIt(const G4Track&, 
					  const G4Step&);
  virtual G4VParticleChange* AtRestDoIt(const G4Track&,
  					  const G4Step&);

  //  Setter for the splitting factor
  inline void SetInteractionSplittingFactor(G4int factor) {splitting_factor=factor;}
  //  Getter for the splitting factor
  inline int GetInteractionSplittingFactor() {return splitting_factor;}


protected:


private:

  G4int splitting_factor;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif













