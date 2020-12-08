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
/// \file TrackingAction.cc
/// \brief Implementation of the TrackingAction class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "TrackingAction.hh"

#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4StepStatus.hh"
#include "G4ParticleTypes.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "g4root.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction()
:G4UserTrackingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PostUserTrackingAction(const G4Track* track)
{
  if(track->GetParentID()!=0) return;
  
  // keep only boundary particles
  G4StepStatus status = track->GetStep()->GetPostStepPoint()->GetStepStatus();
  //printf("%d ",(int)status);
  //if (status != fGeomBoundary) return;
  //printf("Xin2\n");

  G4String name1 = track->GetStep()->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetName();
  G4String name2;
  if(status != fWorldBoundary)
    name2 = track->GetStep()->GetPostStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetName();
  if(true) printf("%s %s\n",name1.data(),name2.data());
  //G4cout << "Xin: " << name1 << " " << name2 << G4endl;
  //printf("%s ",name1.data());
  return;
  
  // //if(name1 == "Rock" || name1 != "Station") return;
  // //if(! ((name1 == "Rock" || name1 == "Station") && (name2 == "Calorimeter" || name2 == "Layer" || name2 == "AbsoLV"))) return;
 
  // const G4ParticleDefinition* particle = track->GetParticleDefinition();
  // G4String name    = particle->GetParticleName();
  // //G4int particleID = particle->GetPDGEncoding();
  // G4double energy  = track->GetKineticEnergy();

  // // histograms: enery flow
  // //
  // G4AnalysisManager* analysis = G4AnalysisManager::Instance();
 
  // G4int ih = 0; 
  // G4String type   = particle->GetParticleType();      
  // G4double charge = particle->GetPDGCharge();
  // if(track->GetParentID()==0) ih = 1;
  // else if (charge > 3.) ih = 8; 
  // else if (particle == G4Gamma::Gamma())             ih = 2;
  // else if (particle == G4Electron::Electron())       ih = 3;
  // else if (particle == G4Positron::Positron())       ih = 3;  
  // else if (particle == G4MuonPlus::MuonPlus())       ih = 4;
  // else if (particle == G4MuonMinus::MuonMinus())     ih = 4;
  // else if (particle == G4Neutron::Neutron())         ih = 5;
  // else if (particle == G4AntiNeutron::AntiNeutron()) ih = 5;
  // else if (particle == G4Proton::Proton())           ih = 6;
  // else if (particle == G4AntiProton::AntiProton())   ih = 6;
  // else if (particle == G4Alpha::Alpha())             ih = 7;       
  // else if (particle == G4AntiAlpha::AntiAlpha())     ih = 7;       
  // else if (type == "nucleus")                        ih = 8;
  // else if (type == "baryon")                         ih = 9;         
  // else if (type == "meson")                          ih = 10;
  // else if (type == "lepton")                         ih = 11;        
  // if (ih > 0) analysis->FillH1(ih,energy/GeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
