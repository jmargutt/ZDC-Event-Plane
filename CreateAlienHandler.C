AliAnalysisGrid* CreateAlienHandler()
{
  // Check if user has a valid token, otherwise make one. This has limitations.
  // One can always follow the standard procedure of calling alien-token-init then
  // source /tmp/gclient_env_$UID in the current shell.
  // if (!AliAnalysisGrid::CreateToken()) return NULL;
  AliAnalysisAlien *plugin = new AliAnalysisAlien();
  //plugin->SetUser("jmargutt");
  
  plugin->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/EMCAL -I$ALICE_ROOT/ANALYSIS -I$ALICE_ROOT/PWGGA -I$ALICE_ROOT/PWGCF -I$ALICE_ROOT/PWGHF -I$ALICE_ROOT/PWGHF/hfe -I$ALICE_ROOT/CONTAINERS -I$ALICE_ROOT/STEER/STEER -I$ALICE_ROOT/STEER/STEERBase -I$ALICE_ROOT/STEER/ESD -I$ALICE_ROOT/STEER/AOD -I$ALICE_ROOT/OADB -I$ALICE_ROOT/PWGHF/base  -I$ALICE_ROOT/include -I$ALICE_ROOT/ITS -I$ALICE_ROOT/TPC -I$ALICE_ROOT/CONTAINERS -I$ALICE_ROOT/STEER -I$ALICE_ROOT/TRD -I$ALICE_ROOT/macros -I$ALICE_ROOT/ANALYSIS  -I$ALICE_ROOT/OADB -I$ALICE_ROOT/PWG/FLOW/Base -I$ALICE_PHYSICS/include -g ");
  
  // Declare all libraries (other than the default ones for the framework. These will be
  // loaded by the generated analysis macro. Add all extra files (task .cxx/.h) here.
  plugin->SetAdditionalLibs("libPWGflowBase.so AliAnalysisTaskZDCEP.cxx AliAnalysisTaskZDCEP.h");
  // Declare the analysis source files names separated by blancs. To be compiled runtime
  // using ACLiC on the worker nodes.
  plugin->SetAnalysisSource("AliAnalysisTaskZDCEP.cxx");
  // Set overwrite mode
  plugin->SetOverwriteMode(kTRUE);
  // Set the run mode (can be "full", "test", "offline", "submit" or "terminate")
  plugin->SetRunMode("test");
  // Are you before the last stage?
  Bool_t pre_final_stage = kTRUE;
  // Set versions of used packages
  plugin->SetAPIVersion("V1.1x");
  // plugin->SetROOTVersion("v5-34-08-6");
  // plugin->SetAliROOTVersion("vAN-20140714");
  // plugin->SetAliROOTVersion("v5-08-00-1");
  plugin->SetAliPhysicsVersion("vAN-20170410-1");
  // Declare input data to be processed.
  // Define production directory LFN
  
  // LHC15o - high IR
  plugin->SetGridDataDir("/alice/data/2015/LHC15o");
  plugin->SetDataPattern("*/pass1/AOD/*AOD.root");
  plugin->SetRunPrefix("000");   // real data  // LHC10h, global quality = 1, TPC OK, #89 138275
  Int_t runArray[] = {246991};//, 246989, 246984, 246982, 246980, 246948, 246945, 246928, 246851, 246847, 246846, 246845, 246844, 246810, 246809};
  
  for (Int_t i = 0; i < sizeof(runArray); i++)
  {
    if (i == sizeof(runArray) / sizeof(runArray[1])) break;
    plugin->AddRunNumber(runArray[i]);
  }
  
  // plugin->SetOutputSingleFolder("output");
  plugin->SetOutputToRunNo();
  // Define alien work directory where all files will be copied. Relative to alien $HOME.
  //plugin->SetGridWorkingDir("MC12_b_0510");
  //plugin->SetGridWorkingDir("MC12_c_1020");
  //plugin->SetGridWorkingDir("MC12_d_2030");
  //plugin->SetGridWorkingDir("MC12_e_3040");
  //plugin->SetGridWorkingDir("MC12_f_4050");
  plugin->SetGridWorkingDir("ZDCEP");
  // Declare alien output directory. Relative to working directory.
  plugin->SetGridOutputDir("output"); // In this case will be $HOME/work/output
  // Declare the output file names separated by blancs (can be like: file.root or file.root@ALICE::Niham::File)
  plugin->SetDefaultOutputs();
  //   plugin->SetOutputFiles("ParticleWeights.root");
  // Optionally define the files to be archived.
  //   plugin->SetOutputArchive("log_archive.zip:stdout,stderr@ALICE::NIHAM::File root_archive.zip:*.root@ALICE::NIHAM::File");
  //   plugin->SetOutputArchive("log_archive.zip:stdout,stderr");
  // Optionally set a name for the generated analysis macro (default MyAnalysis.C)
  plugin->SetAnalysisMacro("MyAnalysis.C");
  // Optionally set maximum number of input files/subjob (default 100, put 0 to ignore)
  plugin->SetSplitMaxInputFileNumber(5);
  // Optionally modify the executable name (default analysis.sh)
  plugin->SetExecutable("analysis.sh");
  // Optionally set number of failed jobs that will trigger killing waiting sub-jobs.
  plugin->SetMaxInitFailed(90);
  // Optionally resubmit threshold.
  plugin->SetMasterResubmitThreshold(95);
  // Optionally set time to live (default 30000 sec)
  plugin->SetTTL(30000);
  // Optionally set input format (default xml-single)
  plugin->SetInputFormat("xml-single");
  // Optionally modify the name of the generated JDL (default analysis.jdl)
  plugin->SetJDLName("analysis.jdl");
  // Optionally modify job price (default 1)
  plugin->SetPrice(1);
  // Optionally modify split mode (default 'se')
  plugin->SetSplitMode("se");
  // Optionally keep log files
  plugin->SetKeepLogs(kTRUE);
  // Optionally set number of runs per masterjob:
  plugin->SetNrunsPerMaster(15);
  // ???
  plugin->SetMergeViaJDL(pre_final_stage);
  plugin->SetMaxMergeStages(2);
  
  // Set number of files to process in "test" mode
  plugin->SetNtestFiles(1);
  
  return plugin;
}
