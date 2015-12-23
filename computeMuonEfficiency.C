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


void computeMuonEfficiency( int option = 1) {



  //*************************************************************************************
  //Reco -> Tight Selection
  //*************************************************************************************
  if (option == 1) {

    //*************************************
    //Monte Carlo
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/MuonTightSelectionDenominatorReco.cfg",
    			0,0,0,0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/TightMuonSelectionEffDenominatorReco/TagAndProbe_TightMuonSelectionEffDenominatorReco_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/TightMuonSelectionEffDenominatorReco/MC_25ns_2015Final_Golden/",
    			"png",
    			true,
    			0,
    			"",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight_Spring15MCTo2015Data.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/MuonTightSelectionDenominatorReco.cfg",
    			2,1,2,3, 
    			//2,2,2,2,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/TightMuonSelectionEffDenominatorReco/TagAndProbe_TightMuonSelectionEffDenominatorReco_SingleMuon_Run2Final_GoodLumiGolden.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/TightMuonSelectionEffDenominatorReco/Data/2015Final_Golden/",
    			"png",
    			true,
    			0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/TightMuonSelectionEffDenominatorReco/TagAndProbe_TightMuonSelectionEffDenominatorReco_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight_Spring15MCTo2015Data.root");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/TightMuonSelectionEffDenominatorReco/Data/2015Final_Golden/eff.root",
    			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/TightMuonSelectionEffDenominatorReco/MC_25ns_2015Final_Golden/eff.root",
    			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/TightMuonSelectionEffDenominatorReco/Data/",
    			       "ScaleFactor_TightMuonSelectionEffDenominatorReco",
    			       "TightMuonSelectionEffDenominatorReco_2015Final_Golden",
    			       20,200,0.8,1.15
    			       );
    
  }



  //*************************************************************************************
  //Reco -> Loose Selection
  //*************************************************************************************
  if (option == 2) {

    //*************************************
    //Monte Carlo
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/MuonLooseSelectionDenominatorReco.cfg",
    			0,0,0,0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/LooseMuonSelectionEffDenominatorReco/TagAndProbe_LooseMuonSelectionEffDenominatorReco_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/LooseMuonSelectionEffDenominatorReco/MC_25ns/",
    			"png",
    			true,
    			0,
    			"",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight_Spring15MCTo2015Data.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/MuonLooseSelectionDenominatorReco.cfg",
    			2,1,2,3,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/LooseMuonSelectionEffDenominatorReco/TagAndProbe_LooseMuonSelectionEffDenominatorReco_SingleMuon_Run2015C_GoodLumi.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/LooseMuonSelectionEffDenominatorReco/Data/2015C/",
    			"png",
    			true,
    			0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/LooseMuonSelectionEffDenominatorReco/TagAndProbe_LooseMuonSelectionEffDenominatorReco_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight_Spring15MCTo2015Data.root");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/LooseMuonSelectionEffDenominatorReco/Data/2015C/eff.root",
			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/LooseMuonSelectionEffDenominatorReco/MC_25ns/eff.root",
			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/LooseMuonSelectionEffDenominatorReco/Data/",
			       "ScaleFactor_LooseMuonSelectionEffDenominatorReco",
			       "LooseMuonSelectionEffDenominatorReco_2015C",
			       20,200,0.8,1.15
			       );
    
  }


  //*************************************************************************************
  //Reco -> Veto Selection
  //*************************************************************************************
  if (option == 3) {

    //*************************************
    //Monte Carlo
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/MuonVetoSelectionDenominatorReco.cfg",
    			0,0,0,0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/VetoMuonSelectionEffDenominatorReco/TagAndProbe_VetoMuonSelectionEffDenominatorReco_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoMuonSelectionEffDenominatorReco/MC_25ns/",
    			"png",
    			true,
    			0,
    			"",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight_Spring15MCTo2015Data.root");

    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/MuonVetoSelectionDenominatorReco.cfg",
    			2,1,2,2,
			//2,1,2,3,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/VetoMuonSelectionEffDenominatorReco/TagAndProbe_VetoMuonSelectionEffDenominatorReco_SingleMuon_Run2015D_GoodLumiGolden.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoMuonSelectionEffDenominatorReco/Data/2015D/",
    			"png",
    			true,
    			0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/VetoMuonSelectionEffDenominatorReco/TagAndProbe_VetoMuonSelectionEffDenominatorReco_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight_Spring15MCTo2015Data.root");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoMuonSelectionEffDenominatorReco/Data/2015D/eff.root",
			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoMuonSelectionEffDenominatorReco/MC_25ns/eff.root",
			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoMuonSelectionEffDenominatorReco/Data/",
			       "ScaleFactor_VetoMuonSelectionEffDenominatorReco",
			       "VetoMuonSelectionEffDenominatorReco_2015D",
			       10,200,0.8,1.15
			       );
    
  }
















  //*************************************************************************************
  //Reco -> ID
  //*************************************************************************************
  if (option == 10) {

    //*************************************
    //Monte Carlo
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/VetoMuonIDEffDenominatorReco.cfg",
    			0,0,0,0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/VetoMuonIDEffDenominatorReco/TagAndProbe_VetoMuonIDEffDenominatorReco_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoMuonIDEffDenominatorReco/MC_25ns/",
    			"png",
    			true,
    			0,
    			"",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/NVtxReweight_ZToMuMu_2015C_25ns.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/VetoMuonIDEffDenominatorReco.cfg",
    			2,1,2,3,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/VetoMuonIDEffDenominatorReco/TagAndProbe_VetoMuonIDEffDenominatorReco_SingleMuon_Run2015C_GoodLumi.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoMuonIDEffDenominatorReco/Data/",
    			"png",
    			true,
    			0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/VetoMuonIDEffDenominatorReco/TagAndProbe_VetoMuonIDEffDenominatorReco_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/NVtxReweight_ZToMuMu_2015C_25ns.root");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoMuonIDEffDenominatorReco/Data/eff.root",
			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoMuonIDEffDenominatorReco/MC_25ns/eff.root",
			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoMuonIDEffDenominatorReco/Data/",
			       "ScaleFactor_VetoMuonIDEffDenominatorReco",
			       "VetoMuonIDEffDenominatorReco_2015C");
    
  }



  //*************************************************************************************
  //Veto ID -> Veto ID & Iso
  //*************************************************************************************
  if (option == 31) {

    //*************************************
    //Monte Carlo
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/VetoMuonSelectionEffDenominatorVetoID.cfg",
    			0,0,0,0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/VetoMuonSelectionEffDenominatorVetoID/TagAndProbe_VetoMuonSelectionEffDenominatorVetoID_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoMuonSelectionEffDenominatorVetoID/MC_25ns/",
    			"png",
    			true,
    			0,
    			"",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight_Spring15MCTo2015Data.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/VetoMuonSelectionEffDenominatorVetoID.cfg",
    			2,1,2,3,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/VetoMuonSelectionEffDenominatorVetoID/TagAndProbe_VetoMuonSelectionEffDenominatorVetoID_SingleMuon_Run2015D_GoodLumiGolden.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoMuonSelectionEffDenominatorVetoID/Data/2015D/",
    			"png",
    			true,
    			0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/VetoMuonSelectionEffDenominatorVetoID/TagAndProbe_VetoMuonSelectionEffDenominatorVetoID_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight_Spring15MCTo2015Data.root");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoMuonSelectionEffDenominatorVetoID/Data/2015D/eff.root",
			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoMuonSelectionEffDenominatorVetoID/MC_25ns/eff.root",
			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoMuonSelectionEffDenominatorVetoID/Data/",
			       "ScaleFactor_VetoMuonSelectionEffDenominatorVetoID",
			       "VetoMuonSelectionEffDenominatorVetoID_2015D");
    
  }



  if (option == 32) {

    //*************************************
    //Monte Carlo
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/VetoMuonSelectionEffDenominatorVetoIso.cfg",
    			0,0,0,0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/VetoMuonSelectionEffDenominatorVetoIso/TagAndProbe_VetoMuonSelectionEffDenominatorVetoIso_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoMuonSelectionEffDenominatorVetoIso/MC_25ns/",
    			"png",
    			true,
    			0,
    			"",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight_Spring15MCTo2015Data.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/VetoMuonSelectionEffDenominatorVetoIso.cfg",
    			2,1,2,3,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/VetoMuonSelectionEffDenominatorVetoIso/TagAndProbe_VetoMuonSelectionEffDenominatorVetoIso_SingleMuon_Run2015D_GoodLumiGolden.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoMuonSelectionEffDenominatorVetoIso/Data/2015D/",
    			"png",
    			true,
    			0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/VetoMuonSelectionEffDenominatorVetoIso/TagAndProbe_VetoMuonSelectionEffDenominatorVetoIso_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight_Spring15MCTo2015Data.root");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoMuonSelectionEffDenominatorVetoIso/Data/2015D/eff.root",
			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoMuonSelectionEffDenominatorVetoIso/MC_25ns/eff.root",
			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/VetoMuonSelectionEffDenominatorVetoIso/Data/",
			       "ScaleFactor_VetoMuonSelectionEffDenominatorVetoIso",
			       "VetoMuonSelectionEffDenominatorVetoIso_2015D");
    
  }



  //*************************************************************************************
  //Tight ID -> Tight Selection
  //*************************************************************************************
  if (option == 13) {

    //*************************************
    //Monte Carlo
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/MuonTightSelectionDenominatorReco.cfg",
    			0,0,0,0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/TightMuonSelectionEffDenominatorTightID/TagAndProbe_TightMuonSelectionEffDenominatorTightID_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/TightMuonSelectionEffDenominatorTightID/MC_25ns_2015D/",
    			"png",
    			true,
    			0,
    			"",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/NVtxReweight_ZToMuMu_2015D_25ns.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/MuonTightSelectionDenominatorReco.cfg",
    			//2,1,2,3,
    			2,2,2,2,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/TightMuonSelectionEffDenominatorTightID/TagAndProbe_TightMuonSelectionEffDenominatorTightID_SingleMuon_Run2015D_GoodLumi.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/TightMuonSelectionEffDenominatorTightID/Data/2015D/",
    			"png",
    			true,
    			0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/TightMuonSelectionEffDenominatorTightID/TagAndProbe_TightMuonSelectionEffDenominatorTightID_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/NVtxReweight_ZToMuMu_2015D_25ns.root");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/TightMuonSelectionEffDenominatorTightID/Data/2015D/eff.root",
    			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/TightMuonSelectionEffDenominatorTightID/MC_25ns_2015D/eff.root",
    			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/TightMuonSelectionEffDenominatorTightID/Data/",
    			       "ScaleFactor_TightMuonSelectionEffDenominatorTightID",
    			       "TightMuonSelectionEffDenominatorTightID_2015D",
    			       20,200,0.8,1.15
    			       );
    
  }





}
