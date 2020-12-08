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
/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Electron.hh"
#include "G4MuonMinus.hh"
#include "G4NeutrinoE.hh"
#include "G4NeutrinoMu.hh"
#include "G4NeutrinoTau.hh"
#include "G4AntiNeutrinoE.hh"
#include "G4AntiNeutrinoMu.hh"
#include "G4AntiNeutrinoTau.hh"
#include "G4SystemOfUnits.hh"
#include "Analysis.hh"
#include <string>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// https://indico.ihep.ac.cn/event/9624/session/4/contribution/15/material/slides/0.pdf
namespace { G4Mutex myMutex = G4MUTEX_INITIALIZER; }

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(nullptr),
   fElectron(nullptr),
   fPositron(nullptr),
   fMuonM(nullptr),
   fMuonP(nullptr),
   fPionM(nullptr),
   fPionP(nullptr),
   fPi0(nullptr),
   fProton(nullptr),
   fProtonB(nullptr),
   fNeutron(nullptr),
   fNeutronB(nullptr),
   fPhoton(nullptr),
   fK0(nullptr),
   fK0B(nullptr),
   fKL0(nullptr),
   fKS0(nullptr),
   fKaonM(nullptr),
   fKaonP(nullptr),
   fSigmaM(nullptr),
   fSigmaMB(nullptr),
   fSigmaP(nullptr),
   fSigmaPB(nullptr),
   fSigma0(nullptr),
   fSigma0B(nullptr),
   fLambda0(nullptr),
   fLambda0B(nullptr),
   fDP(nullptr),
   fDM(nullptr),
   fD0(nullptr),
   fD0B(nullptr),
   fDsP(nullptr),
   fDsM(nullptr),
   fAlpha(nullptr),
   fAlphaB(nullptr),
   rand_general(nullptr)
{
  G4AutoLock lock(&myMutex);
  
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

  // default particle kinematic
  //
  //G4ParticleDefinition* particleDefinition = G4Electron::Definition();
  //G4ParticleDefinition* particleDefinition = G4MuonMinus::Definition();
  //G4ParticleDefinition* particleDefinition = G4NeutrinoE::Definition();

  auto particleTable = G4ParticleTable::GetParticleTable();
  fElectron = particleTable->FindParticle("e-");
  fPositron = particleTable->FindParticle("e+");
  fMuonM = particleTable->FindParticle("mu-");
  fMuonP = particleTable->FindParticle("mu+");
  fPionM = particleTable->FindParticle("pi-");
  fPionP = particleTable->FindParticle("pi+");
  fPi0 = particleTable->FindParticle("pi0");
  fProton = particleTable->FindParticle("proton");
  fProtonB = particleTable->FindParticle("anti_proton");
  fNeutron = particleTable->FindParticle("neutron");
  fNeutronB = particleTable->FindParticle("anti_neutron");
  fPhoton = particleTable->FindParticle("gamma");
  fK0 = particleTable->FindParticle("kaon0");
  fK0B = particleTable->FindParticle("anti_kaon0");
  fKL0 = particleTable->FindParticle("kaon0L");
  fKS0 = particleTable->FindParticle("kaon0S");
  fKaonM = particleTable->FindParticle("kaon-");
  fKaonP = particleTable->FindParticle("kaon+");
  fSigmaM = particleTable->FindParticle("sigma-");
  fSigmaMB = particleTable->FindParticle("anti_sigma-");
  fSigmaP = particleTable->FindParticle("sigma+");
  fSigmaPB = particleTable->FindParticle("anti_sigma+");
  fSigma0 = particleTable->FindParticle("sigma0");
  fSigma0B = particleTable->FindParticle("anti_sigma0");
  fLambda0 = particleTable->FindParticle("lambda");
  fLambda0B = particleTable->FindParticle("anti_lambda");
  fDP = particleTable->FindParticle("D+");
  fDM = particleTable->FindParticle("D-");
  fD0 = particleTable->FindParticle("D0");
  fD0B = particleTable->FindParticle("anti_D0");
  fDsP = particleTable->FindParticle("Ds+");
  fDsM = particleTable->FindParticle("Ds-");
  fAlpha = particleTable->FindParticle("alpha");
  fAlphaB = particleTable->FindParticle("anti_alpha");

  double prob[80] = {2.26737490143638E-13, 1.50857611716220E-13, 9.39261858919731E-14, 5.84739207638017E-14, 3.24571040230013E-14, 2.21104873511029E-14, 1.74440788666807E-14, 1.13615038968873E-14, 1.91376476103987E-14, 1.15572681715077E-14, 7.08157341925533E-15, 5.76416620948928E-15, 7.81623154711213E-15, 6.56754136726407E-15, 9.16825328611326E-15, 9.76938614058058E-15, 9.18127643556841E-15, 1.33602764458908E-14, 1.17961528918552E-14, 1.60210373809235E-14, 1.87954543243886E-14, 2.09410212445005E-14, 1.88130671961507E-14, 1.39192548034658E-14, 1.14311556205024E-14, 1.30576528682980E-14, 7.99847015869422E-15, 8.28721827849399E-15, 8.20053542430517E-15, 8.50771102461677E-15, 5.55066657512325E-15, 6.32841540658163E-15, 3.65932255258140E-15, 4.73664423730078E-15, 2.87956818442589E-15, 2.38836106830032E-15, 3.91517917503873E-15, 2.14509513316961E-15, 2.12635229162121E-15, 2.12444480825151E-15, 1.55989332342560E-15, 1.66292857492464E-15, 1.62162209852412E-15, 2.51204734702591E-15, 2.98771625146124E-15, 1.98064518639804E-15, 1.30185775911172E-15, 1.06588489637830E-15, 1.30072623716252E-15, 1.95825349026024E-15, 1.82974778894870E-15, 1.75743260276838E-15, 7.85581568133859E-16, 8.91558493721251E-16, 4.99882857190907E-16, 5.11922992775425E-16, 5.82075402896300E-16, 5.63767765090528E-16, 1.30568419050939E-15, 3.48866039521030E-16, 1.28705004775493E-15, 3.03361773496548E-16, 1.55472908916981E-16, 1.50737952570158E-16, 2.27297875437635E-16, 1.42200831326507E-16, 7.58404433741371E-17, 1.19773883004941E-16, 2.84401662653014E-17, 6.06723546993097E-17, 1.89601108435342E-17, 1.32720775904740E-17, 1.89601108435342E-17, 5.68803325306028E-17, 0, 1.89601108435342E-17, 3.79202216870685E-17, 1.89601108435342E-17, 1.89601108435342E-17, 0};
  rand_general = new G4RandGeneral(prob,80);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  G4AutoLock lock(&myMutex);
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// This function is called at the begining of event
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{  
  G4AutoLock lock(&myMutex);

  G4double worldYHalfLength = 0.;
  G4double worldZHalfLength = 0.;
  auto worldLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
  G4Box* worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
  if ( worldBox ) {
    worldYHalfLength = worldBox->GetYHalfLength();  
    worldZHalfLength = worldBox->GetZHalfLength();  
  }
  else {
    G4ExceptionDescription msg;
    msg << "World volume of box shape not found." << G4endl;
    msg << "Perhaps you have changed geometry." << G4endl;
    msg << "The gun will be place in the center.";
    G4Exception("B4PrimaryGeneratorAction::GeneratePrimaries()",
      "MyCode0002", JustWarning, msg);
  }

  double rnd = rand_general->shoot();
  double_t energy = 0;
  if(rnd<0.0125)      energy = G4RandFlat::shoot(1,50);// To avoid shoot ambiguity compiler error
  else if(rnd<0.0250) energy = G4RandFlat::shoot(50,100);
  else if(rnd<0.0375) energy = G4RandFlat::shoot(100,150);
  else if(rnd<0.0500) energy = G4RandFlat::shoot(150,200);
  else if(rnd<0.0625) energy = G4RandFlat::shoot(200,250);
  else if(rnd<0.0750) energy = G4RandFlat::shoot(250,300);
  else if(rnd<0.0875) energy = G4RandFlat::shoot(300,350);
  else if(rnd<0.1000) energy = G4RandFlat::shoot(350,400);
  else if(rnd<0.1125) energy = G4RandFlat::shoot(400,450);
  else if(rnd<0.1250) energy = G4RandFlat::shoot(450,500);
  else if(rnd<0.1375) energy = G4RandFlat::shoot(500,550);
  else if(rnd<0.1500) energy = G4RandFlat::shoot(550,600);
  else if(rnd<0.1625) energy = G4RandFlat::shoot(600,650);
  else if(rnd<0.1750) energy = G4RandFlat::shoot(650,700);
  else if(rnd<0.1875) energy = G4RandFlat::shoot(700,750);
  else if(rnd<0.2000) energy = G4RandFlat::shoot(750,800);
  else if(rnd<0.2125) energy = G4RandFlat::shoot(800,850);
  else if(rnd<0.2250) energy = G4RandFlat::shoot(850,900);
  else if(rnd<0.2375) energy = G4RandFlat::shoot(900,950);
  else if(rnd<0.2500) energy = G4RandFlat::shoot(950,1000);
  else if(rnd<0.2625) energy = G4RandFlat::shoot(1000,1050);
  else if(rnd<0.2750) energy = G4RandFlat::shoot(1050,1100);
  else if(rnd<0.2875) energy = G4RandFlat::shoot(1100,1150);
  else if(rnd<0.3000) energy = G4RandFlat::shoot(1150,1200);
  else if(rnd<0.3125) energy = G4RandFlat::shoot(1200,1250);
  else if(rnd<0.3250) energy = G4RandFlat::shoot(1250,1300);
  else if(rnd<0.3375) energy = G4RandFlat::shoot(1300,1350);
  else if(rnd<0.3500) energy = G4RandFlat::shoot(1350,1400);
  else if(rnd<0.3625) energy = G4RandFlat::shoot(1400,1450);
  else if(rnd<0.3750) energy = G4RandFlat::shoot(1450,1500);
  else if(rnd<0.3875) energy = G4RandFlat::shoot(1500,1550);
  else if(rnd<0.4000) energy = G4RandFlat::shoot(1550,1600);
  else if(rnd<0.4125) energy = G4RandFlat::shoot(1600,1650);
  else if(rnd<0.4250) energy = G4RandFlat::shoot(1650,1700);
  else if(rnd<0.4375) energy = G4RandFlat::shoot(1700,1750);
  else if(rnd<0.4500) energy = G4RandFlat::shoot(1750,1800);
  else if(rnd<0.4625) energy = G4RandFlat::shoot(1800,1850);
  else if(rnd<0.4750) energy = G4RandFlat::shoot(1850,1900);
  else if(rnd<0.4875) energy = G4RandFlat::shoot(1900,1950);
  else if(rnd<0.5000) energy = G4RandFlat::shoot(1950,2000);
  else if(rnd<0.5125) energy = G4RandFlat::shoot(2000,2050);
  else if(rnd<0.5250) energy = G4RandFlat::shoot(2050,2100);
  else if(rnd<0.5375) energy = G4RandFlat::shoot(2100,2150);
  else if(rnd<0.5500) energy = G4RandFlat::shoot(2150,2200);
  else if(rnd<0.5625) energy = G4RandFlat::shoot(2200,2250);
  else if(rnd<0.5750) energy = G4RandFlat::shoot(2250,2300);
  else if(rnd<0.5875) energy = G4RandFlat::shoot(2300,2350);
  else if(rnd<0.6000) energy = G4RandFlat::shoot(2350,2400);
  else if(rnd<0.6125) energy = G4RandFlat::shoot(2400,2450);
  else if(rnd<0.6250) energy = G4RandFlat::shoot(2450,2500);
  else if(rnd<0.6375) energy = G4RandFlat::shoot(2500,2550);
  else if(rnd<0.6500) energy = G4RandFlat::shoot(2550,2600);
  else if(rnd<0.6625) energy = G4RandFlat::shoot(2600,2650);
  else if(rnd<0.6750) energy = G4RandFlat::shoot(2650,2700);
  else if(rnd<0.6875) energy = G4RandFlat::shoot(2700,2750);
  else if(rnd<0.7000) energy = G4RandFlat::shoot(2750,2800);
  else if(rnd<0.7125) energy = G4RandFlat::shoot(2800,2850);
  else if(rnd<0.7250) energy = G4RandFlat::shoot(2850,2900);
  else if(rnd<0.7375) energy = G4RandFlat::shoot(2900,2950);
  else if(rnd<0.7500) energy = G4RandFlat::shoot(2950,3000);
  else if(rnd<0.7625) energy = G4RandFlat::shoot(3000,3050);
  else if(rnd<0.7750) energy = G4RandFlat::shoot(3050,3100);
  else if(rnd<0.7875) energy = G4RandFlat::shoot(3100,3150);
  else if(rnd<0.8000) energy = G4RandFlat::shoot(3150,3200);
  else if(rnd<0.8125) energy = G4RandFlat::shoot(3200,3250);
  else if(rnd<0.8250) energy = G4RandFlat::shoot(3250,3300);
  else if(rnd<0.8375) energy = G4RandFlat::shoot(3300,3350);
  else if(rnd<0.8500) energy = G4RandFlat::shoot(3350,3400);
  else if(rnd<0.8625) energy = G4RandFlat::shoot(3400,3450);
  else if(rnd<0.8750) energy = G4RandFlat::shoot(3450,3500);
  else if(rnd<0.8875) energy = G4RandFlat::shoot(3500,3550);
  else if(rnd<0.9000) energy = G4RandFlat::shoot(3550,3600);
  else if(rnd<0.9125) energy = G4RandFlat::shoot(3600,3650);
  else if(rnd<0.9250) energy = G4RandFlat::shoot(3650,3700);
  else if(rnd<0.9375) energy = G4RandFlat::shoot(3700,3750);
  else if(rnd<0.9500) energy = G4RandFlat::shoot(3750,3800);
  else if(rnd<0.9625) energy = G4RandFlat::shoot(3800,3850);
  else if(rnd<0.9750) energy = G4RandFlat::shoot(3850,3900);
  else if(rnd<0.9875) energy = G4RandFlat::shoot(3900,3950);
  else                energy = G4RandFlat::shoot(3950,4000);
  
  double x_muon = G4RandFlat::shoot(-7.50,7.50);
  double y_muon = G4RandFlat::shoot(-6.25,6.25);
  
  // single particle gun:
  fParticleGun->SetParticleDefinition(fMuonM);
  //fParticleGun->SetParticleEnergy(1000.*GeV);
  fParticleGun->SetParticleEnergy(energy*GeV);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1));
  fParticleGun->SetParticlePosition(G4ThreeVector(x_muon*cm,y_muon*cm,-worldZHalfLength));
  fParticleGun->GeneratePrimaryVertex(anEvent);

  e_beam = energy; pdg_beam = 13; x_beam = x_muon; y_beam = y_muon;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
