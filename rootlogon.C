{ 

  TString rfitpath("$ROOFITSYS/include");
  TString path = gSystem->GetIncludePath();
  path += "-I. -I$ROOTSYS/src -I";
  path += rfitpath;
  gSystem->SetIncludePath(path.Data());

  gROOT->Macro("$CMSSW_BASE/src/TagAndProbeAnalysis/FitModels/RooVoigtianShape.cc+");
  gROOT->Macro("$CMSSW_BASE/src//TagAndProbeAnalysis/FitModels/RooErf.cc+");
  gROOT->Macro("$CMSSW_BASE/src//TagAndProbeAnalysis/FitModels/RooCMSShape.cc+");
  gROOT->Macro("$CMSSW_BASE/src/TagAndProbeAnalysis/Utils/CPlot.cc+");
  gROOT->Macro("$CMSSW_BASE/src/TagAndProbeAnalysis/Utils/PlotStyle.cc+"); 
  gROOT->Macro("$CMSSW_BASE/src/TagAndProbeAnalysis/Utils/CEffUser1D.cc+");
  gROOT->Macro("$CMSSW_BASE/src/TagAndProbeAnalysis/Utils/CEffUser2D.cc+"); 
  
}
