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
// $Id: G4InteractionSplittingWrapperProcess.cc 1380 2011-12-16 16:42:45Z vnivanch $
// GRAS tag $Name$
//
//---------------------------------------------------------------------------
//
// ClassName:   G4InteractionSplittingWrapperProcess
//
// Author:      L.Desorgher
//
//---------------------------------------------------------------------------
//

#include "G4InteractionSplittingWrapperProcess.hh"
#include "G4VEmProcess.hh"
#include "G4VEnergyLossProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4InteractionSplittingWrapperProcess::G4InteractionSplittingWrapperProcess()
  :G4WrapperProcess("InteractionSplittingWrapper")
{splitting_factor=1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4InteractionSplittingWrapperProcess::~G4InteractionSplittingWrapperProcess()
{;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4VParticleChange* G4InteractionSplittingWrapperProcess::PostStepDoIt(const G4Track& track,
						     const G4Step& step)
{//The primary and deposited energy is kept as from the first interaction while the secondaries are taken from all
 //   simulated interactions with a weight=1/n_split

 //first interaction
	G4VParticleChange* thePartChange=new G4VParticleChange();
	//thePartChange->SetNumberOfSecondaries(10*splitting_factor);
	//thePartChange->ProposeTrackStatus(fStopAndKill);

 //additional interactions
	for (G4int i=0;i<splitting_factor;i++){
		G4VParticleChange* theParticleChange1 = pRegProcess->PostStepDoIt(track,step);
		G4int nsec = theParticleChange1->GetNumberOfSecondaries();
		for (G4int k=0;k<nsec;k++) {
			G4Track* aTrack = theParticleChange1->GetSecondary(k);
			thePartChange->AddSecondary(aTrack);
		}
		theParticleChange1->Clear();
		theParticleChange1->Initialize(track);
	}

 // change the weight of secondaries
	thePartChange->SetSecondaryWeightByProcess(true);
	G4int n = thePartChange->GetNumberOfSecondaries();
	G4double mother_weight = track.GetWeight();
	for (G4int i=0;i<n;i++) {
	  thePartChange->GetSecondary(i)->SetWeight(mother_weight/splitting_factor);
	}
	thePartChange->ProposeTrackStatus(fStopAndKill);

	return thePartChange;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4VParticleChange* G4InteractionSplittingWrapperProcess::AtRestDoIt(const G4Track& track,
  					  const G4Step& step)
{
//The primary and deposited energy is kept as from the first interaction while the secondaries are taken from all
 //   simulated interactions with a weight=1/n_split

 //first interaction
	G4VParticleChange* thePartChange=new G4VParticleChange();
	//thePartChange->ProposeTrackStatus(fStopAndKill);
	//thePartChange->SetNumberOfSecondaries(10*splitting_factor);

 //additional interactions
	for (G4int i=0;i<splitting_factor;i++){
		G4VParticleChange* theParticleChange1 = pRegProcess->AtRestDoIt(track,step);
		G4int nsec = theParticleChange1->GetNumberOfSecondaries();
		for (G4int k=0;k<nsec;k++) {
			G4Track* aTrack = theParticleChange1->GetSecondary(k);
			thePartChange->AddSecondary(aTrack);
		}
		theParticleChange1->Clear();
		theParticleChange1->Initialize(track);
	}

 // change the weight of secondaries
	thePartChange->SetSecondaryWeightByProcess(true);
	G4int n = thePartChange->GetNumberOfSecondaries();
	G4double mother_weight = track.GetWeight();
	for (G4int i=0;i<n;i++) {
	  thePartChange->GetSecondary(i)->SetWeight(mother_weight/splitting_factor);
	}
	thePartChange->ProposeTrackStatus(fStopAndKill);
	return thePartChange;
}




