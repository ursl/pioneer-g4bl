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
// Geant4 Header : HIMBUserInterface
//

#include "HIMBUserInterface.hh"

HIMBUserInterface*  HIMBUserInterface::m_user_interface=0;
//////////////////////////////////////////////////////////////////////////////////////
//
HIMBUserInterface* HIMBUserInterface::GetPointer()
{
  if(0 == m_user_interface) {
    static  HIMBUserInterface man;
    m_user_interface = &man;
  }
  return m_user_interface;
}
//////////////////////////////////////////////////////////////////////////////////////
//
HIMBUserInterface::HIMBUserInterface():
 theHIMBModel(0)
{;
}
//////////////////////////////////////////////////////////////////////////////////////
//
HIMBUserInterface::~HIMBUserInterface()
{;
}
//////////////////////////////////////////////////////////////////////////////////////
//
void HIMBUserInterface::SetHIMBModel(HIMBPionProduction* aModel)
{ theHIMBModel=aModel;
}
//////////////////////////////////////////////////////////////////////////////////////
//
void HIMBUserInterface::SetSplittingFactorForHIMBModel(G4int n)
{ if (theHIMBModel)
	theHIMBModel->SetSplittingFactor(n);
}
int HIMBUserInterface::GetSplittingFactorForHIMBModel()
{ if (theHIMBModel)
    return theHIMBModel->GetSplittingFactor();
  else
    return 1;
}
