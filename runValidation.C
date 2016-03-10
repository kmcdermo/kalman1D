void setupcpp11(){ // customize ACLiC's behavior ...
  TString o;
  // customize MakeSharedLib
  o = TString(gSystem->GetMakeSharedLib());
  o = o.ReplaceAll(" -c ", " -std=c++0x -c ");
  gSystem->SetMakeSharedLib(o.Data());
  // customize MakeExe
  o = TString(gSystem->GetMakeExe());
  o = o.ReplaceAll(" -c ", " -std=c++0x -c ");
  gSystem->SetMakeExe(o.Data());
} 

void runValidation() {
  setupcpp11(); //  use this to get PlotValidation to compile ... phiphi ROOT build has ACLiC with C++98!
  gROOT->LoadMacro("PlotValidation.cpp++g");//compile the code

  PlotValidation Val("validation.root");
  Val.Validation();
}
