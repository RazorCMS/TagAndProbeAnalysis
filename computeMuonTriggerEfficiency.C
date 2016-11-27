//================================================================================================
//
// Do Efficiency Calculations
//
//________________________________________________________________________________________________

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>                  // access to gROOT, entry point to ROOT system
#include <TSystem.h>                // interface to OS
#include <TStyle.h>                 // class to handle ROOT plotting style
#include <TFile.h>                  // file handle class
#include <TTree.h>                  // class to access ntuples
#include <TCanvas.h>                // class for drawing
#include <TH1D.h>                   // 1D histograms
#include <TH2D.h>                   // 2D histograms
#include <TBenchmark.h>             // class to track macro running statistics
#include <TEfficiency.h>            // class to handle efficiency calculations
#include <vector>                   // STL vector class
#include <iostream>                 // standard I/O
#include <iomanip>                  // functions to format standard I/O
#include <fstream>                  // functions for file I/O
#include <string>                   // C++ string class
#include <sstream>                  // class for parsing strings

#include "TagAndProbeAnalysis/Utils/ScaleFactors.hh"
#include "TagAndProbeAnalysis/Utils/EfficiencyTools.hh"

#endif


void computeMuonTriggerEfficiency( int option = 1) {

  //*************************************************************************************
  //Tight -> HLT (Single Muon)
  //*************************************************************************************
  if (option == 1) {

    //*************************************
    //Monte Carlo
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/MuonSingleMuHLTDenominatorTight.cfg",
    			0,0,0,0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/MuTriggerIsoMu27ORMu50EffDenominatorTight/TagAndProbe_MuTriggerIsoMu20ORMu50EffDenominatorTight_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/MuTriggerMuCombinedEffDenominatorTight/MC_2016_Rereco/",
    			"png",
    			true,
    			0,
    			"",
    			"/afs/cern.ch/user/s/sixie/CMSSW_run2/src/RazorAnalyzer/data/PileupWeights/PileupReweight_2016_36p2ifb.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/MuonSingleMuHLTDenominatorTight.cfg",
    			//1,1,1,3,
    			//2,1,2,2,
    			2,1,2,3,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/MuTriggerIsoMu27ORMu50EffDenominatorTight/TagAndProbe_MuTriggerIsoMu20ORMu50EffDenominatorTight_SingleMuon_2016G_PRv1_GoodLumiGolden_SUSYUnblind.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/MuTriggerMuCombinedEffDenominatorTight/Data_2016_Rereco/",
    			"png",
    			true,
    			0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/MuTriggerIsoMu27ORMu50EffDenominatorTight/TagAndProbe_MuTriggerIsoMu20ORMu50EffDenominatorTight_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/user/s/sixie/CMSSW_run2/src/RazorAnalyzer/data/PileupWeights/PileupReweight_2016_36p2ifb.root");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/MuTriggerMuCombinedEffDenominatorTight/Data_2016_Rereco/eff.root",
    			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/MuTriggerMuCombinedEffDenominatorTight/MC_2016_Rereco/eff.root",
    			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/MuTriggerMuCombinedEffDenominatorTight/Data_2016_Rereco/",
    			       "ScaleFactor_MuTriggerIsoMu27ORMu50EffDenominatorTight",
    			       "MuTriggerIsoMu27ORMu50EffDenominatorTight_2016_Rereco_Golden",
    			       20,200,0.0,1.3
    			       );
 
     //*************************************
    //Make Efficiency File
    //*************************************
    makeEfficiencyPlot("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/MuTriggerMuCombinedEffDenominatorTight/Data_2016_Rereco/eff.root",
    			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/MuTriggerMuCombinedEffDenominatorTight/Data_2016_Rereco/",
    			       "ScaleFactor_MuTriggerIsoMu27ORMu50EffDenominatorTight",
    			       "MuTriggerIsoMu27ORMu50EffDenominatorTight_2016_Rereco_Golden",
    			       20,200,0.0,1.3
    			       );
  
  }




}
