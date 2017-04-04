AliAnalysisTask * AddTaskZDCEP(TString ZDCCalibFileName,
                               TString Label="",
                               const char* suffix="")
{
  // load libraries
  gSystem->Load("libGeom");
  gSystem->Load("libVMC");
  gSystem->Load("libXMLIO");
  gSystem->Load("libPhysics");
  gSystem->Load("libCore.so");
  gSystem->Load("libTree.so");
  gSystem->Load("libSTEERBase.so");
  gSystem->Load("libESD.so");
  gSystem->Load("libAOD.so");
  gSystem->Load("libANALYSIS.so");
  gSystem->Load("libANALYSISalice.so");
  gSystem->Load("libOADB.so");
  gSystem->Load("libPWGflowBase.so");
  gSystem->Load("libPWGflowTasks.so");
  
  gROOT->ProcessLine(".include $ALICE_ROOT/include");
  gROOT->ProcessLine(".include $ALICE_PHYSICS/include");
  gSystem->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/EMCAL -I$ALICE_ROOT/ANALYSIS -I$ALICE_ROOT/OCDB -I$ALICE_ROOT/STEER/macros -I$ALICE_ROOT/include -I$ALICE_ROOT/ITS -I$ALICE_ROOT/TPC -I$ALICE_ROOT/TRD -I$ALICE_ROOT/ZDC -I$ALICE_ROOT/macros -I$ALICE_PHYSICS -I$ALICE_PHYSICS/include -I$ALICE_PHYSICS/OADB $ALICE_PHYSICS/OADB/macros -I$ALICE_PHYSICS/PWGGA -I$ALICE_PHYSICS/PWGCF -I$ALICE_PHYSICS/PWGHF -I$ALICE_PHYSICS/TENDER -I$ALICE_PHYSICS/TENDER/Tender -I$ALICE_PHYSICS/TENDER/TenderSupplies -I$ALICE_PHYSICS/PARfiles -I$ALICE_PHYSICS/PWGCF/FLOW/macros I$ALICE_PHYSICS/PWGPP/ZDC -g ");
  
  // the manager is static, so get the existing manager via the static method
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    printf("No analysis manager to connect to!\n");
    return NULL;
  }
  
  // just to see if all went well, check if the input event handler has been connected
  if (!mgr->GetInputEventHandler()) {
    printf("This task requires an input event handler!\n");
    return NULL;
  }
  
  // get the default name of the output file ("AnalysisResults.root")
  TString file = "AnalysisResults";
  
  // get the common input container from the analysis manager
  AliAnalysisDataContainer *cinput = mgr->GetCommonInputContainer();
  
  // create the flow analysis tasks
  TString AnalysisTaskName = "AnalysisTask";
  AnalysisTaskName += Label;
  AnalysisTaskName += suffix;
  AliAnalysisTaskZDCEP *taskZDC = new AliAnalysisTaskZDCEP(AnalysisTaskName);
  taskZDC->SelectCollisionCandidates(AliVEvent::kINT7);
  
  // add list for ZDC towers gain equalization
  TString ZDCTowerEqFileName = "alien:///alice/cern.ch/user/j/jmargutt/15oHI_EZDCcalib.root";
  TFile* ZDCTowerEqFile = TFile::Open(ZDCTowerEqFileName,"READ");
  gROOT->cd();
  TList* ZDCTowerEqList = (TList*)(ZDCTowerEqFile->FindObjectAny("EZNcalib"));
  if(ZDCTowerEqList) {
    taskZDC->SetTowerEqList(ZDCTowerEqList);
    cout << "ZDCTowerEq set (from " <<  ZDCTowerEqFileName.Data() << ")" << endl;
  } else {
    cout << "ERROR: ZDCTowerEqList not found!" << endl;
    exit(1);
  }
  delete ZDCTowerEqFile;
  
  // add list for ZDC Q-vector re-centering
  TFile* ZDCCalibFile = TFile::Open(ZDCCalibFileName,"READ");
  if(!ZDCCalibFile) {
    cout << "ERROR: ZDC calibration not found!" << endl;
    exit(1);
  }
  TList* ZDCCalibList = dynamic_cast<TList*>(ZDCCalibFile->FindObjectAny("Q Vectors"));
  if(ZDCCalibList) {
    taskZDC->SetZDCCalibList(ZDCCalibList);
    cout << "ZDC calibration set (from " <<  ZDCCalibFileName.Data() << ")" << endl;
  }
  delete ZDCCalibFile;
  
  // connect the task to the analysis manager
  mgr->AddTask(taskZDC);
  
  // initialize output name
  TString outputQC = file;
  outputQC += Label;
  outputQC += suffix;
  outputQC += ".root";
  // create and connect the output containers
  AliAnalysisDataContainer *coutputQC = mgr->CreateContainer(outputQC.Data(),
                                                             TList::Class(),
                                                             AliAnalysisManager::kOutputContainer,
                                                             outputQC);
  // connect the output of the flow event task to the flow analysis task
  mgr->ConnectInput(taskZDC, 0, cinput);
  // and connect the output of the flow analysis task to the output container
  // which will be written to the output file
  mgr->ConnectOutput(taskZDC, 2, coutputQC);
  
  return taskZDC;
}

