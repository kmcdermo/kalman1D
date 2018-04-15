#include "validation/PlotValidation.cpp+"
#include "TString.h"

void runValidation(const TString & infile = "valtree.root", const TString & outdir = "output", const Bool_t moveInput = true) 
{
  // First argument is input name of root file
  // Second argument is output name of directory/rootfile/file plots
  // Third argument boolean argument == true to move input root file to output directory, false to keep input file where it is.

  PlotValidation Val(infile,outdir,moveInput);
  Val.Validation();
}
