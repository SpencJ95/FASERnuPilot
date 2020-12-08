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
/// \file electromagnetic/TestEm5/src/SteppingAction.cc
/// \brief Implementation of the SteppingAction class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "g4root.hh"
#include "Analysis.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction()
:G4UserSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  const G4Track* track = aStep->GetTrack();
  //if(track->GetParentID()!=0) return;

  G4StepStatus status = aStep->GetPostStepPoint()->GetStepStatus();
  if(status != fGeomBoundary) return;
  
  G4String name1 = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetName();
  G4String name2;
  if(status != fWorldBoundary) name2 = aStep->GetPostStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetName();

  if(!( name1=="Gap" && name2=="AbsoLV" )) return;
  //printf("Xin1: %s %s\n",name1.data(),name2.data());
  
  const G4ParticleDefinition* particle = track->GetParticleDefinition();
  G4String name    = particle->GetParticleName();
  G4int particleID = particle->GetPDGEncoding();
  G4String type    = particle->GetParticleType();      
  G4double chargeP = particle->GetPDGCharge();
  G4double energy  = aStep->GetPreStepPoint()->GetKineticEnergy();
  //printf("Xin2: %d %g\n",track->GetParentID(),energy/GeV);

  // histograms: enery flow
  //
  G4AnalysisManager* analysis = G4AnalysisManager::Instance();
 
  G4int ih = 0; 
  if(track->GetParentID()==0)                        {
    ih = 1;
    pdg_primary = particleID;
    e_primary = energy;
    x_primary = aStep->GetPreStepPoint()->GetPosition().x();
    y_primary = aStep->GetPreStepPoint()->GetPosition().y();
  }
  else if (particle == G4Gamma::Gamma())             ih = 2;
  else if (particle == G4Electron::Electron())       ih = 3;
  else if (particle == G4Positron::Positron())       ih = 3;  
  else if (particle == G4MuonPlus::MuonPlus())       ih = 4;
  else if (particle == G4MuonMinus::MuonMinus())     ih = 4;
  else if (particle == G4Neutron::Neutron())         {
    ih = 5;
    // pdg_neutron = particleID;
    // e_neutron = energy;
    // x_neutron = aStep->GetPreStepPoint()->GetPosition().x();
    // y_neutron = aStep->GetPreStepPoint()->GetPosition().y();
  }
  else if (particle == G4AntiNeutron::AntiNeutron()) {
    ih = 6;
    // pdg_neutron = particleID;
    // e_neutron = energy;
    // x_neutron = aStep->GetPreStepPoint()->GetPosition().x();
    // y_neutron = aStep->GetPreStepPoint()->GetPosition().y();
  }
  else if (particle == G4Proton::Proton())                   ih = 7;
  else if (particle == G4AntiProton::AntiProton())           ih = 8;
  else if (particle == G4PionPlus::PionPlus())               ih = 9;       
  else if (particle == G4PionMinus::PionMinus())             ih = 10;       
  else if (particle == G4PionZero::PionZero())               ih = 11;       
  else if (particle == G4KaonZeroLong::KaonZeroLong())       ih = 12;       
  else if (particle == G4KaonZeroShort::KaonZeroShort())     ih = 13;       
  else if (particle == G4KaonZero::KaonZero())               ih = 14;       
  else if (particle == G4AntiKaonZero::AntiKaonZero())       ih = 15;       
  else if (particle == G4KaonPlus::KaonPlus())               ih = 16;       
  else if (particle == G4KaonMinus::KaonMinus())             ih = 17;       
  else if (particle == G4Lambda::Lambda())                   ih = 18;       
  else if (particle == G4AntiLambda::AntiLambda())           ih = 19;
  else if (particle == G4SigmaPlus::SigmaPlus())             ih = 20;
  else if (particle == G4AntiSigmaPlus::AntiSigmaPlus())     ih = 21;
  else if (particle == G4SigmaMinus::SigmaMinus())           ih = 22;
  else if (particle == G4AntiSigmaMinus::AntiSigmaMinus())   ih = 23;
  else if (particle == G4SigmaZero::SigmaZero())             ih = 24;
  else if (particle == G4AntiSigmaZero::AntiSigmaZero())     ih = 25;
  else if (type == "baryon")                                 ih = 26;         
  else if (type == "meson")                                  ih = 27;
  else if (type == "lepton")                                 ih = 28;

  //printf("Xin3: ih = %d\n",ih);
  if (ih > 0) analysis->FillH1(ih,energy/GeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

