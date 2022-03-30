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
//
// Geant4 Header : HIMBUserInterface
//
// Author : L. Desorgher (Creation 20-04-2015)
//  
// Modified:
//
// Class Description
// Interface for setting parameters of the HIMB physics mdels
// Class Description - End

#ifndef HIMBUserInterface_h
#define HIMBUserInterface_h 1
 
#include "globals.hh"
#include "HIMBPionProduction.hh"


class HIMBUserInterface
{
public:
  // With description

  static HIMBUserInterface* GetPointer();

  ~HIMBUserInterface();

  private:
   // private constructor to prevent multiple instances with operator "new"
   HIMBUserInterface();

  private:
     // MEMBERS
     static HIMBUserInterface* m_user_interface;
     HIMBPionProduction* theHIMBModel;

  //methods
  public:
   void SetHIMBModel(HIMBPionProduction* aModel);
   void SetSplittingFactorForHIMBModel(G4int n);
    int GetSplittingFactorForHIMBModel();


};
#endif
