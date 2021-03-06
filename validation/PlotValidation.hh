#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>

class PlotValidation 
{
public :
  TFile * file;
  TTree * fChain;   //!pointer to the analyzed TTree or TChain
  Int_t   fCurrent; //!current Tree number in a TChain

  // Declaration of leaf types
  Int_t   evtID;
  Int_t   trackID;
  Int_t   layer;
  Float_t x_mc;
  Float_t vx_mc;
  Float_t x_filter;
  Float_t vx_filter;
  Float_t x_smooth;
  Float_t vx_smooth;
  Float_t x_hit;
  Float_t exx_filter;
  Float_t evxvx_filter;
  Float_t exvx_filter;
  Float_t evxx_filter;
  Float_t exx_smooth;
  Float_t evxvx_smooth;
  Float_t exvx_smooth;
  Float_t evxx_smooth;
  Float_t exx_hit;
  Float_t chi2;

  // List of branches
  TBranch * b_evtID;
  TBranch * b_trackID;
  TBranch * b_layer;
  TBranch * b_x_mc;
  TBranch * b_vx_mc;
  TBranch * b_x_filter;
  TBranch * b_vx_filter;
  TBranch * b_x_smooth;
  TBranch * b_vx_smooth;
  TBranch * b_x_hit;
  TBranch * b_exx_filter;
  TBranch * b_evxvx_filter;
  TBranch * b_exvx_filter;
  TBranch * b_evxx_filter;
  TBranch * b_exx_smooth;
  TBranch * b_evxvx_smooth;
  TBranch * b_exvx_smooth;
  TBranch * b_evxx_smooth;
  TBranch * b_exx_hit;
  TBranch * b_chi2;

  PlotValidation(TString filename, TString outdir, Bool_t mvinput);
  ~PlotValidation();
  Int_t    GetEntry(Long64_t entry);
  Long64_t LoadTree(Long64_t entry);
  void     Init(TTree *tree);
  void     Validation();
  void     MoveInput();

private:
  TString inFileName;
  TString outDir;
  Bool_t  mvInput;
};
