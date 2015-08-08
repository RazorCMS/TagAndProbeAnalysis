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


void computeEfficiency( int option = 1) {

  /*void MakeEfficiencyPlots(const TString conf, const Int_t   sigModPass, const Int_t   bkgModPass, 
    const Int_t   sigModFail, const Int_t   bkgModFail, const TString infilename,
    const TString outputDir, const TString format, const Bool_t  doAbsEta, const Int_t   charge,
    const TString signalmcfilename="", const TString pileupReweightFile = "")*/

  //*************************************************************************************
  //Reco -> Tight Electrons
  //*************************************************************************************
  if (option == 1) {

    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/ElectronTightDenominatorReco.cfg",
			0,0,0,0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/ElectronRecoToTight_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronRecoToTight_Zee",
			"png",
			false,
			0,
			"",
			"/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/RazorCommon/pileup_weights_2015B.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/ElectronTightDenominatorReco.cfg",
			2,1,2,2,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/ElectronRecoToTight_PR_SingleElectron.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronRecoToTight_PR",
			"png",
			false,
			0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/ElectronRecoToTight_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronRecoToTight_PR/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronRecoToTight_Zee/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronRecoToTight_PR",
			       "ScaleFactor_EleRecoToTight",
			       "EleRecoToTightEff_2015B");
    
  }

  //*************************************************************************************
  //Reco -> Loose Electrons
  //*************************************************************************************
  if (option == 2) {

    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/ElectronTightDenominatorReco.cfg",
			0,0,0,0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/ElectronRecoToLoose_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronRecoToLoose_Zee",
			"png",
			false,
			0,
			"",
			"/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/RazorCommon/pileup_weights_2015B.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/ElectronTightDenominatorReco.cfg",
			2,1,2,2,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/ElectronRecoToLoose_PR_SingleElectron.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronRecoToLoose_PR",
			"png",
			false,
			0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/ElectronRecoToLoose_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronRecoToLoose_PR/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronRecoToLoose_Zee/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronRecoToLoose_PR",
			       "ScaleFactor_EleRecoToLoose",
			       "EleRecoToLooseEff_2015B");
    
  }

  //*************************************************************************************
  //Reco -> Veto Electrons
  //*************************************************************************************
  if (option == 3) {
  }

  //*************************************************************************************
  //Reco -> Tight Muons
  //*************************************************************************************
  if (option == 4) {

    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/MuonTightDenominatorReco.cfg",
			0,0,0,0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/MuonRecoToTight_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToTight_Zmm",
			"png",
			false,
			0,
			"",
			"/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/RazorCommon/pileup_weights_2015B.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/MuonTightDenominatorReco.cfg",
			2,1,2,2,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/MuonRecoToTight_PR_SingleMuon_Golden.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToTight_PR",
			"png",
			false,
			0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/MuonRecoToTight_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToTight_PR/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToTight_Zmm/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToTight_PR",
			       "ScaleFactor_MuonRecoToTightEff",
			       "MuonRecoToTightEff_2015B");
    
  }

  //*************************************************************************************
  //Reco -> Loose Muons
  //*************************************************************************************
  if (option == 5) {

    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/MuonTightDenominatorReco.cfg",
			0,0,0,0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/MuonRecoToLoose_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToLoose_Zmm",
			"png",
			false,
			0,
			"",
			"/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/RazorCommon/pileup_weights_2015B.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/MuonTightDenominatorReco.cfg",
			2,1,2,2,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/MuonRecoToLoose_PR_SingleMuon.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToLoose_PR",
			"png",
			false,
			0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/MuonRecoToLoose_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToLoose_PR/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToLoose_Zmm/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToLoose_PR",
			       "ScaleFactor_MuonRecoToLooseEff",
			       "MuonRecoToLooseEff_2015B");
    
  }

  //*************************************************************************************
  //Reco -> POG Tight Muons
  //*************************************************************************************
  if (option == 6) {

    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/MuonTightDenominatorReco.cfg",
			0,0,0,0,
			//"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/MuonRecoToPOGTight_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/MuonRecoToPOGTight_test.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToPOGTight_Zmm_test",
			"png",
			false,
			0,
			"",
			"/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/RazorCommon/pileup_weights_2015B.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    /*    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/MuonTightDenominatorReco.cfg",
			2,1,2,2,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/MuonRecoToPOGTight_PR_SingleMuon_Golden.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToPOGTight_PR",
			"png",
			false,
			0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/MuonRecoToPOGTight_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"");*/

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    /*makeEfficiencyScaleFactors("/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToPOGTight_PR/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToPOGTight_Zmm/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToPOGTight_PR",
			       "ScaleFactor_MuonRecoToTightEff",
			       "MuonRecoToTightEff_2015B");*/
    
  }

  
}
