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


void computeEfficiency_SUSY_SF( int option = 1) {

  /*void MakeEfficiencyPlots(const TString conf, const Int_t   sigModPass, const Int_t   bkgModPass, 
    const Int_t   sigModFail, const Int_t   bkgModFail, const TString infilename,
    const TString outputDir, const TString format, const Bool_t  doAbsEta, const Int_t   charge,
    const TString signalmcfilename="", const TString pileupReweightFile = "")*/

  //*************************************************************************************
  //Reco -> Tight ID Electrons
  //*************************************************************************************
  if (option == 1) {

    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/ElectronSUSY_SF.cfg",
			0,0,0,0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/ElectronRecoToTightID/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronRecoToTightID_Zee",
			"png",
			false,
			0,
			"",
			"/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/RazorCommon/pileup_weights_2015B.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/ElectronSUSY_SF.cfg",
			2,1,2,2,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/ElectronRecoToTightID/SingleElectron_Run2015B.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronRecoToTightID_PR",
			"png",
			false,
			0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/ElectronRecoToTightID/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronRecoToTightID_PR/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronRecoToTightID_Zee/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronRecoToTightID_PR",
			       "ScaleFactor_EleRecoToTightID",
			       "EleRecoToTightIDEff_2015B");
    
  }

  //*************************************************************************************
  //Tight ID -> Iso Electrons
  //*************************************************************************************
  if (option == 2) {

    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/ElectronSUSY_SF.cfg",
			0,0,0,0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/ElectronTightIDToTightIso/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronTightIDToTightIso_Zee",
			"png",
			false,
			0,
			"",
			"/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/RazorCommon/pileup_weights_2015B.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/ElectronSUSY_SF.cfg",
			0,0,0,0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/ElectronTightIDToTightIso/SingleElectron_Run2015B.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronTightIDToTightIso_PR",
			"png",
			false,
			0,
			"", 
			"");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronTightIDToTightIso_PR/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronTightIDToTightIso_Zee/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronTightIDToTightIso_PR",
			       "ScaleFactor_EleTightIDToTightIso",
			       "EleTightIDToTightIsoEff_2015B");
  }

  //*************************************************************************************
  //Reco -> Loose ID Electrons
  //*************************************************************************************
  if (option == 3) {

    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/ElectronSUSY_SF.cfg",
			0,0,0,0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/ElectronRecoToLooseID/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronRecoToLooseID_Zee",
			"png",
			false,
			0,
			"",
			"/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/RazorCommon/pileup_weights_2015B.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/ElectronSUSY_SF.cfg",
			2,1,2,2,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/ElectronRecoToLooseID/SingleElectron_Run2015B.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronRecoToLooseID_PR",
			"png",
			false,
			0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/ElectronRecoToLooseID/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronRecoToLooseID_PR/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronRecoToLooseID_Zee/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronRecoToLooseID_PR",
			       "ScaleFactor_EleRecoToLooseID",
			       "EleRecoToLooseIDEff_2015B");
    
  }

  //*************************************************************************************
  //Loose ID -> Iso Electrons
  //*************************************************************************************
  if (option == 4) {

    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/ElectronSUSY_SF.cfg",
			0,0,0,0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/ElectronLooseIDToLooseIso/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronLooseIDToLooseIso_Zee",
			"png",
			false,
			0,
			"",
			"/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/RazorCommon/pileup_weights_2015B.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/ElectronSUSY_SF.cfg",
			2,1,2,1,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/ElectronLooseIDToLooseIso/SingleElectron_Run2015B.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronLooseIDToLooseIso_PR",
			"png",
			false,
			0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/ElectronLooseIDToLooseIso/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronLooseIDToLooseIso_PR/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronLooseIDToLooseIso_Zee/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/ElectronLooseIDToLooseIso_PR",
			       "ScaleFactor_EleLooseIDToLooseIso",
			       "EleLooseIDToLooseIsoEff_2015B");
  }

  //*************************************************************************************
  //Reco -> Tight ID Muons
  //*************************************************************************************
  if (option == 5) {

    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/MuonSUSY_SF.cfg",
			0,0,0,0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/MuonRecoToTightID/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToTightID_Zee",
			"png",
			false,
			0,
			"",
			"/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/RazorCommon/pileup_weights_2015B.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/MuonSUSY_SF.cfg",
			2,1,2,2,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/MuonRecoToTightID/SingleMuon_Run2015B.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToTightID_PR",
			"png",
			false,
			0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/MuonRecoToTightID/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToTightID_PR/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToTightID_Zee/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToTightID_PR",
			       "ScaleFactor_MuRecoToTightID",
			       "MuRecoToTightIDEff_2015B");
    
  }

  //*************************************************************************************
  //Tight ID -> Iso Muons
  //*************************************************************************************
  if (option == 6) {

    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/MuonSUSY_SF.cfg",
			0,0,0,0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/MuonTightIDToTightIso/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonTightIDToTightIso_Zee",
			"png",
			false,
			0,
			"",
			"/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/RazorCommon/pileup_weights_2015B.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/MuonSUSY_SF.cfg",
			0,0,0,0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/MuonTightIDToTightIso/SingleMuon_Run2015B.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonTightIDToTightIso_PR",
			"png",
			false,
			0,
			"",
			"");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonTightIDToTightIso_PR/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonTightIDToTightIso_Zee/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonTightIDToTightIso_PR",
			       "ScaleFactor_MuTightIDToTightIso",
			       "MuTightIDToTightIsoEff_2015B");
  }

  //*************************************************************************************
  //Reco -> Loose ID Muons
  //*************************************************************************************
  if (option == 7) {

    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/MuonSUSY_SF.cfg",
			0,0,0,0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/MuonRecoToLooseID/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToLooseID_Zee",
			"png",
			false,
			0,
			"",
			"/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/RazorCommon/pileup_weights_2015B.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/MuonSUSY_SF.cfg",
			2,1,2,1,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/MuonRecoToLooseID/SingleMuon_Run2015B.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToLooseID_PR",
			"png",
			false,
			0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/MuonRecoToLooseID/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToLooseID_PR/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToLooseID_Zee/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonRecoToLooseID_PR",
			       "ScaleFactor_MuRecoToLooseID",
			       "MuRecoToLooseIDEff_2015B");
    
  }

  //*************************************************************************************
  //Loose ID -> Iso Muons
  //*************************************************************************************
  if (option == 8) {

    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/MuonSUSY_SF.cfg",
			0,0,0,0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/MuonLooseIDToLooseIso/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonLooseIDToLooseIso_Zee",
			"png",
			false,
			0,
			"",
			"/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/RazorCommon/pileup_weights_2015B.root");
    
    //*************************************
    //Data : Do Fit
    //*************************************
    MakeEfficiencyPlots("/afs/cern.ch/user/j/jlawhorn/RazorCMS/CMSSW_7_1_5/src/TagAndProbeAnalysis/config/MuonSUSY_SF.cfg",
			2,1,2,1,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/MuonLooseIDToLooseIso/SingleMuon_Run2015B.root",
			"/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonLooseIDToLooseIso_PR",
			"png",
			false,
			0,
			"root://eoscms//store/user/jlawhorn/RazorTagAndProbe/MuonLooseIDToLooseIso/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root",
			"");

    //*************************************
    //Make Scale Factor Tables
    //*************************************
    makeEfficiencyScaleFactors("/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonLooseIDToLooseIso_PR/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonLooseIDToLooseIso_Zee/eff.root",
			       "/afs/cern.ch/work/j/jlawhorn/public/RazorLeptonEfficiencies/MuonLooseIDToLooseIso_PR",
			       "ScaleFactor_MuLooseIDToLooseIso",
			       "MuLooseIDToLooseIsoEff_2015B");
  }


}
