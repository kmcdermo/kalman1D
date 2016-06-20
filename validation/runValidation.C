void setupcpp11(){ // customize ACLiC's behavior ...
  TString o;
  // customize MakeSharedLib
  o = TString(gSystem->GetMakeSharedLib());
  o = o.ReplaceAll(" -c ", " -std=c++11 -c ");
  gSystem->SetMakeSharedLib(o.Data());
  // customize MakeExe
  o = TString(gSystem->GetMakeExe());
  o = o.ReplaceAll(" -c ", " -std=c++11 -c ");
  gSystem->SetMakeExe(o.Data());
} 

void runValidation() {
  setupcpp11(); //  use this to get PlotValidation to compile with c++11
  gROOT->LoadMacro("PlotValidation.cpp++g");//compile the code

  // First argument is input name of root file
  // Second argument is output name of directory/rootfile/file plots
  // Third argument boolean argument == true to move input root file to output directory, false to keep input file where it is.

  PlotValidation Val("valtree.root","output",true);
  Val.Validation();
}
