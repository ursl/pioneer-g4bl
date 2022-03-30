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
// $Id$
//
////////////////////////////////////////////////////////////////////////
// PhysicsOrderedFreeVector Class Definition
////////////////////////////////////////////////////////////////////////
//
// File:	G4CopyPhysicsOrderedFreeVector.hh
// Created:     1996-08-13
// Author:      Juliet Armstrong
// Updated:     1997-03-25 by Peter Gumplinger
//		> cosmetics (only)
//              2000-11-11 by H.Kurashige
//              > use STL vector for dataVector and binVector
// mail:        gum@triumf.ca
//
// Class description:
//
//    A physics ordered free vector inherits from G4PhysicsVector which
//    has values of energy-loss, cross-section, and other physics values
//    of a particle in matter in a given range of the energy, momentum,
//    etc.). In addition, the ordered free vector provides a method for
//    the user to insert energy/value pairs in sequence.  Methods to
//    Retrieve the Max and Min energies and values from the vector are
//    also provided. 

////////////////////////////////////////////////////////////////////////

#ifndef G4CopyPhysicsOrderedFreeVector_h
#define G4CopyPhysicsOrderedFreeVector_h 1

/////////////
// Includes
/////////////

#include "G4PhysicsVector.hh"

/////////////////////
// Class Definition
/////////////////////

class G4CopyPhysicsOrderedFreeVector : public G4PhysicsVector
{

 public: // with description
	
  ////////////////////////////////
  // Constructors and Destructor
  ////////////////////////////////
  
  G4CopyPhysicsOrderedFreeVector();
  G4CopyPhysicsOrderedFreeVector(G4double* Energies,
			     G4double* Values,
			     size_t VectorLength);
  
  virtual ~G4CopyPhysicsOrderedFreeVector();
  
  ////////////
  // Methods
  ////////////
  
  void InsertValues(G4double energy, G4double value); 

  G4double GetLowEdgeEnergy(size_t binNumber) const;
  
  G4double GetMaxValue();
  
  G4double GetMinValue();
  
  G4double GetEnergy(G4double aValue);
  
  G4double GetMaxLowEdgeEnergy();
  
  G4double GetMinLowEdgeEnergy();
  G4double CopyFindLinearEnergy(G4double rand) const;
  
  void DumpValues();

 private:

  virtual size_t FindBinLocation(G4double theEnergy) const;
  
  size_t FindValueBinLocation(G4double aValue);
  
  G4double LinearInterpolationOfEnergy(G4double aValue, size_t theLocBin);
};

#include "G4CopyPhysicsOrderedFreeVector.icc"

#endif /* G4CopyPhysicsOrderedFreeVector_h */






