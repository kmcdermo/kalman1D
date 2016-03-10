//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Mar 10 16:09:14 2016 by ROOT version 5.34/30
// from TTree tree/tree
// found on file: validation.root
//////////////////////////////////////////////////////////

#ifndef PlotValidation_h
#define PlotValidation_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class PlotValidation {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           evtID;
   Int_t           trackID;
   Int_t           layer;
   Float_t         x_mc;
   Float_t         vx_mc;
   Float_t         x_up;
   Float_t         vx_up;
   Float_t         x_hit;
   Float_t         exx_up;
   Float_t         evxvx_up;
   Float_t         exvx_up;
   Float_t         evxx_up;
   Float_t         exx_hit;
   Float_t         chi2;

   // List of branches
   TBranch        *b_evtID;   //!
   TBranch        *b_trackID;   //!
   TBranch        *b_layer;   //!
   TBranch        *b_x_mc;   //!
   TBranch        *b_vx_mc;   //!
   TBranch        *b_x_up;   //!
   TBranch        *b_vx_up;   //!
   TBranch        *b_x_hit;   //!
   TBranch        *b_exx_up;   //!
   TBranch        *b_evxvx_up;   //!
   TBranch        *b_exvx_up;   //!
   TBranch        *b_evxx_up;   //!
   TBranch        *b_exx_hit;   //!
   TBranch        *b_chi2;   //!

   PlotValidation(TTree *tree=0);
   virtual ~PlotValidation();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef PlotValidation_cxx
PlotValidation::PlotValidation(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("validation.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("validation.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

PlotValidation::~PlotValidation()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t PlotValidation::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Long64_t PlotValidation::LoadTree(Long64_t entry){
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
   }
   return centry;
}

void PlotValidation::Init(TTree *tree){
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("evtID", &evtID, &b_evtID);
   fChain->SetBranchAddress("trackID", &trackID, &b_trackID);
   fChain->SetBranchAddress("layer", &layer, &b_layer);
   fChain->SetBranchAddress("x_mc", &x_mc, &b_x_mc);
   fChain->SetBranchAddress("vx_mc", &vx_mc, &b_vx_mc);
   fChain->SetBranchAddress("x_up", &x_up, &b_x_up);
   fChain->SetBranchAddress("vx_up", &vx_up, &b_vx_up);
   fChain->SetBranchAddress("x_hit", &x_hit, &b_x_hit);
   fChain->SetBranchAddress("exx_up", &exx_up, &b_exx_up);
   fChain->SetBranchAddress("evxvx_up", &evxvx_up, &b_evxvx_up);
   fChain->SetBranchAddress("exvx_up", &exvx_up, &b_exvx_up);
   fChain->SetBranchAddress("evxx_up", &evxx_up, &b_evxx_up);
   fChain->SetBranchAddress("exx_hit", &exx_hit, &b_exx_hit);
   fChain->SetBranchAddress("chi2", &chi2, &b_chi2);
}

#endif // #ifdef PlotValidation_cxx
