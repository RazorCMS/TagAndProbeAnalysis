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


void computePhotonEfficiency( int option = 0) {

  //*************************************************************************************
  //Reco -> Loose 2016
  //*************************************************************************************
  if (option == 0) {
 
    //*************************************
    //Monte Carlo
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/PhotonLooseDenominatorReco.cfg",
    			0,0,0,0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/PhoLooseEffDenominatorReco/2016/5July/TagAndProbe_PhoLooseEffDenominatorReco_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoLooseEffDenominatorReco/MC/2016/",
    			"png",
    			false,
    			0,
    			"",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupWeights/PileupReweight2016_ICHEP.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/PhotonLooseDenominatorReco.cfg",
    			2,1,2,2,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/PhoLooseEffDenominatorReco/2016/TagAndProbe_PhoLooseEffDenominatorReco_SingleElectron_2016_PRv2_GoodLumiGolden12p9ifb.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoLooseEffDenominatorReco/Data/2016/",
    			"png",
    			false,
    			0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/PhoLooseEffDenominatorReco/2016/5July/TagAndProbe_PhoLooseEffDenominatorReco_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupWeights/PileupReweight2016_ICHEP.root");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoLooseEffDenominatorReco/Data/2016/eff.root",
    			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoLooseEffDenominatorReco/MC/2016/eff.root",
    			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoLooseEffDenominatorReco/Data/",
    			       "ScaleFactor_PhoLooseEffDenominatorReco",
    			       "PhoLooseEffDenominatorReco_2016_ICHEP",
			       25,100,
			       0.5, 1.15
			       );
    
  }

  //*************************************************************************************
  //Reco -> Loose 2015
  //*************************************************************************************
  if (option == 1) {
 
    //*************************************
    //Monte Carlo
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/PhotonLooseDenominatorReco.cfg",
    			0,0,0,0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/PhoLooseEffDenominatorReco/2015/TagAndProbe_PhoLooseEffDenominatorReco_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoLooseEffDenominatorReco/MC/2015/",
    			"png",
    			false,
    			0,
    			"",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight2015_7_6.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis//config/PhotonLooseDenominatorReco.cfg",
    			2,1,2,2,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/PhoLooseEffDenominatorReco/2015/TagAndProbe_PhoLooseEffDenominatorReco_SingleElectron_2015_GoodLumiGolden.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoLooseEffDenominatorReco/Data/2015/",
    			"png",
    			false,
    			0,
    			"/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/Run2Analysis/TagAndProbe/PhoLooseEffDenominatorReco/2015/TagAndProbe_PhoLooseEffDenominatorReco_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root",
    			"/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/RazorAnalyzer/data/PileupReweight2015_7_6.root");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoLooseEffDenominatorReco/Data/2015/eff.root",
    			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoLooseEffDenominatorReco/MC/2015/eff.root",
    			       "/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_4_2/src/TagAndProbeAnalysis/output/PhoLooseEffDenominatorReco/Data/2015/",
    			       "ScaleFactor_PhoLooseEffDenominatorReco",
    			       "PhoLooseEffDenominatorReco_2015",
			       25,100,
			       0.5, 1.15
			       );
    
  }



}
