#define AliAnalysisTaskZDCEP_cxx

#include "Riostream.h"
#include "AliFlowCommonConstants.h"
#include "AliFlowCommonHist.h"
#include "AliFlowCommonHistResults.h"
#include "TChain.h"

#include "TFile.h"
#include "TList.h"
#include "TGraph.h"
#include "TParticle.h"
#include "TRandom3.h"
#include "TStyle.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TMath.h"
#include "TArrow.h"
#include "TPaveLabel.h"
#include "TCanvas.h"
#include "TVector2.h"
#include "AliFlowEventSimple.h"
#include "AliFlowVector.h"
#include "AliFlowTrackSimple.h"
#include "AliFlowAnalysisCRC.h"
#include "TRandom.h"
#include "TF1.h"
#include "TNtuple.h"
#include "THnSparse.h"
#include "AliAnalysisTaskZDCEP.h"
#include "AliAODTrack.h"
#include "AliAODEvent.h"
#include "AliAODZDC.h"

class TH1;
class TH2;
class TGraph;
class TPave;
class TLatex;
class TMarker;
class TRandom3;
class TObjArray;
class TList;
class TCanvas;
class TSystem;
class TROOT;
class TVector2;
class AliFlowVector;

AliAnalysisTaskZDCEP::AliAnalysisTaskZDCEP () :
AliAnalysisTaskSE (),
fOutputList(0x0),
fHistList(0x0),
fZDCGainAlpha(0.395),
fUseMCTune(kTRUE),
fZDCCalibList(0x0),
fCachedRunNum(0),
fAnalysisUtils(0x0),
fTH2PsiAPsiC(0x0)
{
  //  ROOT  IO  constructor , don 't  allocate  memory  here!
  for (Int_t i=0; i<8; i++) fZDCQHist[i] = NULL;
  for (Int_t i=0; i<2; i++) fZDCFlowVect[i] = NULL;
  
  Int_t dRun10h[] = {139510, 139507, 139505, 139503, 139465, 139438, 139437, 139360, 139329, 139328, 139314, 139310, 139309, 139173, 139107, 139105, 139038, 139037, 139036, 139029, 139028, 138872, 138871, 138870, 138837, 138732, 138730, 138666, 138662, 138653, 138652, 138638, 138624, 138621, 138583, 138582, 138579, 138578, 138534, 138469, 138442, 138439, 138438, 138396, 138364, 138275, 138225, 138201, 138197, 138192, 138190, 137848, 137844, 137752, 137751, 137724, 137722, 137718, 137704, 137693, 137692, 137691, 137686, 137685, 137639, 137638, 137608, 137595, 137549, 137546, 137544, 137541, 137539, 137531, 137530, 137443, 137441, 137440, 137439, 137434, 137432, 137431, 137430, 137366, 137243, 137236, 137235, 137232, 137231, 137230, 137162, 137161};
  for(Int_t r=0; r<fnRun; r++) {
    fRunList[r] = dRun10h[r];
  }
  
  for(Int_t r=0;r<fnRun;r++) {
    fQVecListRun[r] = NULL;
    for (Int_t i=0; i<4; i++) fZDCQVecRaw[r][i] = NULL;
    for (Int_t i=0; i<4; i++) fZDCQVecRec[r][i] = NULL;
    for (Int_t i=0; i<4; i++) fZDCQVecRecHig[r][i] = NULL;
    for (Int_t i=0; i<2; i++) fZDCEVPLRec[r][i] = NULL;
  }
}

//=====================================================================

AliAnalysisTaskZDCEP::AliAnalysisTaskZDCEP(const  char* name)
: AliAnalysisTaskSE(name),
fOutputList(0x0),
fHistList(0x0),
fZDCGainAlpha(0.395),
fUseMCTune(kTRUE),
fZDCCalibList(0x0),
fCachedRunNum(0),
fAnalysisUtils(0x0),
fTH2PsiAPsiC(0x0)
{
  for (Int_t i=0; i<8; i++) fZDCQHist[i] = NULL;
  for (Int_t i=0; i<2; i++) fZDCFlowVect[i] = NULL;
  
  Int_t dRun10h[] = {139510, 139507, 139505, 139503, 139465, 139438, 139437, 139360, 139329, 139328, 139314, 139310, 139309, 139173, 139107, 139105, 139038, 139037, 139036, 139029, 139028, 138872, 138871, 138870, 138837, 138732, 138730, 138666, 138662, 138653, 138652, 138638, 138624, 138621, 138583, 138582, 138579, 138578, 138534, 138469, 138442, 138439, 138438, 138396, 138364, 138275, 138225, 138201, 138197, 138192, 138190, 137848, 137844, 137752, 137751, 137724, 137722, 137718, 137704, 137693, 137692, 137691, 137686, 137685, 137639, 137638, 137608, 137595, 137549, 137546, 137544, 137541, 137539, 137531, 137530, 137443, 137441, 137440, 137439, 137434, 137432, 137431, 137430, 137366, 137243, 137236, 137235, 137232, 137231, 137230, 137162, 137161};
  for(Int_t r=0; r<fnRun; r++) {
    fRunList[r] = dRun10h[r];
  }
  
  for(Int_t r=0;r<fnRun;r++) {
    fQVecListRun[r] = NULL;
    for (Int_t i=0; i<4; i++) fZDCQVecRaw[r][i] = NULL;
    for (Int_t i=0; i<4; i++) fZDCQVecRec[r][i] = NULL;
    for (Int_t i=0; i<4; i++) fZDCQVecRecHig[r][i] = NULL;
    for (Int_t i=0; i<2; i++) fZDCEVPLRec[r][i] = NULL;
  }
  
  DefineInput(0, TChain::Class());
  DefineOutput(1, TList::Class());
  DefineOutput(2, TList::Class());
}

//=====================================================================

AliAnalysisTaskZDCEP::~AliAnalysisTaskZDCEP()
{
  // Destructor
  delete fOutputList;
  delete fHistList;
  delete fAnalysisUtils;
}

//=====================================================================

void AliAnalysisTaskZDCEP::UserCreateOutputObjects ()
{
  //  create  a new  TList  that  OWNS  its  objects
  fOutputList = new TList();
  fOutputList->SetOwner(kTRUE);
  
  for(Int_t c=0;c<2;c++) {
    fZDCFlowVect[c] = new AliFlowVector();
    fOutputList->Add(fZDCFlowVect[c]);
  }

  fHistList = new TList();
  fHistList->SetOwner(kTRUE);
  
  for(Int_t k=0; k<8; k++) {
    fZDCQHist[k] = new TProfile(Form("fZDCQHist[%d]",k),Form("fZDCQHist[%d]",k),100,0.,100.,"s");
    fHistList->Add(fZDCQHist[k]);
  }
  
  // QA histos
  
  for(Int_t r=0;r<fnRun;r++) {
    fQVecListRun[r] = new TList();
    fQVecListRun[r]->SetName(Form("Run %d",fRunList[r]));
    fQVecListRun[r]->SetOwner(kTRUE);
    fHistList->Add(fQVecListRun[r]);
    for(Int_t k=0; k<4; k++) {
      fZDCQVecRaw[r][k] = new TProfile(Form("fZDCQVecRaw[%d][%d]",fRunList[r],k),Form("fZDCQVecRaw[%d][%d]",fRunList[r],k),100,0.,100.,"s");
      fZDCQVecRaw[r][k]->Sumw2();
      fQVecListRun[r]->Add(fZDCQVecRaw[r][k]);
      fZDCQVecRec[r][k] = new TProfile(Form("fZDCQVecRec[%d][%d]",fRunList[r],k),Form("fZDCQVecRec[%d][%d]",fRunList[r],k),100,0.,100.,"s");
      fZDCQVecRec[r][k]->Sumw2();
      fQVecListRun[r]->Add(fZDCQVecRec[r][k]);
      fZDCQVecRecHig[r][k] = new TProfile(Form("fZDCQVecRecHig[%d][%d]",fRunList[r],k),Form("fZDCQVecRecHig[%d][%d]",fRunList[r],k),100,0.,100.,"s");
      fZDCQVecRecHig[r][k]->Sumw2();
      fQVecListRun[r]->Add(fZDCQVecRecHig[r][k]);
    }
    for(Int_t k=0; k<2; k++) {
      fZDCEVPLRec[r][k] = new TH2F(Form("fZDCEVPLRec[%d][%d]",fRunList[r],k),Form("fZDCEVPLRec[%d][%d]",fRunList[r],k),100,0.,100.,1000,-TMath::Pi(),TMath::Pi());
      fZDCEVPLRec[r][k]->Sumw2();
      fQVecListRun[r]->Add(fZDCEVPLRec[r][k]);
    }
  }
  
  fTH2PsiAPsiC = new TH2D("fTH2PsiAPsiC","fTH2PsiAPsiC",100,-TMath::Pi(),TMath::Pi(),100,-TMath::Pi(),TMath::Pi());
  fHistList->Add(fTH2PsiAPsiC);
  
  fAnalysisUtils = new AliAnalysisUtils;
  fAnalysisUtils->SetUseMVPlpSelection(kTRUE);
  fAnalysisUtils->SetUseOutOfBunchPileUp(kTRUE);
  
  //  add  the  list  to  our  output  file
  PostData(1, fOutputList);
  PostData(2, fHistList); //  recall  1 from  constructor
}

//=====================================================================

void AliAnalysisTaskZDCEP::UserExec(Option_t *)
{
  //  get  an  event  from  the  analysis  manager
  AliAODEvent *aod = dynamic_cast<AliAODEvent*>(InputEvent());
  if(!aod) return;
  Float_t Centrality = ((AliVAODHeader*)aod->GetHeader())->GetCentralityP()->GetCentralityPercentile("V0M");
  Int_t RunNum = aod->GetRunNumber();
  Int_t RunBin=-1, bin=0;
  for(Int_t c=0;c<fnRun;c++) {
    if(fRunList[c]==RunNum) RunBin=bin;
    else bin++;
  }
  if(RunBin==-1) return;
  
  // event cuts
  const AliVVertex* pvtx = aod->GetPrimaryVertex();
  Double_t pvtxz = 0.;
  Int_t ncontrib = 0;
  if(pvtx) pvtxz = pvtx->GetZ();
  if(fabs(pvtxz)>10.)  return;
  if(fAnalysisUtils->IsPileUpEvent(InputEvent()))  return;
  
  // zdc selection
  AliAODZDC *aodZDC = aod->GetZDCData();
  
  const Double_t * towZNC = aodZDC->GetZNCTowerEnergy();
  const Double_t * towZPC = aodZDC->GetZPCTowerEnergy();
  const Double_t * towZNA = aodZDC->GetZNATowerEnergy();
  const Double_t * towZPA = aodZDC->GetZPATowerEnergy();
  
  // Get centroid from ZDCs ********************************************
  
  Double_t xyZNC[2]={999.,999.}, xyZNA[2]={999.,999.};
  Float_t numXZNC=0., numYZNC=0., denZNC=0., wZNC;
  Float_t numXZNA=0., numYZNA=0., denZNA=0., wZNA;
  
  if (fUseMCTune) {
    aodZDC->GetZNCentroidInPbPb(1380., xyZNC, xyZNA);
    for(Int_t i=0; i<4; i++) {
      if(towZNC[i+1]>0.) {
        denZNC += TMath::Power(towZNC[i+1], 0.395);
      }
      if(towZNA[i+1]>0.) {
        denZNA += TMath::Power(towZNA[i+1], 0.395);
      }
    }
  } else {
    const Float_t x[4] = {-1.75, 1.75, -1.75, 1.75};
    const Float_t y[4] = {-1.75, -1.75, 1.75, 1.75};
    for(Int_t i=0; i<4; i++) {
      if(towZNC[i+1]>0.) {
        wZNC = TMath::Power(towZNC[i+1], fZDCGainAlpha);
        numXZNC += x[i]*wZNC;
        numYZNC += y[i]*wZNC;
        denZNC += wZNC;
      }
      if(towZNA[i+1]>0.) {
        wZNA = TMath::Power(towZNA[i+1], fZDCGainAlpha);
        numXZNA += x[i]*wZNA;
        numYZNA += y[i]*wZNA;
        denZNA += wZNA;
      }
    }
    if(denZNC!=0) {
      xyZNC[0] = numXZNC/denZNC;
      xyZNC[1] = numYZNC/denZNC;
    }
    if(denZNA!=0) {
      xyZNA[0] = numXZNA/denZNA;
      xyZNA[1] = numYZNA/denZNA;
    }
  }
  
  fZDCFlowVect[0]->Set(xyZNC[0],xyZNC[1]);
  fZDCFlowVect[1]->Set(xyZNA[0],xyZNA[1]);
  fZDCFlowVect[0]->SetMult(denZNC);
  fZDCFlowVect[1]->SetMult(denZNA);
  
  Double_t QCRe = fZDCFlowVect[0]->X();
  Double_t QCIm = fZDCFlowVect[0]->Y();
  Double_t QARe = fZDCFlowVect[1]->X();
  Double_t QAIm = fZDCFlowVect[1]->Y();
  
  if(denZNC<=0.||denZNA<=0.) return;
  if(sqrt(QCRe*QCRe+QCIm*QCIm)>1.5 || sqrt(QARe*QARe+QAIm*QAIm)>1.5) return;
  
  fZDCQVecRaw[RunBin][0]->Fill(Centrality,QCRe);
  fZDCQVecRaw[RunBin][1]->Fill(Centrality,QCIm);
  fZDCQVecRaw[RunBin][2]->Fill(Centrality,QARe);
  fZDCQVecRaw[RunBin][3]->Fill(Centrality,QAIm);
  
  if(fZDCCalibList) {
    
    // get histos of run
    
    if(RunNum!=fCachedRunNum) {
      fZDCQHist[0] = (TProfile*)(fZDCCalibList->FindObject(Form("Run %d",RunNum))->FindObject(Form("fCRCZDCQVecC[%d][%d]",RunNum,0)));
      fZDCQHist[1] = (TProfile*)(fZDCCalibList->FindObject(Form("Run %d",RunNum))->FindObject(Form("fCRCZDCQVecC[%d][%d]",RunNum,1)));
      fZDCQHist[2] = (TProfile*)(fZDCCalibList->FindObject(Form("Run %d",RunNum))->FindObject(Form("fCRCZDCQVecA[%d][%d]",RunNum,0)));
      fZDCQHist[3] = (TProfile*)(fZDCCalibList->FindObject(Form("Run %d",RunNum))->FindObject(Form("fCRCZDCQVecA[%d][%d]",RunNum,1)));
      fZDCQHist[4] = (TProfile*)(fCRCZDCCalibList->FindObject(Form("Run %d",fRunNum))->FindObject(Form("fCRCZDCcos2C[%d]",fRunNum)));
      fZDCQHist[5] = (TProfile*)(fCRCZDCCalibList->FindObject(Form("Run %d",fRunNum))->FindObject(Form("fCRCZDCsin2C[%d]",fRunNum)));
      fZDCQHist[6] = (TProfile*)(fCRCZDCCalibList->FindObject(Form("Run %d",fRunNum))->FindObject(Form("fCRCZDCcos2A[%d]",fRunNum)));
      fZDCQHist[7] = (TProfile*)(fCRCZDCCalibList->FindObject(Form("Run %d",fRunNum))->FindObject(Form("fCRCZDCsin2A[%d]",fRunNum)));
    }
    
    // recentering
    
    if (!fZDCQHist[0] || !fZDCQHist[1] || !fZDCQHist[2] || !fZDCQHist[3]) return;
    
    Double_t AvQCRe = fZDCQHist[0]->GetBinContent(fZDCQHist[0]->FindBin(Centrality));
    Double_t AvQCIm = fZDCQHist[1]->GetBinContent(fZDCQHist[1]->FindBin(Centrality));
    
    Double_t QCReM = QCRe-AvQCRe;
    Double_t QCImM = QCIm-AvQCIm;
    
    fZDCFlowVect[0]->Set(QCReM,QCImM);
    
    Double_t AvQARe = fZDCQHist[2]->GetBinContent(fZDCQHist[2]->FindBin(Centrality));
    Double_t AvQAIm = fZDCQHist[3]->GetBinContent(fZDCQHist[3]->FindBin(Centrality));
    
    Double_t QAReM = QARe-AvQARe;
    Double_t QAImM = QAIm-AvQAIm;
    
    fZDCFlowVect[1]->Set(QAReM,QAImM);
    
    // flattening
    
    if (!fZDCQHist[4] || !fZDCQHist[5] || !fZDCQHist[6] || !fZDCQHist[7]) return;
    
    Double_t AvCos2C = fZDCFitSec[0]->Eval(fCentralityEBE);
    Double_t AvSin2C = fZDCFitSec[1]->Eval(fCentralityEBE);
    
    fEvPlDPsiC = -AvSin2C*cos(2.*EvPlZDCC) + AvCos2C*sin(2.*EvPlZDCC);
    EvPlZDCC += fEvPlDPsiC;
    fEvPlZDCCflat = EvPlZDCC;
    if(EvPlZDCC<-TMath::Pi()) EvPlZDCC += TMath::TwoPi();
    if(EvPlZDCC> TMath::Pi()) EvPlZDCC -= TMath::TwoPi();
    
    fCRCZDCEP[3]->Fill(fRunBin+0.5,fCentralityEBE,EvPlZDCC);
    
    Double_t AvCos2A = fZDCFitSec[2]->Eval(fCentralityEBE);
    Double_t AvSin2A = fZDCFitSec[3]->Eval(fCentralityEBE);
    
    fEvPlDPsiA = -AvSin2A*cos(2.*EvPlZDCA) + AvCos2A*sin(2.*EvPlZDCA);
    EvPlZDCA += fEvPlDPsiA;
    fEvPlZDCAflat = EvPlZDCA;
    
    fCRCZDCEP[4]->Fill(fRunBin+0.5,fCentralityEBE,EvPlZDCA);
    
    EvPlZDCsum = fEvPlZDCCflat+fEvPlZDCAflat;
    if(EvPlZDCsum<-TMath::Pi()) EvPlZDCsum += TMath::TwoPi();
    if(EvPlZDCsum> TMath::Pi()) EvPlZDCsum -= TMath::TwoPi();
    fCRCZDCEP[5]->Fill(fRunBin+0.5,fCentralityEBE,EvPlZDCsum);
    
    fCRCZDCQ2[4]->Fill(fRunBin+0.5,fCentralityEBE,cos(2.*fEvPlZDCCflat));
    fCRCZDCQ2[5]->Fill(fRunBin+0.5,fCentralityEBE,sin(2.*fEvPlZDCCflat));
    fCRCZDCQ2[6]->Fill(fRunBin+0.5,fCentralityEBE,cos(2.*fEvPlZDCAflat));
    fCRCZDCQ2[7]->Fill(fRunBin+0.5,fCentralityEBE,sin(2.*fEvPlZDCAflat));
    
    // save
    
    fZDCQVecRec[RunBin][0]->Fill(Centrality,QCReM);
    fZDCQVecRec[RunBin][1]->Fill(Centrality,QCImM);
    fZDCQVecRec[RunBin][2]->Fill(Centrality,QAReM);
    fZDCQVecRec[RunBin][3]->Fill(Centrality,QAImM);
    
    fZDCQVecRecHig[RunBin][0]->Fill(Centrality,QCReM*QCReM-QCImM*QCImM);
    fZDCQVecRecHig[RunBin][1]->Fill(Centrality,2.*QCReM*QCImM);
    fZDCQVecRecHig[RunBin][2]->Fill(Centrality,QAReM*QAReM-QAImM*QAImM);
    fZDCQVecRecHig[RunBin][3]->Fill(Centrality,2.*QAReM*QAImM);
    
    fEvPlZDCAflat = EvPlZDCA;
    if(EvPlZDCA<-TMath::Pi()) EvPlZDCA += TMath::TwoPi();
    if(EvPlZDCA> TMath::Pi()) EvPlZDCA -= TMath::TwoPi();
    fZDCEVPLRec[RunBin][0]->Fill(Centrality,TMath::ATan2(QCImM,QCReM));
    
    fEvPlZDCAflat = EvPlZDCA;
    if(EvPlZDCA<-TMath::Pi()) EvPlZDCA += TMath::TwoPi();
    if(EvPlZDCA> TMath::Pi()) EvPlZDCA -= TMath::TwoPi();
    fZDCEVPLRec[RunBin][1]->Fill(Centrality,TMath::ATan2(QAImM,QAReM));
    
  } else {
    printf(" WARNING: no weights provided for ZDC recentering !!! \n");
  }
  
  // save run number
  fCachedRunNum = RunNum;
  
  PostData(1, fOutputList);
  PostData(2, fHistList);
}

//=====================================================================

void AliAnalysisTaskZDCEP::Terminate(Option_t */*option*/)
{
}


