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
// $Id: G4HIMBProtonBuilder.cc 66892 2013-01-17 10:57:59Z gunter $
//
//---------------------------------------------------------------------------
//
// ClassName:   G4HIMBProtonBuilder
//
// Author: L.Desorgher
//
//
//----------------------------------------------------------------------------
//
#include "G4HIMBProtonBuilder.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"
#include "G4HIMBProtonInelasticXS.hh"
#include "HIMBUserInterface.hh"

G4HIMBProtonBuilder::
G4HIMBProtonBuilder(G4bool is_hybrid, G4bool is_g4model_binary, G4int splitting_factor):
hybrid_mode(is_hybrid)
{
  G4String model_name = G4String("ProtonPionProductionHIMB");
  if (hybrid_mode){
	  if (is_g4model_binary) model_name= G4String("ProtonHIMBBinaryCascadeHybrid");
	  else model_name= G4String("ProtonHIMBBertiniHybrid");
  }
  theModel = new HIMBPionProduction(model_name,is_hybrid,is_g4model_binary);
  theModel->SetSplittingFactor(splitting_factor);
    HIMBUserInterface::GetPointer()->SetHIMBModel(theModel);
  theMin = 0.*MeV; //set by laurent
   // theMin = 325.*MeV;

  theMax = 1.*GeV;
}

void G4HIMBProtonBuilder::
Build(G4ProtonInelasticProcess * aP)
{
  theModel->SetMinEnergy(theMin);
  theModel->SetMaxEnergy(theMax);
  aP->RegisterMe(theModel);

  //And now add the XS
  //--------------------
  G4HIMBProtonInelasticXS* theXS = new G4HIMBProtonInelasticXS((G4Proton::Proton()),hybrid_mode);
  aP->AddDataSet(theXS);
  if (hybrid_mode) theModel->SetHIMBProtonInelasticXS(theXS);
}

G4HIMBProtonBuilder::
~G4HIMBProtonBuilder()
{
}

void G4HIMBProtonBuilder::
Build(G4HadronElasticProcess * )
{
}

// 2015 by L. Desorgher
