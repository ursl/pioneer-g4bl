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
// $Id: G4HadronPhysicsQGSP_BIC_HP_BIAS.cc 73040 2013-08-15 09:36:57Z gcosmo $
//
//---------------------------------------------------------------------------
//
// ClassName:   G4HadronPhysicsQGSP_BIC_HP_BIAS
//
// Author: 2006 G.Folger
//
// Based on G4HadronPhysicsQGSP_BIC
//
// Modified:
// 25.04.2007 G.Folger: Add code for quasielastic
// 31.10.2012 A.Ribon: Use G4MiscBuilder
// 19.03.2013 A.Ribon: Replace LEP with FTFP and BERT
//
//----------------------------------------------------------------------------
//
#include <iomanip>   

#include "G4HadronPhysicsQGSP_BIC_HP_BIAS.hh"

#include "globals.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4IonConstructor.hh"

#include "G4HadronCaptureProcess.hh"
#include "G4NeutronRadCapture.hh"
#include "G4NeutronCaptureXS.hh"
#include "G4NeutronHPCaptureData.hh"
#include "G4LFission.hh"
#include "G4InteractionSplittingWrapperProcess.hh"

#include "G4PhysicsListHelper.hh"

// factory
#include "G4PhysicsConstructorFactory.hh"
//
G4_DECLARE_PHYSCONSTR_FACTORY(G4HadronPhysicsQGSP_BIC_HP_BIAS);

G4ThreadLocal G4HadronPhysicsQGSP_BIC_HP_BIAS::ThreadPrivate*
G4HadronPhysicsQGSP_BIC_HP_BIAS::tpdata = 0;

G4HadronPhysicsQGSP_BIC_HP_BIAS::G4HadronPhysicsQGSP_BIC_HP_BIAS(G4int)
    :  G4VPhysicsConstructor("hInelastic QGSP_BIC_HP")
/*    , theNeutrons(0)
    , theFTFPNeutron(0)
    , theQGSPNeutron(0)
    , theBinaryNeutron(0)
    , theHPNeutron(0)
    , thePiK(0)
    , theFTFPPiK(0)
    , theQGSPPiK(0)
    , theBertiniPiK(0)
    , thePro(0)
    , theFTFPPro(0)
    , theQGSPPro(0)
    , theBinaryPro(0)
    , theHyperon(0)
    , theAntiBaryon(0)
    , theFTFPAntiBaryon(0)
    , xsNeutronCaptureXS(0)*/
//    , QuasiElastic(true)
{}

G4HadronPhysicsQGSP_BIC_HP_BIAS::G4HadronPhysicsQGSP_BIC_HP_BIAS(const G4String& name, G4bool /* quasiElastic */)
    :  G4VPhysicsConstructor(name)  
/*    , theNeutrons(0)
    , theFTFPNeutron(0)
    , theQGSPNeutron(0)
    , theBinaryNeutron(0)
    , theHPNeutron(0)
    , thePiK(0)
    , theFTFPPiK(0)
    , theQGSPPiK(0)
    , theBertiniPiK(0)
    , thePro(0)
    , theFTFPPro(0)
    , theQGSPPro(0)
    , theBinaryPro(0)
    , theHyperon(0)
    , theAntiBaryon(0)
    , theFTFPAntiBaryon(0)
    , xsNeutronCaptureXS(0)*/
//    , QuasiElastic(quasiElastic)
{}

void G4HadronPhysicsQGSP_BIC_HP_BIAS::CreateModels()
{
  G4bool quasiElasticFTF= false;   // Use built-in quasi-elastic (not add-on)
  G4bool quasiElasticQGS= true;    // For QGS, it must use it.

  const G4double maxFTFP = 25.0*GeV;
  const G4double minFTFP =  9.5*GeV;
  const G4double maxBIC  =  9.9*GeV;
  const G4double maxBERT =  5.0*GeV;
  const G4double maxHP   = 19.9*MeV;

  tpdata->theNeutrons=new G4NeutronBuilder( true ); // Fission on
  tpdata->theNeutrons->RegisterMe(tpdata->theQGSPNeutron=new G4QGSPNeutronBuilder(quasiElasticQGS));
  tpdata->theNeutrons->RegisterMe(tpdata->theFTFPNeutron=new G4FTFPNeutronBuilder(quasiElasticFTF));
  tpdata->theFTFPNeutron->SetMinEnergy(minFTFP);
  tpdata->theFTFPNeutron->SetMaxEnergy(maxFTFP);

  tpdata->theNeutrons->RegisterMe(tpdata->theBinaryNeutron=new G4BinaryNeutronBuilder);
  tpdata->theBinaryNeutron->SetMinEnergy(maxHP);
  tpdata->theBinaryNeutron->SetMaxEnergy(maxBIC);

  tpdata->theNeutrons->RegisterMe(tpdata->theHPNeutron=new G4NeutronHPBuilder);

  tpdata->thePro=new G4ProtonBuilder;
  tpdata->thePro->RegisterMe(tpdata->theQGSPPro=new G4QGSPProtonBuilder(quasiElasticQGS));
  tpdata->thePro->RegisterMe(tpdata->theFTFPPro=new G4FTFPProtonBuilder(quasiElasticFTF));
  tpdata->theFTFPPro->SetMinEnergy(minFTFP);
  tpdata->theFTFPPro->SetMaxEnergy(maxFTFP);
  tpdata->thePro->RegisterMe(tpdata->theBinaryPro=new G4BinaryProtonBuilder);
  tpdata->theBinaryPro->SetMaxEnergy(maxBIC);




  tpdata->thePiK=new G4PiKBuilder;
  tpdata->thePiK->RegisterMe(tpdata->theQGSPPiK=new G4QGSPPiKBuilder(quasiElasticQGS));
  tpdata->thePiK->RegisterMe(tpdata->theFTFPPiK=new G4FTFPPiKBuilder(quasiElasticFTF));
  tpdata->theFTFPPiK->SetMaxEnergy(maxFTFP);
  tpdata->thePiK->RegisterMe(tpdata->theBertiniPiK=new G4BertiniPiKBuilder);
  tpdata->theBertiniPiK->SetMaxEnergy(maxBERT);

  tpdata->theHyperon=new G4HyperonFTFPBuilder;

  tpdata->theAntiBaryon=new G4AntiBarionBuilder;
  tpdata->theAntiBaryon->RegisterMe(tpdata->theFTFPAntiBaryon=new G4FTFPAntiBarionBuilder(quasiElasticFTF));
}

G4HadronPhysicsQGSP_BIC_HP_BIAS::~G4HadronPhysicsQGSP_BIC_HP_BIAS()
{
   delete tpdata->theHPNeutron;
   delete tpdata->theBinaryNeutron;
   delete tpdata->theQGSPNeutron;
   delete tpdata->theFTFPNeutron;
   delete tpdata->theBertiniPiK;
   delete tpdata->theQGSPPiK;
   delete tpdata->theFTFPPiK;
   delete tpdata->thePiK;
   delete tpdata->theBinaryPro;
   delete tpdata->theQGSPPro;
   delete tpdata->theFTFPPro;
   delete tpdata->thePro;
   delete tpdata->theFTFPAntiBaryon;
   delete tpdata->theAntiBaryon;
   delete tpdata->theHyperon;
   delete tpdata->xsNeutronCaptureXS;

   delete tpdata; tpdata = 0;
}

void G4HadronPhysicsQGSP_BIC_HP_BIAS::ConstructParticle()
{
  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();

  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();
}

#include "G4ProcessManager.hh"
void G4HadronPhysicsQGSP_BIC_HP_BIAS::ConstructProcess()
{
  if ( tpdata == 0 ) tpdata = new ThreadPrivate;
  CreateModels();
  tpdata->theNeutrons->Build();
  tpdata->thePro->Build();
  tpdata->thePiK->Build();
  tpdata->theHyperon->Build();
  tpdata->theAntiBaryon->Build();

  // --- Neutrons ---
  G4HadronicProcess* capture = 0;
  G4HadronicProcess* fission = 0;
  G4ProcessManager* pmanager = G4Neutron::Neutron()->GetProcessManager();
  G4ProcessVector*  pv = pmanager->GetProcessList();
  for ( size_t i=0; i < static_cast<size_t>(pv->size()); ++i ) {
    if ( fCapture == ((*pv)[i])->GetProcessSubType() ) {
      capture = static_cast<G4HadronicProcess*>((*pv)[i]);
    } else if ( fFission == ((*pv)[i])->GetProcessSubType() ) {
      fission = static_cast<G4HadronicProcess*>((*pv)[i]);
    }
  }
  if ( ! capture ) {
    capture = new G4HadronCaptureProcess("nCapture");
    pmanager->AddDiscreteProcess(capture);
  }
  tpdata->xsNeutronCaptureXS = new G4NeutronCaptureXS();
  capture->AddDataSet(tpdata->xsNeutronCaptureXS);
  capture->AddDataSet( new G4NeutronHPCaptureData );
  G4NeutronRadCapture* theNeutronRadCapture = new G4NeutronRadCapture(); 
  theNeutronRadCapture->SetMinEnergy( 19.9*MeV ); 
  capture->RegisterMe( theNeutronRadCapture );
  if ( ! fission ) {
    fission = new G4HadronFissionProcess("nFission");
    pmanager->AddDiscreteProcess(fission);
  }
  G4LFission* theNeutronLEPFission = new G4LFission();
  theNeutronLEPFission->SetMinEnergy( 19.9*MeV );
  fission->RegisterMe( theNeutronLEPFission );

  //Biasing wrapper process for proton inelastic
  pmanager = G4Proton::Proton()->GetProcessManager();
  pv=G4ProcessTable::GetProcessTable()->FindProcesses("protonInelastic");
  G4InteractionSplittingWrapperProcess* aWrapperProcess = new G4InteractionSplittingWrapperProcess();
  aWrapperProcess->RegisterProcess((*pv)[0]);
  pmanager = G4Proton::Proton()->GetProcessManager();
  pmanager->AddDiscreteProcess(aWrapperProcess);
  pmanager->RemoveProcess((*pv)[0]);


  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

    // All wrapper processes are in the BLCMDphysics.cc file

//  //Biasing wrapper process for pi+ decay
//  G4VProcess* aProcess=G4ProcessTable::GetProcessTable()->FindProcess("Decay","pi+");
//  G4InteractionSplittingWrapperProcess* aWrapperProcess1 = new G4InteractionSplittingWrapperProcess();
//  aWrapperProcess1->RegisterProcess(aProcess);
//  pmanager = G4PionPlus::PionPlus()->GetProcessManager();
//  pmanager->AddProcess(aWrapperProcess1,1,-1,1000);
//  pmanager->RemoveProcess(aProcess);
//
//  //Biasing wrapper process for pi- decay
//  G4VProcess* aProcess1=G4ProcessTable::GetProcessTable()->FindProcess("Decay","pi-");
//  G4InteractionSplittingWrapperProcess* aWrapperProcess2 = new G4InteractionSplittingWrapperProcess();
//  aWrapperProcess2->RegisterProcess(aProcess1);
//  pmanager = G4PionMinus::PionMinus()->GetProcessManager();
//  pmanager->AddProcess(aWrapperProcess2,1,-1,1000);
//  pmanager->RemoveProcess(aProcess1);
//
//

}
