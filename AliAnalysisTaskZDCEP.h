#ifndef  AliAnalysisTaskZDCEP_H
#define  AliAnalysisTaskZDCEP_H

#include "AliAnalysisTaskSE.h"
#include "AliAnalysisUtils.h"

class AliAnalysisUtils;
class AliAnalysisTaskSE;

class  AliAnalysisTaskZDCEP : public  AliAnalysisTaskSE
{
 public:
  //  two  class  constructors
  AliAnalysisTaskZDCEP();
  AliAnalysisTaskZDCEP(const  char *name);
  //  class  destructor
  virtual ~AliAnalysisTaskZDCEP();
  //  called  once  at  beginning  or  runtime
  virtual void UserCreateOutputObjects();
  //  called  for  each  event
  virtual void UserExec(Option_t* option);
  //  called  at  end  of  analysis
  virtual void Terminate(Option_t* option);
  
  void SetZDCCalibList(TList* const wlist) {this->fZDCCalibList = wlist;}
  TList* GetZDCCalibList() const {return this->fZDCCalibList;}
  
 private:
  
  TList* fOutputList;             //! output list containing ZDC q-vectors
  TList* fHistList;               //! output list containing QA histograms
  Double_t fZDCGainAlpha;         //
  Bool_t fUseMCTune;              //
  TList *fZDCCalibList;           // list containing ZDC calibration
  TProfile* fZDCQHist[8];         //!
  AliFlowVector* fZDCFlowVect[2]; //! ZDC q-vectors
  Int_t fCachedRunNum;            //
  const static Int_t fnRun = 92;  //
  Int_t fRunList[fnRun];          // run list
  TList *fQVecListRun[fnRun];     //! run-by-run list
  
  TProfile* fZDCQVecRaw[fnRun][4];       //! q_x,y distribution
  TProfile* fZDCQVecRec[fnRun][4];       //! q_x,y distribution (after recentering)
  TProfile* fZDCQVecRecHig[fnRun][4];    //! Higher harmonics terms (after recentering)
  TH2F* fZDCEVPLRec[fnRun][2];           //! Event Plane (after recentering)
  TH2F* fTH2PsiAPsiC;                    //! 
  
  AliAnalysisUtils* fAnalysisUtils; //!
  
  ClassDef(AliAnalysisTaskZDCEP, 2);
};

#endif