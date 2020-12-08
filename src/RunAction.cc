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
/// \file RunAction.cc
/// \brief Implementation of the RunAction class

#include "RunAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal double e_beam;
G4ThreadLocal int pdg_beam;
G4ThreadLocal double x_beam;
G4ThreadLocal double y_beam;

G4ThreadLocal int pdg_primary;
G4ThreadLocal double e_primary;
G4ThreadLocal double x_primary;
G4ThreadLocal double y_primary;
G4ThreadLocal int pdg_neutron;
G4ThreadLocal double e_neutron;
G4ThreadLocal double x_neutron;
G4ThreadLocal double y_neutron;

G4ThreadLocal int pdgnu_nuEvt;
G4ThreadLocal int pdglep_nuEvt;
G4ThreadLocal double Enu_nuEvt;
G4ThreadLocal double Plep_nuEvt;
G4ThreadLocal int cc_nuEvt;
G4ThreadLocal double x_nuEvt;
G4ThreadLocal double y_nuEvt;
G4ThreadLocal double z_nuEvt;
G4ThreadLocal std::vector<int> cham;
G4ThreadLocal std::vector<int> idz;
G4ThreadLocal std::vector<int> idzsub;
G4ThreadLocal std::vector<int> pdgid;
G4ThreadLocal std::vector<int> id;
G4ThreadLocal std::vector<int> idParent;
G4ThreadLocal std::vector<double> charge;
G4ThreadLocal std::vector<double> x;
G4ThreadLocal std::vector<double> y;
G4ThreadLocal std::vector<double> z;
G4ThreadLocal std::vector<double> px;
G4ThreadLocal std::vector<double> py;
G4ThreadLocal std::vector<double> pz;
G4ThreadLocal std::vector<double> e1;
G4ThreadLocal std::vector<double> e2;
G4ThreadLocal std::vector<double> len;
G4ThreadLocal std::vector<double> edep;

RunAction::RunAction()
 : G4UserRunAction()
{ 
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);     

  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in Analysis.hh
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Create directories 
  analysisManager->SetHistoDirectoryName("histo");
  analysisManager->SetFirstHistoId(1);
  //analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
  // Note: merging ntuples is available only with Root output

  // Book histograms
  const G4String idx[29] = {"h0","h1","h2","h3","h4","h5","h6","h7","h8","h9","h10","h11",
			    "h12","h13","h14","h15","h16","h17","h18","h19","h20","h21",
                            "h22","h23","h24","h25","h26","h27","h28"};
  const G4String title[29] = 
    { "dummy",                                                        //0
      "energy spectrum of primary",                                   //1
      "energy spectrum of emerging gamma",                            //2
      "energy spectrum of emerging e+-",                              //3
      "energy spectrum of emerging mu+-",                             //4
      "energy spectrum of emerging neutrons",                         //5
      "energy spectrum of emerging antineutrons",                     //6
      "energy spectrum of emerging protons",                          //7
      "energy spectrum of emerging antiprotons",                      //8
      "energy spectrum of emerging pion+",                            //9
      "energy spectrum of emerging pion-",                            //10
      "energy spectrum of emerging pion0",                            //11
      "energy spectrum of emerging KL",                               //12
      "energy spectrum of emerging KS",                               //13
      "energy spectrum of emerging K0",                               //14
      "energy spectrum of emerging antiK0",                           //15
      "energy spectrum of emerging K+",                               //16
      "energy spectrum of emerging K-",                               //17
      "energy spectrum of emerging Lambda0",                          //18
      "energy spectrum of emerging antiLambda0",                      //19
      "energy spectrum of emerging Sigma+",                           //20
      "energy spectrum of emerging antiSigma+",                       //21
      "energy spectrum of emerging Sigma-",                           //22
      "energy spectrum of emerging antiSigma-",                       //23
      "energy spectrum of emerging Sigma0",                           //24
      "energy spectrum of emerging antiSigma0",                       //25
      "energy spectrum of all others emerging baryons",               //26
      "energy spectrum of all others emerging mesons",                //27
      "energy spectrum of all others emerging leptons (neutrinos)"    //28
    };

  for (G4int k=1; k<=28; k++) {
    if(k==1) analysisManager->CreateH1(idx[k], title[k], 400, 110, 4110);
    else if(k==2) analysisManager->CreateH1(idx[k], title[k], 200, 0, 200);
    else if(k==3) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==4) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==5) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==6) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==7) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==8) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==9) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==10) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==11) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==12) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==13) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==14) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==15) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==16) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==17) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==18) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==19) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==20) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==21) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==22) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==23) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==24) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==25) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==26) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==27) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
    else if(k==28) analysisManager->CreateH1(idx[k], title[k], 500, 0, 500);
  }

  // // Creating ntuple
  // //
  // analysisManager->CreateNtuple("FASERnuPilot", "");
  // analysisManager->CreateNtupleIColumn("pdg_beam");
  // analysisManager->CreateNtupleDColumn("e_beam");
  // analysisManager->CreateNtupleDColumn("x_beam");
  // analysisManager->CreateNtupleDColumn("y_beam");

  // analysisManager->CreateNtupleIColumn("pdg_primary");
  // analysisManager->CreateNtupleDColumn("e_primary");
  // analysisManager->CreateNtupleDColumn("x_primary");
  // analysisManager->CreateNtupleDColumn("y_primary");
  // analysisManager->CreateNtupleIColumn("pdg_neutron");
  // analysisManager->CreateNtupleDColumn("e_neutron");
  // analysisManager->CreateNtupleDColumn("x_neutron");
  // analysisManager->CreateNtupleDColumn("y_neutron");
  
  //analysisManager->CreateNtupleIColumn("pdgnu_nuEvt");
  //analysisManager->CreateNtupleIColumn("pdglep_nuEvt");
  //analysisManager->CreateNtupleDColumn("Enu_nuEvt");
  //analysisManager->CreateNtupleDColumn("Plep_nuEvt");
  //analysisManager->CreateNtupleIColumn("cc_nuEvt");
  //analysisManager->CreateNtupleDColumn("x_nuEvt");
  //analysisManager->CreateNtupleDColumn("y_nuEvt");
  //analysisManager->CreateNtupleDColumn("z_nuEvt");
  //analysisManager->CreateNtupleIColumn("chamber",cham);
  //analysisManager->CreateNtupleIColumn("iz",idz);
  //analysisManager->CreateNtupleIColumn("izsub",idzsub);
  //analysisManager->CreateNtupleIColumn("pdgid",pdgid);
  //analysisManager->CreateNtupleIColumn("id",id);
  //analysisManager->CreateNtupleIColumn("idParent",idParent);
  //analysisManager->CreateNtupleDColumn("charge",charge);
  //analysisManager->CreateNtupleDColumn("x",x);
  //analysisManager->CreateNtupleDColumn("y",y);
  //analysisManager->CreateNtupleDColumn("z",z);
  //analysisManager->CreateNtupleDColumn("px",px);
  //analysisManager->CreateNtupleDColumn("py",py);
  //analysisManager->CreateNtupleDColumn("pz",pz);
  //analysisManager->CreateNtupleDColumn("e1",e1);
  //analysisManager->CreateNtupleDColumn("e2",e2);
  //analysisManager->CreateNtupleDColumn("len",len);
  //analysisManager->CreateNtupleDColumn("edep",edep);
  
  //analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
  //G4String fileName = "FASERnuPilot";
  analysisManager->OpenFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // print histogram statistics
  //
  auto analysisManager = G4AnalysisManager::Instance();

  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
