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


void computePhotonTriggerEfficiency( int option = 0) {

  //*************************************************************************************
  //Trailing Leg HLT Efficiency 2016
  //*************************************************************************************
  if (option == 0) {
 
    // *************************************
    // Monte Carlo
    // *************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/PhotonHLTDenominatorLoose.cfg",
    			0,0,0,0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/PhoHLTTrailingLegEffDenominatorLoose/2016/TagAndProbe_PhoHLTTrailingLegEffDenominatorLoose_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTTrailingLegEffDenominatorLoose/MC_2016_Rereco/",
    			"png",
    			false,
    			0,
    			"",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupWeights/PileupReweight_2016_36p2ifb.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/PhotonHLTDenominatorLoose.cfg",
    			2,1,2,3,
    			//0,0,0,0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/PhoHLTTrailingLegEffDenominatorLoose/2016/TagAndProbe_PhoHLTTrailingLegEffDenominatorLoose_SingleElectron_2016_GoodLumiGolden_26p4ifb.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTTrailingLegEffDenominatorLoose/Data_2016_Rereco/",
    			"png",
    			false,
    			0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/PhoHLTTrailingLegEffDenominatorLoose/2016/TagAndProbe_PhoHLTTrailingLegEffDenominatorLoose_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupWeights/PileupReweight_2016_36p2ifb.root");

    // *************************************
    // Make Scale Factor Tables
    // *************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTTrailingLegEffDenominatorLoose/Data_2016_Rereco/eff.root",
    			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTTrailingLegEffDenominatorLoose/MC_2016_Rereco/eff.root",
    			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTTrailingLegEffDenominatorLoose/Data_2016_Rereco/",
    			       "ScaleFactor_PhoHLTTrailingLegEffDenominatorLoose",
    			       "PhoHLTTrailingLegEffDenominatorLoose_2016_Rereco",
			       0,80,
			       0.0, 1.15
			       );
 
    makeEfficiencyPlot("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTTrailingLegEffDenominatorLoose/Data_2016_Rereco/eff.root",
    			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTTrailingLegEffDenominatorLoose/Data_2016_Rereco/",
    			       "ScaleFactor_PhoHLTTrailingLegEffDenominatorLoose",
    			       "PhoHLTTrailingLegEffDenominatorLoose", 20, 150, 0.0, 1.15);
 
  }

  *************************************************************************************
  Leading Leg HLT Efficiency 2016
  *************************************************************************************
  if (option == 1) {
 
    //*************************************
    //Monte Carlo
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/PhotonHLTDenominatorLoose.cfg",
    			0,0,0,0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/PhoHLTLeadingLegEffDenominatorLoose/2016/TagAndProbe_PhoHLTLeadingLegEffDenominatorLoose_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTLeadingLegEffDenominatorLoose/MC_2016_Rereco/",
    			"png",
    			false,
    			0,
    			"",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupWeights/PileupReweight_2016_36p2ifb.root");
    
    // //*************************************
    // //Data : Do Fit
    // //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/PhotonHLTDenominatorLoose.cfg",
    			2,1,2,3,
    			//0,0,0,0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/PhoHLTLeadingLegEffDenominatorLoose/2016/TagAndProbe_PhoHLTLeadingLegEffDenominatorLoose_SingleElectron_2016_GoodLumiGolden_26p4ifb.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTLeadingLegEffDenominatorLoose/Data_2016_Rereco/",
    			"png",
    			false,
    			0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/PhoHLTLeadingLegEffDenominatorLoose/2016/TagAndProbe_PhoHLTLeadingLegEffDenominatorLoose_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupWeights/PileupReweight_2016_36p2ifb.root");

    // *************************************
    // Make Scale Factor Tables
    // *************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTLeadingLegEffDenominatorLoose/Data_2016_Rereco/eff.root",
    			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTLeadingLegEffDenominatorLoose/MC_2016_Rereco/eff.root",
    			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTLeadingLegEffDenominatorLoose/Data_2016_Rereco/",
    			       "ScaleFactor_PhoHLTLeadingLegEffDenominatorLoose",
    			       "PhoHLTLeadingLegEffDenominatorLoose_2016_Rereco",
			       0,80,
			       0.0, 1.15
			       );
 
   makeEfficiencyPlot("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTLeadingLegEffDenominatorLoose/Data_2016_Rereco/eff.root",
    			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTLeadingLegEffDenominatorLoose/Data_2016_Rereco/",
    			       "ScaleFactor_PhoHLTLeadingLegEffDenominatorLoose",
    			       "PhoHLTLeadingLegEffDenominatorLoose", 20, 150, 0.0, 1.15);
    
  }

    









  //*************************************************************************************
  //Trailing Leg HLT Efficiency 2015
  //*************************************************************************************
  if (option == 10) {
 
    //*************************************
    //Monte Carlo
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/PhotonHLTDenominatorLoose.cfg",
    			0,0,0,0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/PhoHLTTrailingLegEffDenominatorLoose/2015/TagAndProbe_PhoHLTTrailingLegEffDenominatorLoose_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTTrailingLegEffDenominatorLoose/MC/2015/",
    			"png",
    			false,
    			0,
    			"",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight2015_7_6.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/PhotonHLTDenominatorLoose.cfg",
    			// 2,1,2,3,
    			1,1,1,3,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/PhoHLTTrailingLegEffDenominatorLoose/2015/TagAndProbe_PhoHLTTrailingLegEffDenominatorLoose_SingleElectron_2015D_GoodLumiGolden.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTTrailingLegEffDenominatorLoose/Data/2015/",
    			"png",
    			false,
    			0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/PhoHLTTrailingLegEffDenominatorLoose/2015/TagAndProbe_PhoHLTTrailingLegEffDenominatorLoose_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight2015_7_6.root");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    // makeEfficiencyPlot("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTTrailingLegEffDenominatorLoose/Data/2015/eff.root",
    // 			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTTrailingLegEffDenominatorLoose/Data/2015/",
    // 			       "ScaleFactor_PhoHLTTrailingLegEffDenominatorLoose",
    // 			       "PhoHLTTrailingLegEffDenominatorLoose", 20, 150, 0.0, 1.15);
 
    makeEfficiencyScaleFactors("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTTrailingLegEffDenominatorLoose/Data/2015/eff.root",
    			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTTrailingLegEffDenominatorLoose/MC/2015/eff.root",
    			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTTrailingLegEffDenominatorLoose/Data/2015/",
    			       "ScaleFactor_PhoHLTTrailingLegEffDenominatorLoose",
    			       "PhoHLTTrailingLegEffDenominatorLoose",
			       25,100,
			       0.5, 1.15
			       );
    
  

  }

  //*************************************************************************************
  //Leading Leg HLT Efficiency 2015
  //*************************************************************************************
  if (option == 11) {
 
    //*************************************
    //Monte Carlo
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/PhotonHLTDenominatorLoose.cfg",
    			0,0,0,0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/PhoHLTLeadingLegEffDenominatorLoose/2015/TagAndProbe_PhoHLTLeadingLegEffDenominatorLoose_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTLeadingLegEffDenominatorLoose/MC/2015/",
    			"png",
    			false,
    			0,
    			"",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight2015_7_6.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/PhotonHLTDenominatorLoose.cfg",
			//2,1,2,3,
			1,1,1,3,    			
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/PhoHLTLeadingLegEffDenominatorLoose/2015/TagAndProbe_PhoHLTLeadingLegEffDenominatorLoose_SingleElectron_2015D_GoodLumiGolden.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTLeadingLegEffDenominatorLoose/Data/2015/",
    			"png",
    			false,
    			0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/PhoHLTLeadingLegEffDenominatorLoose/2015/TagAndProbe_PhoHLTLeadingLegEffDenominatorLoose_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight2015_7_6.root");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    // makeEfficiencyPlot("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTLeadingLegEffDenominatorLoose/Data/2015/eff.root",
    // 			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTLeadingLegEffDenominatorLoose/Data/2015/",
    // 			       "ScaleFactor_PhoHLTLeadingLegEffDenominatorLoose",
    // 			       "PhoHLTLeadingLegEffDenominatorLoose", 20, 150, 0.0, 1.15);
    

   makeEfficiencyScaleFactors("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTLeadingLegEffDenominatorLoose/Data/2015/eff.root",
    			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTLeadingLegEffDenominatorLoose/MC/2015/eff.root",
    			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoHLTLeadingLegEffDenominatorLoose/Data/2015/",
    			       "ScaleFactor_PhoHLTLeadingLegEffDenominatorLoose",
    			       "PhoHLTLeadingLegEffDenominatorLoose",
			       25,100,
			       0.5, 1.15
			       );
    
  

  }

    




}
