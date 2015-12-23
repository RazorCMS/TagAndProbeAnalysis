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


void computeElectronEfficiency( int option = 1) {

  //*************************************************************************************
  //Reco -> Tight Selection
  //*************************************************************************************
  if (option == 1) {

    //*************************************
    //Monte Carlo
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/ElectronTightSelectionDenominatorReco.cfg",
    			0,0,0,0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/TightElectronSelectionEffDenominatorReco/TagAndProbe_TightElectronSelectionEffDenominatorReco_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/TightElectronSelectionEffDenominatorReco/MC_25ns_2015Final_Golden/",
    			"png",
    			true,
    			0,
    			"",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight_Spring15MCTo2015Data.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/ElectronTightSelectionDenominatorReco.cfg",
    			//2,1,2,3,
    			2,1,2,2,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/TightElectronSelectionEffDenominatorReco/TagAndProbe_TightElectronSelectionEffDenominatorReco_SingleElectron_Run2015Final_GoodLumiGolden.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/TightElectronSelectionEffDenominatorReco/Data_2015Final_Golden/",
    			"png",
    			true,
    			0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/TightElectronSelectionEffDenominatorReco/TagAndProbe_TightElectronSelectionEffDenominatorReco_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight_Spring15MCTo2015Data.root");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/TightElectronSelectionEffDenominatorReco/Data_2015Final_Golden/eff.root",
			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/TightElectronSelectionEffDenominatorReco/MC_25ns_2015Final_Golden/eff.root",
			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/TightElectronSelectionEffDenominatorReco/Data_2015Final_Golden/",
			       "ScaleFactor_TightElectronSelectionEffDenominatorReco",
			       "TightElectronSelectionEffDenominatorReco_2015Final_Golden",
			       20,200,0.5,1.15
			       );    
  }



  //*************************************************************************************
  //Reco -> Loose Selection
  //*************************************************************************************
  if (option == 2) {

    //*************************************
    //Monte Carlo
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/ElectronLooseSelectionDenominatorReco.cfg",
    			0,0,0,0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/LooseElectronSelectionEffDenominatorReco/TagAndProbe_LooseElectronSelectionEffDenominatorReco_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/LooseElectronSelectionEffDenominatorReco/MC_25ns/",
    			"png",
    			true,
    			0,
    			"",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight_Spring15MCTo2015Data.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/ElectronLooseSelectionDenominatorReco.cfg",
    			2,1,2,3,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/LooseElectronSelectionEffDenominatorReco/TagAndProbe_LooseElectronSelectionEffDenominatorReco_SingleElectron_Run2015C_GoodLumi.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/LooseElectronSelectionEffDenominatorReco/Data/2015C/",
    			"png",
    			true,
    			0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/LooseElectronSelectionEffDenominatorReco/TagAndProbe_LooseElectronSelectionEffDenominatorReco_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight_Spring15MCTo2015Data.root");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/LooseElectronSelectionEffDenominatorReco/Data/2015C/eff.root",
			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/LooseElectronSelectionEffDenominatorReco/MC_25ns/eff.root",
			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/LooseElectronSelectionEffDenominatorReco/Data/",
			       "ScaleFactor_LooseElectronSelectionEffDenominatorReco",
			       "LooseElectronSelectionEffDenominatorReco_2015C",
			       20,200,0.5,1.15
			       );
    
  }


  //*************************************************************************************
  //Reco -> Veto Selection
  //*************************************************************************************
  if (option == 3) {

    //*************************************
    //Monte Carlo
    //*************************************
    // MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/ElectronVetoSelectionDenominatorReco.cfg",
    // 			0,0,0,0,
    // 			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/VetoElectronSelectionEffDenominatorReco/TagAndProbe_VetoElectronSelectionEffDenominatorReco_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
    // 			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoElectronSelectionEffDenominatorReco/MC_25ns/",
    // 			"png",
    // 			true,
    // 			0,
    // 			"",
    // 			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight_Spring15MCTo2015Data.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    // MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/ElectronVetoSelectionDenominatorReco.cfg",
    // 			2,1,2,3,
    // 			//2,2,2,2,
    // 			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/VetoElectronSelectionEffDenominatorReco/TagAndProbe_VetoElectronSelectionEffDenominatorReco_SingleElectron_Run2015D_GoodLumiGolden.root",
    // 			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoElectronSelectionEffDenominatorReco/Data/2015Final/",
    // 			"png",
    // 			true,
    // 	 		0,
    // 			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/VetoElectronSelectionEffDenominatorReco/TagAndProbe_VetoElectronSelectionEffDenominatorReco_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
    // 			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight_Spring15MCTo2015Data.root");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoElectronSelectionEffDenominatorReco/Data/2015Final/eff.root",
  			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoElectronSelectionEffDenominatorReco/MC_25ns/eff.root",
  			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoElectronSelectionEffDenominatorReco/Data/",
  			       "ScaleFactor_VetoElectronSelectionEffDenominatorReco",
  			       "VetoElectronSelectionEffDenominatorReco_2015Final",
  			       10,200,0.5,1.15
  			       );
    
   }




  //*************************************************************************************
  //Reco -> Veto ID
  //*************************************************************************************
  if (option == 10) {

    //*************************************
    //Monte Carlo
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/ElectronSingleEleHLTDenominatorTight.cfg",
    			0,0,0,0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/EleTriggerEleCombinedEffDenominatorTight/TagAndProbe_EleTriggerEleCombinedEffDenominatorTight_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/EleTriggerEleCombinedEffDenominatorTight/MC_25ns/",
    			"png",
    			false,
    			0,
    			"",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight_Spring15MCTo2015Data.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/ElectronSingleEleHLTDenominatorTight.cfg",
    			2,1,2,3,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/EleTriggerEleCombinedEffDenominatorTight/TagAndProbe_EleTriggerEleCombinedEffDenominatorTight_SingleElectron_Run2015B_GoodLumi.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/EleTriggerEleCombinedEffDenominatorTight/Data/",
    			"png",
    			false,
    			0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/EleTriggerEleCombinedEffDenominatorTight/TagAndProbe_EleTriggerEleCombinedEffDenominatorTight_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight_Spring15MCTo2015Data.root");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/EleTriggerEleCombinedEffDenominatorTight/Data/eff.root",
			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/EleTriggerEleCombinedEffDenominatorTight/MC_25ns/eff.root",
			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/EleTriggerEleCombinedEffDenominatorTight/Data/",
			       "ScaleFactor_EleTriggerEleCombinedEffDenominatorTight",
			       "EleTriggerEleCombinedEffDenominatorTight_2015B");
    
  }


  //*************************************************************************************
  //Veto ID -> Veto Selection
  //*************************************************************************************
  if (option == 11) {

    // //*************************************
    // //Monte Carlo
    // //*************************************
    // MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/VetoElectronSelectionEffDenominatorVetoID.cfg",
    // 			0,0,0,0,
    // 			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/VetoElectronSelectionEffDenominatorVetoID/TagAndProbe_VetoElectronSelectionEffDenominatorVetoID_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    // 			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoElectronSelectionEffDenominatorVetoID/MC_25ns/",
    // 			"png",
    // 			true,
    // 			0,
    // 			"",
    // 			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/NVtxReweight_ZToMuMu_2015C_25ns.root");
    
    // //*************************************
    // //Data : Do Fit
    // //*************************************
    // MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/ElectronSingleEleHLTDenominatorTight.cfg",
    // 			2,1,2,3,
    // 			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/VetoElectronSelectionEffDenominatorVetoID/TagAndProbe_VetoElectronSelectionEffDenominatorVetoID_SingleElectron_Run2015C.root",
    // 			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoElectronSelectionEffDenominatorVetoID/Data/2015C/",
    // 			"png",
    // 			true,
    // 			0,
    // 			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/VetoElectronSelectionEffDenominatorVetoID/TagAndProbe_VetoElectronSelectionEffDenominatorVetoID_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    // 			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/NVtxReweight_ZToMuMu_2015C_25ns.root");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoElectronSelectionEffDenominatorVetoID/Data/2015C/eff.root",
			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoElectronSelectionEffDenominatorVetoID/MC_25ns/eff.root",
			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoElectronSelectionEffDenominatorVetoID/Data/",
			       "ScaleFactor_VetoElectronSelectionEffDenominatorVetoID",
			       "VetoElectronSelectionEffDenominatorVetoID_2015C");
    
  }



  //*************************************************************************************
  //Tight ID -> Tight Selection
  //*************************************************************************************
  if (option == 13) {

    // //*************************************
    // //Monte Carlo
    // //*************************************
    // MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/TightElectronSelectionEffDenominatorTightID.cfg",
    // 			0,0,0,0,
    // 			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/TightElectronSelectionEffDenominatorTightID/TagAndProbe_TightElectronSelectionEffDenominatorTightID_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    // 			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/TightElectronSelectionEffDenominatorTightID/MC_25ns_2015D/",
    // 			"png",
    // 			true,
    // 			0,
    // 			"",
    // 			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/NVtxReweight_ZToMuMu_2015D_25ns.root");
    
    // //*************************************
    // //Data : Do Fit
    // //*************************************
    // MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/TightElectronSelectionEffDenominatorTightID.cfg",
    // 			2,1,2,3,
    // 			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/TightElectronSelectionEffDenominatorTightID/TagAndProbe_TightElectronSelectionEffDenominatorTightID_SingleElectron_Run2015D_GoodLumi.root",
    // 			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/TightElectronSelectionEffDenominatorTightID/Data/2015D/",
    // 			"png",
    // 			true,
    // 			0,
    // 			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/TightElectronSelectionEffDenominatorTightID/TagAndProbe_TightElectronSelectionEffDenominatorTightID_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    // 			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/NVtxReweight_ZToMuMu_2015D_25ns.root");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/TightElectronSelectionEffDenominatorTightID/Data/2015D/eff.root",
			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/TightElectronSelectionEffDenominatorTightID/MC_25ns_2015D/eff.root",
			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/TightElectronSelectionEffDenominatorTightID/Data/",
			       "ScaleFactor_TightElectronSelectionEffDenominatorTightID",
			       "TightElectronSelectionEffDenominatorTightID_2015D");
    
  }


}
