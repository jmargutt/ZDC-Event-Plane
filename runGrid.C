#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"

void runGrid()
{
  // load libraries
  gSystem->Load("libCore.so");
  gSystem->Load("libTree.so");
  gSystem->Load("libGeom");
  gSystem->Load("libVMC");
  gSystem->Load("libXMLIO");
  gSystem->Load("libPhysics");
  gSystem->Load("libSTEERBase");
  gSystem->Load("libESD");
  gSystem->Load("libAOD");
  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISalice");
  gSystem->Load("libANALYSISaliceBase");
  gSystem->Load("libCORRFW");
  gSystem->Load("libOADB");
  gSystem->Load("libPWGflowBase.so");
  gSystem->Load("libPWGflowTasks.so");
  gSystem->Load("libqpythia");
  gSystem->Load("libEGPythia6");
  gSystem->Load("libAliPythia6");
  gSystem->Load("libAliPythia8");
  gSystem->Load("libTHerwig");
  gSystem->Load("libHERWIG");
  
  // Use AliRoot includes to compile our task
  gROOT->ProcessLine(".include $ALICE_ROOT/include");
  gROOT->ProcessLine(".include $ALICE_PHYSICS/include");
  
  // Create and configure the alien handler plugin
  gROOT->LoadMacro("CreateAlienHandler.C");
  AliAnalysisGrid *alienHandler = CreateAlienHandler();
  
  if (!alienHandler) return;
  
  // Create the analysis manager
  AliAnalysisManager *mgr = new AliAnalysisManager("testAnalysis");
  
  // Connect plug-in to the analysis manager
  mgr->SetGridHandler(alienHandler);
  
  // create an input handler
  AliVEventHandler* inputH = new AliAODInputHandler();
  mgr->SetInputEventHandler(inputH);
  
  TGrid::Connect("alien://");
  
  gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
  AliMultSelectionTask *task = AddTaskMultSelection(kFALSE);
  task->SetSelectedTriggerClass(AliVEvent::kINT7);
  //  task->SetAlternateOADBforEstimators("LHC15o-DefaultMC-HIJING"); // only if MC !!!!!!!!!!!!
  mgr->AddTask(task);
  
  gROOT->LoadMacro("AliAnalysisTaskZDCEP.cxx++");
  gROOT->LoadMacro("AddTaskZDCEP.C");
  
  AddTaskZDCEP("alien:///alice/cern.ch/user/j/jmargutt/15oHI_ZDCcalibVar_CenVtxCen_VtxRbR_Ecom.root");
  
  // check if we can initialize the manager
  if (!mgr->InitAnalysis()) {
    printf("Impossible to initialize the manager!\n");
    return;
  }
  // print to screen how the analysis is progressing
  mgr->SetUseProgressBar(1, 25);
  // print the status of the manager to screen
  mgr->PrintStatus();
  // Start analysis in grid.
  mgr->StartAnalysis("grid");
  
};
