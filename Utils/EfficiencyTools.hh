//================================================================================================
//
// Signal Extraction
//-------------------
//  0: probe counting
//  1: Breit-Wigner convolved with Crystal Ball function
//  2: MC template convolved with Gaussian
//  3: Phil's Crystal Ball based "Voigtian" shape
//  4: Unbinned MC data convolved with Gaussian
//
// Background Model
//------------------
//  0: no background
//  1: exponential model
//  2: erf*exp model
//  3: double exponential model
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

#include "TagAndProbeAnalysis/Utils/CPlot.hh"	    // helper class for plots
#include "TagAndProbeAnalysis/Utils/PlotStyle.hh"  // style settings for drawing
#include "TagAndProbeAnalysis/Utils/CEffUser1D.hh"     // class for handling efficiency graphs
#include "TagAndProbeAnalysis/Utils/CEffUser2D.hh"     // class for handling efficiency tables

// structure for output ntuple
#include "RazorAnalyzer/include/TagAndProbePair.h"
#include "TagAndProbeAnalysis/FitModels/ZSignals.hh"
#include "TagAndProbeAnalysis/FitModels/ZBackgrounds.hh"
#endif

// RooFit headers
#include "RooRealVar.h"
#include "RooCategory.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooFormulaVar.h"
#include "RooSimultaneous.h"
#include "RooAddPdf.h"
#include "RooFitResult.h"
#include "RooExtendPdf.h"
#include "RooWorkspace.h"
#include "RooMsgService.h"
 

//=== FUNCTION DECLARATIONS ======================================================================================

// generate web page
void makeHTML(const TString outDir);
void makeHTML(const TString outDir, const TString name, const Int_t n);

// Make efficiency graph
TGraphAsymmErrors* makeEffGraph(const vector<Double_t> &edgesv, const vector<TTree*> &passv, const vector<TTree*> &failv, const Int_t method,
                                const TString name, const Double_t massLo, const Double_t massHi, const TString format, const Bool_t doAbsEta);
TGraphAsymmErrors* makeEffGraph(const vector<Double_t> &edgesv, const vector<TTree*> &passv, const vector<TTree*> &failv,
                                const Int_t sigpass, const Int_t bkgpass, const Int_t sigfail, const Int_t bkgfail, 		                
				const TString name, const Double_t massLo, const Double_t massHi, const TString format, const Bool_t doAbsEta);

// Make 2D efficiency map
void makeEffHist2D(TH2D *hEff, TH2D *hErrl, TH2D *hErrh, const vector<TTree*> &passv, const vector<TTree*> &failv, const Int_t method,
                   const TString name, const Double_t massLo, const Double_t massHi, const TString format, const Bool_t doAbsEta);
void makeEffHist2D(TH2D *hEff, TH2D *hErrl, TH2D *hErrh, const vector<TTree*> &passv, const vector<TTree*> &failv,
                   const Int_t sigpass, const Int_t bkgpass, const Int_t sigfail, const Int_t bkgfail, 
		   const TString name, const Double_t massLo, const Double_t massHi, const TString format, const Bool_t doAbsEta);


// Generate MC-based signal templates
void generateHistTemplates(const TString infilename, 
                           const TString outfilename,
                           TH1F *puWeights, 
                           const vector<Double_t> &ptEdgesv, const vector<Double_t> &etaEdgesv, const vector<Double_t> &phiEdgesv, 
                           const vector<Double_t> &npvEdgesv, const vector<Double_t> &rhoEdgesv,
		           const Double_t massLo, const Double_t massHi, const Bool_t doAbsEta, const Int_t charge, Bool_t collapseEtaBins = false); 
void generateDataTemplates(const TString infilename,
                           const vector<Double_t> &ptEdgesv, const vector<Double_t> &etaEdgesv, const vector<Double_t> &phiEdgesv, 
                           const vector<Double_t> &npvEdgesv, const vector<Double_t> &rhoEdgesv,
		           const Double_t massLo, const Double_t massHi, const Bool_t doAbsEta, const Int_t charge); 
			   
// Perform count
void performCount(Double_t &resEff, Double_t &resErrl, Double_t &resErrh,
                  const Int_t ibin, const Double_t xbinLo, const Double_t xbinHi, const Double_t ybinLo, const Double_t ybinHi,
		  TTree *passTree, TTree *failTree, const Int_t method, 
		  const TString name, const Double_t massLo, const Double_t massHi, const TString format, const Bool_t doAbsEta,
		  TCanvas *cpass, TCanvas *cfail);

// Perform fit
void performFit(Double_t &resEff, Double_t &resErrl, Double_t &resErrh,
                const Int_t ibin, const Double_t xbinLo, const Double_t xbinHi, const Double_t ybinLo, const Double_t ybinHi,
		TTree *passTree, TTree *failTree,
		const Int_t sigpass, const Int_t bkgpass, const Int_t sigfail, const Int_t bkgfail, 
		const TString name, const Double_t massLo, const Double_t massHi, const TString format, const Bool_t doAbsEta,
		TCanvas *cpass, TCanvas *cfail);

// Print correlations
void printCorrelations(ostream& os, RooFitResult *res);

//=== MAIN MACRO ================================================================================================= 

void MakeEfficiencyPlots(const TString conf,          // input file
             const Int_t   sigModPass,    // signal extraction method for PASS sample
	     const Int_t   bkgModPass,    // background model for PASS sample
	     const Int_t   sigModFail,    // signal extraction method for FAIL sample	     
	     const Int_t   bkgModFail,    // background model for FAIL sample
	     const TString infilename,    // ROOT file of probes
             const TString outputDir,     // output directory
             const TString format,        // plot format
	     const Bool_t  doAbsEta,      // bin in |eta| instead of eta
	     const Int_t   charge,        // 0 (no charge requirement), -1, +1
	     const TString signalmcfilename="", // ROOT file containing MC events to generate templates from
             const TString pileupReweightFile = ""
) {
  gBenchmark->Start("plotEff");


  //--------------------------------------------------------------------------------------------------------------
  // Settings 
  //============================================================================================================== 
  
  // mass region
  const Double_t massLo = 60;
  const Double_t massHi = 120;
  
  // bin edges for kinematic variables
  vector<Double_t> ptBinEdgesv;
  vector<Double_t> etaBinEdgesv;
  vector<Double_t> phiBinEdgesv;
  vector<Double_t> npvBinEdgesv;
  vector<Double_t> rhoBinEdgesv;

  // option flags
  Int_t opts[7];

  //
  // parse .bins file
  //
  ifstream ifs;
  ifs.open(conf.Data());
  assert(ifs.is_open());
  string line;
  Int_t state=0;
  while(getline(ifs,line)) {
    if(line[0]=='#') continue;
    if(line[0]=='%') { 
      state++; 
      continue; 
    }
    
    Double_t edge;
    stringstream ss(line);
    if(state==0) {
      ss >> opts[0] >> opts[1] >> opts[2] >> opts[3] >> opts[4] >> opts[5] >> opts[6];
    } else {
      ss >> edge;
      if(state==1)      { ptBinEdgesv.push_back(edge);  }
      else if(state==2) { etaBinEdgesv.push_back(edge); }
      else if(state==3) { phiBinEdgesv.push_back(edge); }
      else if(state==4) { npvBinEdgesv.push_back(edge); }
      else if(state==5) { rhoBinEdgesv.push_back(edge); }
    }
  }
  ifs.close();
  
  // efficiency error calculation method
  // method: 0 -> Clopper-Pearson
  //         1 -> Feldman-Cousins
  const Int_t method=0;
  
  gSystem->mkdir(outputDir,kTRUE);
  CPlot::sOutDir = outputDir + TString("/plots");
  gSystem->mkdir(CPlot::sOutDir,kTRUE);  
  cout << "sOutDir = " << CPlot::sOutDir << endl;

  // y-axis range
//   const Double_t yhigh = 1.03;
//   const Double_t ylow  = 0.6;
  const Double_t yhigh = 1.03;
  const Double_t ylow  = 0.0;
 

  //--------------------------------------------------------------------------------------------------------------
  // Main analysis code 
  //==============================================================================================================   
  
  //  
  // Set up binning in kinematic variables
  //        
  const UInt_t ptNbins = ptBinEdgesv.size()-1;
  Double_t ptEdges[ptBinEdgesv.size()];
  for(UInt_t iedge=0; iedge<ptBinEdgesv.size(); iedge++)
    ptEdges[iedge] = ptBinEdgesv[iedge];
    
  const UInt_t etaNbins = etaBinEdgesv.size()-1;
  Double_t etaEdges[etaBinEdgesv.size()];
  for(UInt_t iedge=0; iedge<etaBinEdgesv.size(); iedge++)
    etaEdges[iedge] = etaBinEdgesv[iedge];
  
  const UInt_t phiNbins = phiBinEdgesv.size()-1;
  Double_t phiEdges[phiBinEdgesv.size()];
  for(UInt_t iedge=0; iedge<phiBinEdgesv.size(); iedge++)
    phiEdges[iedge] = phiBinEdgesv[iedge];
  
  const UInt_t npvNbins = npvBinEdgesv.size()-1;
  Double_t npvEdges[npvBinEdgesv.size()];
  for(UInt_t iedge=0; iedge<npvBinEdgesv.size(); iedge++)
    npvEdges[iedge] = npvBinEdgesv[iedge];

  const UInt_t rhoNbins = rhoBinEdgesv.size()-1;
  Double_t rhoEdges[rhoBinEdgesv.size()];
  for(UInt_t iedge=0; iedge<rhoBinEdgesv.size(); iedge++)
    rhoEdges[iedge] = rhoBinEdgesv[iedge];
  
  char tname[50];
  Float_t mass,wgt;
    
  vector<TTree*> passTreePtv;
  vector<TTree*> failTreePtv;
  for(UInt_t ibin=0; ibin<ptNbins; ibin++) {
    sprintf(tname,"passPt_%i",ibin);
    passTreePtv.push_back(new TTree(tname,""));
    passTreePtv[ibin]->Branch("m",&mass,"m/F");
    passTreePtv[ibin]->Branch("w",&wgt,"w/F");
    passTreePtv[ibin]->SetDirectory(0);
    sprintf(tname,"failPt_%i",ibin);
    failTreePtv.push_back(new TTree(tname,""));
    failTreePtv[ibin]->Branch("m",&mass,"m/F");
    failTreePtv[ibin]->Branch("w",&wgt,"w/F");
    failTreePtv[ibin]->SetDirectory(0);
  }
  
  vector<TTree*> passTreeEtav;
  vector<TTree*> failTreeEtav;
  for(UInt_t ibin=0; ibin<etaNbins; ibin++) {
    sprintf(tname,"passEta_%i",ibin);
    passTreeEtav.push_back(new TTree(tname,""));
    passTreeEtav[ibin]->Branch("m",&mass,"m/F");
    passTreeEtav[ibin]->Branch("w",&wgt,"w/F");
    passTreeEtav[ibin]->SetDirectory(0);
    sprintf(tname,"failEta_%i",ibin);
    failTreeEtav.push_back(new TTree(tname,""));
    failTreeEtav[ibin]->Branch("m",&mass,"m/F");
    failTreeEtav[ibin]->Branch("w",&wgt,"w/F");
    failTreeEtav[ibin]->SetDirectory(0);
  }
  
  vector<TTree*> passTreePhiv;  
  vector<TTree*> failTreePhiv;
  for(UInt_t ibin=0; ibin<phiNbins; ibin++) {
    sprintf(tname,"passPhi_%i",ibin);
    passTreePhiv.push_back(new TTree(tname,""));
    passTreePhiv[ibin]->Branch("m",&mass,"m/F");
    passTreePhiv[ibin]->Branch("w",&wgt,"w/F");
    passTreePhiv[ibin]->SetDirectory(0);
    sprintf(tname,"failPhi_%i",ibin);
    failTreePhiv.push_back(new TTree(tname,""));
    failTreePhiv[ibin]->Branch("m",&mass,"m/F");
    failTreePhiv[ibin]->Branch("w",&wgt,"w/F");
    failTreePhiv[ibin]->SetDirectory(0);
  }
  
  vector<TTree*> passTreeEtaPtv;  
  vector<TTree*> failTreeEtaPtv;
  for(UInt_t ibin=0; ibin<(etaNbins*ptNbins); ibin++) {
    sprintf(tname,"passEtaPt_%i",ibin);
    passTreeEtaPtv.push_back(new TTree(tname,""));
    passTreeEtaPtv[ibin]->Branch("m",&mass,"m/F");
    passTreeEtaPtv[ibin]->Branch("w",&wgt,"w/F");
    passTreeEtaPtv[ibin]->SetDirectory(0);
    sprintf(tname,"failEtaPt_%i",ibin);
    failTreeEtaPtv.push_back(new TTree(tname,""));
    failTreeEtaPtv[ibin]->Branch("m",&mass,"m/F");
    failTreeEtaPtv[ibin]->Branch("w",&wgt,"w/F");
    failTreeEtaPtv[ibin]->SetDirectory(0);
  }
  
  vector<TTree*> passTreeEtaPhiv;
  vector<TTree*> failTreeEtaPhiv;
  for(UInt_t ibin=0; ibin<(etaNbins*phiNbins); ibin++) {
    sprintf(tname,"passEtaPhi_%i",ibin); 
    passTreeEtaPhiv.push_back(new TTree(tname,""));
    passTreeEtaPhiv[ibin]->Branch("m",&mass,"m/F");
    passTreeEtaPhiv[ibin]->Branch("w",&wgt,"w/F");
    passTreeEtaPhiv[ibin]->SetDirectory(0);
    sprintf(tname,"failEtaPhi_%i",ibin);
    failTreeEtaPhiv.push_back(new TTree(tname,""));
    failTreeEtaPhiv[ibin]->Branch("m",&mass,"m/F");
    failTreeEtaPhiv[ibin]->Branch("w",&wgt,"w/F");
    failTreeEtaPhiv[ibin]->SetDirectory(0);
  }

  vector<TTree*> passTreeNPVv;  
  vector<TTree*> failTreeNPVv;
  for(UInt_t ibin=0; ibin<npvNbins; ibin++) {
    sprintf(tname,"passNPV_%i",ibin);
    passTreeNPVv.push_back(new TTree(tname,""));
    passTreeNPVv[ibin]->Branch("m",&mass,"m/F");
    passTreeNPVv[ibin]->Branch("w",&wgt,"w/F");
    passTreeNPVv[ibin]->SetDirectory(0);
    sprintf(tname,"failNPV_%i",ibin);
    failTreeNPVv.push_back(new TTree(tname,""));
    failTreeNPVv[ibin]->Branch("m",&mass,"m/F");
    failTreeNPVv[ibin]->Branch("w",&wgt,"w/F");
    failTreeNPVv[ibin]->SetDirectory(0);
  }  

  vector<TTree*> passTreeRhov;  
  vector<TTree*> failTreeRhov;
  for(UInt_t ibin=0; ibin<npvNbins; ibin++) {
    sprintf(tname,"passRho_%i",ibin);
    passTreeRhov.push_back(new TTree(tname,""));
    passTreeRhov[ibin]->Branch("m",&mass,"m/F");
    passTreeRhov[ibin]->Branch("w",&wgt,"w/F");
    passTreeRhov[ibin]->SetDirectory(0);
    sprintf(tname,"failRho_%i",ibin);
    failTreeRhov.push_back(new TTree(tname,""));
    failTreeRhov[ibin]->Branch("m",&mass,"m/F");
    failTreeRhov[ibin]->Branch("w",&wgt,"w/F");
    failTreeRhov[ibin]->SetDirectory(0);
  }  

  //
  // Pile-up reweighting functions 
  //

  cout << "PUFILE: " << pileupReweightFile.Data() << endl;
  Bool_t doPUReweighting = kTRUE;
  if (pileupReweightFile == "") doPUReweighting = kFALSE;

  TH1F *puWeights = 0;
  TFile *pufile = 0;
  if (doPUReweighting) {
    pufile = new TFile(pileupReweightFile.Data(), "READ");  
    puWeights = (TH1F*)pufile->Get("PileupReweight");
    //puWeights = (TH1F*)pufile->Get("NVtxReweight");
    assert(puWeights);
  }

  cout << "load pu: " << pileupReweightFile.Data() << " " << Bool_t(puWeights) << endl;


  //
  // Generate histogram templates from MC if necessary
  //
  if(sigModPass==2 || sigModFail==2) {
    generateHistTemplates(signalmcfilename,"signalHistTemplates.root", puWeights, ptBinEdgesv,etaBinEdgesv,phiBinEdgesv,npvBinEdgesv,rhoBinEdgesv,massLo,massHi,doAbsEta,charge);
  }
  if(sigModPass==4 || sigModFail==4) {
    generateDataTemplates(signalmcfilename,ptBinEdgesv,etaBinEdgesv,phiBinEdgesv,npvBinEdgesv,rhoBinEdgesv,massLo,massHi,doAbsEta,charge);
  }
    
  //
  // Read in probes data
  //
  TagAndProbePair *data = new TagAndProbePair;
  data->LoadTree(infilename);
  
  for(UInt_t ientry=0; ientry < data->tree_->GetEntries(); ientry++) {
    data->tree_->GetEntry(ientry);
    
    //if((data->charge)*charge < 0) continue;
    if(data->mass < massLo)  continue;
    if(data->mass > massHi)  continue;
    
    //make MET cut
    //if(data->met > 30) continue;
    //if(!(data->passTighterTag)) continue;

    mass = data->mass;
    wgt  = data->weight/abs(data->weight);
    if (doPUReweighting) {
      Int_t npuxbin = puWeights->GetXaxis()->FindFixBin(TMath::Min(double(data->NPU_0), 49.499));
      //Int_t npuxbin = puWeights->GetXaxis()->FindFixBin(TMath::Min(double(data->NPV), 30.499));
      wgt = puWeights->GetBinContent(npuxbin);
    }
    
    Int_t ipt=-1;
    for(UInt_t ibin=0; ibin<ptNbins; ibin++)
      if((data->pt >= ptBinEdgesv[ibin]) && (data->pt < ptBinEdgesv[ibin+1]))
        ipt = ibin;
    if(ipt<0) continue;
    
    Int_t ieta=-1;
    for(UInt_t ibin=0; ibin<etaNbins; ibin++) {
      if(doAbsEta) {
        assert(etaBinEdgesv[ibin]>=0);
        if((fabs(data->eta) >= etaBinEdgesv[ibin]) && (fabs(data->eta) < etaBinEdgesv[ibin+1]))
          ieta = ibin;
      } else {
        if((data->eta >= etaBinEdgesv[ibin]) && (data->eta < etaBinEdgesv[ibin+1]))
          ieta = ibin;
      }
    }
    if(ieta<0) continue;
	
    Int_t iphi=-1;
    for(UInt_t ibin=0; ibin<phiNbins; ibin++)
      if((data->phi >= phiBinEdgesv[ibin]) && (data->phi < phiBinEdgesv[ibin+1]))
        iphi = ibin;
    if(iphi<0) continue;

    Int_t inpv=-1;
    for(UInt_t ibin=0; ibin<npvNbins; ibin++)
      if((data->NPV >= npvBinEdgesv[ibin]) && (data->NPV < npvBinEdgesv[ibin+1]))
        inpv = ibin;
    if(inpv<0) continue;

    Int_t irho=-1;
    for(UInt_t ibin=0; ibin<rhoNbins; ibin++)
      if((data->Rho >= rhoBinEdgesv[ibin]) && (data->Rho < rhoBinEdgesv[ibin+1]))
        irho = ibin;
    if(irho<0) continue;
        
    if(data->pass) {
      passTreePtv[ipt]->Fill();
      passTreeEtav[ieta]->Fill();
      passTreePhiv[iphi]->Fill();
      passTreeEtaPtv[ipt*etaNbins + ieta]->Fill();
      passTreeEtaPhiv[iphi*etaNbins + ieta]->Fill();
      passTreeNPVv[inpv]->Fill();
      passTreeRhov[inpv]->Fill();
    } else {
      failTreePtv[ipt]->Fill();
      failTreeEtav[ieta]->Fill();
      failTreePhiv[iphi]->Fill();
      failTreeEtaPtv[ipt*etaNbins + ieta]->Fill();
      failTreeEtaPhiv[iphi*etaNbins + ieta]->Fill();
      failTreeNPVv[inpv]->Fill();
      failTreeRhov[inpv]->Fill();
    }    
  }  
  
  //
  // Compute efficiencies and make plots 
  // 
  TCanvas *c = MakeCanvas("c","c",800,600);
   
  TGraphAsymmErrors *grEffPt=0;
  TGraphAsymmErrors *grEffEta=0;
  TGraphAsymmErrors *grEffPhi=0;
  TGraphAsymmErrors *grEffNPV=0;
  TGraphAsymmErrors *grEffRho=0;
  TH2D *hEffEtaPt   = new TH2D("hEffEtaPt","",etaNbins,etaEdges,ptNbins,ptEdges);
  TH2D *hErrlEtaPt  = (TH2D*)hEffEtaPt->Clone("hErrlEtaPt");
  TH2D *hErrhEtaPt  = (TH2D*)hEffEtaPt->Clone("hErrhEtaPt");
  TH2D *hEffEtaPhi  = new TH2D("hEffEtaPhi","",etaNbins,etaEdges,phiNbins,phiEdges);
  TH2D *hErrlEtaPhi = (TH2D*)hEffEtaPhi->Clone("hErrlEtaPhi");
  TH2D *hErrhEtaPhi = (TH2D*)hEffEtaPhi->Clone("hErrhEtaPhi");
    
  if(sigModPass==0 && sigModFail==0) {  // probe counting
    
    // efficiency in pT
    if(opts[0]) {
      grEffPt = makeEffGraph(ptBinEdgesv, passTreePtv, failTreePtv, method, "pt", massLo, massHi, format, doAbsEta);
      grEffPt->SetName("grEffPt");
      CPlot plotEffPt("effpt","","probe p_{T} [GeV/c]","#varepsilon");    
      plotEffPt.AddGraph(grEffPt,"",kBlack);
      plotEffPt.SetYRange(ylow,yhigh);
      //plotEffPt.SetYRange(0.75,1.05);
      plotEffPt.SetXRange(0.9*(ptBinEdgesv[0]),1.1*(ptBinEdgesv[ptNbins-1]));
      plotEffPt.Draw(c,kTRUE,format);    
    }

    // efficiency in eta
    if(opts[1]) {
      grEffEta = makeEffGraph(etaBinEdgesv, passTreeEtav, failTreeEtav, method, "eta", massLo, massHi, format, doAbsEta);
      grEffEta->SetName("grEffEta");
      CPlot plotEffEta("effeta","","probe #eta","#varepsilon");
      if(doAbsEta) plotEffEta.SetXTitle("probe |#eta|");
      plotEffEta.AddGraph(grEffEta,"",kBlack);
      plotEffEta.SetYRange(0.9,1.05);
      plotEffEta.Draw(c,kTRUE,format);
    
      CPlot plotEffEta2("effeta2","","probe #eta","#varepsilon");
      if(doAbsEta) plotEffEta2.SetXTitle("probe |#eta|");
      plotEffEta2.AddGraph(grEffEta,"",kBlack);
      plotEffEta2.SetYRange(ylow,yhigh);
      plotEffEta2.Draw(c,kTRUE,format);    
    }
    
    // efficiency in phi
    if(opts[2]) {
      grEffPhi = makeEffGraph(phiBinEdgesv, passTreePhiv, failTreePhiv, method, "phi", massLo, massHi, format, doAbsEta);
      grEffPhi->SetName("grEffPhi");
      CPlot plotEffPhi("effphi","","probe #phi","#varepsilon");
      plotEffPhi.AddGraph(grEffPhi,"",kBlack);
      plotEffPhi.SetYRange(ylow,yhigh);
      plotEffPhi.Draw(c,kTRUE,format);   
    }

    // efficiency in N_PV
    if(opts[3]) {
      grEffNPV = makeEffGraph(npvBinEdgesv, passTreeNPVv, failTreeNPVv, method, "npv", massLo, massHi, format, doAbsEta);
      grEffNPV->SetName("grEffNPV");
      CPlot plotEffNPV("effnpv","","N_{PV}","#varepsilon");
      plotEffNPV.AddGraph(grEffNPV,"",kBlack);
      plotEffNPV.SetYRange(ylow,yhigh);
      plotEffNPV.Draw(c,kTRUE,format);   
    }

    // efficiency in Rho
    if(opts[4]) {
      grEffRho = makeEffGraph(rhoBinEdgesv, passTreeRhov, failTreeRhov, method, "rho", massLo, massHi, format, doAbsEta);
      grEffRho->SetName("grEffRho");
      CPlot plotEffRho("effrho","","#rho (Energy Density) [GeV]","#varepsilon");
      plotEffRho.AddGraph(grEffRho,"",kBlack);
      plotEffRho.SetYRange(ylow,yhigh);
      plotEffRho.Draw(c,kTRUE,format);   
    }
        
    gStyle->SetPalette(1);
    c->SetRightMargin(0.15);
    c->SetLeftMargin(0.15);
    
    //
    // eta-pT efficiency maps
    //
    if(opts[5]) {
      makeEffHist2D(hEffEtaPt, hErrlEtaPt, hErrhEtaPt, passTreeEtaPtv, failTreeEtaPtv, method, "etapt", massLo, massHi, format, doAbsEta);
      hEffEtaPt->SetTitleOffset(1.2,"Y");
      if(ptNbins>2)
        hEffEtaPt->GetYaxis()->SetRangeUser(ptBinEdgesv[0],ptBinEdgesv[ptNbins-2]);
      CPlot plotEffEtaPt("effetapt","","probe #eta","probe p_{T} [GeV/c]");
      if(doAbsEta) plotEffEtaPt.SetXTitle("probe |#eta|");
      plotEffEtaPt.AddHist2D(hEffEtaPt,"COLZ,text");
      plotEffEtaPt.Draw(c,kTRUE,format);    

      hErrlEtaPt->SetTitleOffset(1.2,"Y");
      if(ptNbins>2)
        hErrlEtaPt->GetYaxis()->SetRangeUser(ptBinEdgesv[0],ptBinEdgesv[ptNbins-2]);
      CPlot plotErrlEtaPt("errletapt","","probe #eta","probe p_{T} [GeV/c]");
      if(doAbsEta) plotErrlEtaPt.SetXTitle("probe |#eta|");
      plotErrlEtaPt.AddHist2D(hErrlEtaPt,"COLZ,text");
      plotErrlEtaPt.Draw(c,kTRUE,format);
  
      hErrhEtaPt->SetTitleOffset(1.2,"Y");
      if(ptNbins>2)
        hErrhEtaPt->GetYaxis()->SetRangeUser(ptBinEdgesv[0],ptBinEdgesv[ptNbins-2]);
      CPlot plotErrhEtaPt("errhetapt","","probe #eta","probe p_{T} [GeV/c]");
      if(doAbsEta) plotErrhEtaPt.SetXTitle("probe |#eta|");
      plotErrhEtaPt.AddHist2D(hErrhEtaPt,"COLZ,text");
      plotErrhEtaPt.Draw(c,kTRUE,format);
    }
    
    //
    // eta-phi efficiency maps
    //
    if(opts[6]) {
      makeEffHist2D(hEffEtaPhi, hErrlEtaPhi, hErrhEtaPhi, passTreeEtaPhiv, failTreeEtaPhiv, method, "etaphi", massLo, massHi, format, doAbsEta);
      hEffEtaPhi->SetTitleOffset(1.2,"Y");
      CPlot plotEffEtaPhi("effetaphi","","probe #eta","probe #phi");
      if(doAbsEta) plotEffEtaPhi.SetXTitle("probe |#eta|");
      plotEffEtaPhi.AddHist2D(hEffEtaPhi,"COLZ");
      plotEffEtaPhi.Draw(c,kTRUE,format);   

      hErrlEtaPhi->SetTitleOffset(1.2,"Y");
      CPlot plotErrlEtaPhi("errletaphi","","probe #eta","probe #phi");
      plotErrlEtaPhi.AddHist2D(hErrlEtaPhi,"COLZ");
      if(doAbsEta) plotErrlEtaPhi.SetXTitle("probe |#eta|");
      plotErrlEtaPhi.Draw(c,kTRUE,format);

      hErrhEtaPhi->SetTitleOffset(1.2,"Y");
      CPlot plotErrhEtaPhi("errhetaphi","","probe #eta","probe #phi");
      if(doAbsEta) plotErrhEtaPhi.SetXTitle("probe |#eta|");
      plotErrhEtaPhi.AddHist2D(hErrhEtaPhi,"COLZ");
      plotErrhEtaPhi.Draw(c,kTRUE,format);
    }
       
  } else {
    
    // efficiency in pT
    if(opts[0]) {
      grEffPt = makeEffGraph(ptBinEdgesv, passTreePtv, failTreePtv, sigModPass, bkgModPass, sigModFail, bkgModFail, "pt", massLo, massHi, format, doAbsEta);
      grEffPt->SetName("grEffPt");
      CPlot plotEffPt("effpt","","probe p_{T} [GeV/c]","#varepsilon");
      plotEffPt.AddGraph(grEffPt,"",kBlack);
      //plotEffPt.SetYRange(ylow,yhigh);
      plotEffPt.SetYRange(0.55,1.05);
      plotEffPt.SetXRange(0.9*(ptBinEdgesv[0]),1.1*(ptBinEdgesv[ptNbins-1]));
      plotEffPt.Draw(c,kTRUE,format);
    }
        
    // efficiency in eta
    if(opts[1]) {
      grEffEta = makeEffGraph(etaBinEdgesv, passTreeEtav, failTreeEtav, sigModPass, bkgModPass, sigModFail, bkgModFail, "eta", massLo, massHi, format, doAbsEta);
      grEffEta->SetName("grEffEta");
      CPlot plotEffEta("effeta","","probe #eta","#varepsilon");
      if(doAbsEta) plotEffEta.SetXTitle("probe |#eta|");
      plotEffEta.AddGraph(grEffEta,"",kBlack);
      plotEffEta.SetYRange(0.9,1.05);
      plotEffEta.Draw(c,kTRUE,format);
    
      CPlot plotEffEta2("effeta2","","probe #eta","#varepsilon");
      if(doAbsEta) plotEffEta2.SetXTitle("probe |#eta|");
      plotEffEta2.AddGraph(grEffEta,"",kBlack);
      plotEffEta2.SetYRange(ylow,yhigh);
      plotEffEta2.Draw(c,kTRUE,format);
    }    
    
    // efficiency in phi
    if(opts[2]) {
      grEffPhi = makeEffGraph(phiBinEdgesv, passTreePhiv, failTreePhiv, sigModPass, bkgModPass, sigModFail, bkgModFail, "phi", massLo, massHi, format, doAbsEta);
      grEffPhi->SetName("grEffPhi");
      CPlot plotEffPhi("effphi","","probe #phi","#varepsilon");
      plotEffPhi.AddGraph(grEffPhi,"",kBlack);
      plotEffPhi.SetYRange(ylow,yhigh);
      plotEffPhi.Draw(c,kTRUE,format);
    }
    
    // efficiency in N_PV
    if(opts[3]) {
      grEffNPV = makeEffGraph(npvBinEdgesv, passTreeNPVv, failTreeNPVv, sigModPass, bkgModPass, sigModFail, bkgModFail, "npv", massLo, massHi, format, doAbsEta);
      grEffNPV->SetName("grEffNPV");
      CPlot plotEffNPV("effnpv","","N_{PV}","#varepsilon");
      plotEffNPV.AddGraph(grEffNPV,"",kBlack);
      //plotEffNPV.SetYRange(ylow,yhigh);
      plotEffNPV.SetYRange(0.50,1.05);
      plotEffNPV.Draw(c,kTRUE,format);
    }
    
    // efficiency in Rho
    if(opts[4]) {
      grEffRho = makeEffGraph(rhoBinEdgesv, passTreeRhov, failTreeRhov, sigModPass, bkgModPass, sigModFail, bkgModFail, "rho", massLo, massHi, format, doAbsEta);
      grEffRho->SetName("grEffRho");
      CPlot plotEffRho("effrho","","#rho (Energy Density) [GeV]","#varepsilon");
      plotEffRho.AddGraph(grEffRho,"",kBlack);
      plotEffRho.SetYRange(ylow,yhigh);
      plotEffRho.Draw(c,kTRUE,format);
    }
    
            
    gStyle->SetPalette(1);
    c->SetRightMargin(0.15);
    c->SetLeftMargin(0.15);
    
    //
    // eta-pT efficiency maps
    //
    if(opts[5]) {
      makeEffHist2D(hEffEtaPt, hErrlEtaPt, hErrhEtaPt, passTreeEtaPtv, failTreeEtaPtv, sigModPass, bkgModPass, sigModFail, bkgModFail, "etapt", massLo, massHi, format, doAbsEta);
      hEffEtaPt->SetTitleOffset(1.2,"Y");
      hEffEtaPt->GetYaxis()->SetRangeUser(ptBinEdgesv[0],ptBinEdgesv[ptNbins-2]);
      CPlot plotEffEtaPt("effetapt","","probe #eta","probe p_{T} [GeV/c]");
      if(doAbsEta) plotEffEtaPt.SetXTitle("probe |#eta|");
      plotEffEtaPt.AddHist2D(hEffEtaPt,"COLZ");
      plotEffEtaPt.Draw(c,kTRUE,format);    

      hErrlEtaPt->SetTitleOffset(1.2,"Y");
      hErrlEtaPt->GetYaxis()->SetRangeUser(ptBinEdgesv[0],ptBinEdgesv[ptNbins-2]);
      CPlot plotErrlEtaPt("errletapt","","probe #eta","probe p_{T} [GeV/c]");
      if(doAbsEta) plotErrlEtaPt.SetXTitle("probe |#eta|");
      plotErrlEtaPt.AddHist2D(hErrlEtaPt,"COLZ");
      plotErrlEtaPt.Draw(c,kTRUE,format);
  
      hErrhEtaPt->SetTitleOffset(1.2,"Y");
      hErrhEtaPt->GetYaxis()->SetRangeUser(ptBinEdgesv[0],ptBinEdgesv[ptNbins-2]);
      CPlot plotErrhEtaPt("errhetapt","","probe #eta","probe p_{T} [GeV/c]");
      if(doAbsEta) plotErrhEtaPt.SetXTitle("probe |#eta|");
      plotErrhEtaPt.AddHist2D(hErrhEtaPt,"COLZ");
      plotErrhEtaPt.Draw(c,kTRUE,format);
    }
    
    //
    // eta-phi efficiency maps
    //
    if(opts[6]) {
      makeEffHist2D(hEffEtaPhi, hErrlEtaPhi, hErrhEtaPhi, passTreeEtaPhiv, failTreeEtaPhiv, sigModPass, bkgModPass, sigModFail, bkgModFail, "etaphi", massLo, massHi, format, doAbsEta);
      hEffEtaPhi->SetTitleOffset(1.2,"Y");
      CPlot plotEffEtaPhi("effetaphi","","probe #eta","probe #phi");
      if(doAbsEta) plotEffEtaPhi.SetXTitle("probe |#eta|");
      plotEffEtaPhi.AddHist2D(hEffEtaPhi,"COLZ");
      plotEffEtaPhi.Draw(c,kTRUE,format);    

      hErrlEtaPhi->SetTitleOffset(1.2,"Y");
      CPlot plotErrlEtaPhi("errletaphi","","probe #eta","probe #phi");
      plotErrlEtaPhi.AddHist2D(hErrlEtaPhi,"COLZ");
      if(doAbsEta) plotErrlEtaPhi.SetXTitle("probe |#eta|");
      plotErrlEtaPhi.Draw(c,kTRUE,format);
  
      hErrhEtaPhi->SetTitleOffset(1.2,"Y");
      CPlot plotErrhEtaPhi("errhetaphi","","probe #eta","probe #phi");
      if(doAbsEta) plotErrhEtaPhi.SetXTitle("probe |#eta|");
      plotErrhEtaPhi.AddHist2D(hErrhEtaPhi,"COLZ");
      plotErrhEtaPhi.Draw(c,kTRUE,format);
    }
  }

  // Undo scaling of axes before saving to file
  hEffEtaPt->GetYaxis()->SetRangeUser(ptBinEdgesv[0],ptBinEdgesv[ptNbins]);
  hErrlEtaPt->GetYaxis()->SetRangeUser(ptBinEdgesv[0],ptBinEdgesv[ptNbins]);
  hErrhEtaPt->GetYaxis()->SetRangeUser(ptBinEdgesv[0],ptBinEdgesv[ptNbins]);


  //--------------------------------------------------------------------------------------------------------------
  // Output
  //==============================================================================================================
   
  cout << "*" << endl;
  cout << "* SUMMARY" << endl;
  cout << "*--------------------------------------------------" << endl;
  cout << endl;
  
  TFile *outfile = new TFile(outputDir + TString("/eff.root"), "RECREATE");
  if(grEffPt)  grEffPt->Write();
  if(grEffEta) grEffEta->Write();
  if(grEffPhi) grEffPhi->Write();
  if(grEffNPV) grEffNPV->Write();
  if(grEffRho) grEffRho->Write();
  hEffEtaPt->Write();
  hErrlEtaPt->Write();
  hErrhEtaPt->Write();
  hEffEtaPhi->Write();
  hErrlEtaPhi->Write();
  hErrhEtaPhi->Write();
  outfile->Close();
  delete outfile;
  
  makeHTML(outputDir);
  makeHTML(outputDir, "pt", ptNbins);
  makeHTML(outputDir, "eta", etaNbins);
  makeHTML(outputDir, "phi", phiNbins);
  makeHTML(outputDir, "npv", npvNbins);  
  makeHTML(outputDir, "rho", rhoNbins);     
  makeHTML(outputDir, "etapt", etaNbins*ptNbins);
  makeHTML(outputDir, "etaphi", etaNbins*phiNbins); 

  ofstream txtfile;
  char txtfname[100];    
  sprintf(txtfname,"%s/summary.txt",outputDir.Data());
  txtfile.open(txtfname);
  assert(txtfile.is_open());

  CEffUser2D effetapt;
  CEffUser2D effetaphi;
 
  CEffUser1D effpt;
  CEffUser1D effeta;
  CEffUser1D effphi;
  CEffUser1D effnpv;
  CEffUser1D effrho;
  
  if(hEffEtaPt->GetEntries()>0) {
    effetapt.loadEff(hEffEtaPt,hErrlEtaPt,hErrhEtaPt);
    effetapt.printEff(txtfile);     txtfile << endl;
    effetapt.printErrLow(txtfile);  txtfile << endl;
    effetapt.printErrHigh(txtfile); txtfile << endl;
    txtfile << endl;
  }
  
  if(hEffEtaPhi->GetEntries()>0) {
    effetaphi.loadEff(hEffEtaPhi,hErrlEtaPhi,hErrhEtaPhi);
    effetaphi.printEff(txtfile);     txtfile << endl;
    effetaphi.printErrLow(txtfile);  txtfile << endl;
    effetaphi.printErrHigh(txtfile); txtfile << endl;
    txtfile << endl;
  }
  
  if(grEffPt) {
    effpt.loadEff(grEffPt);
    effpt.printEff(txtfile);
    txtfile << endl;
    effpt.printErrLow(txtfile);
    txtfile << endl;
    effpt.printErrHigh(txtfile);
    txtfile << endl;
    txtfile << endl;
  }
  
  if(grEffEta) {
    effeta.loadEff(grEffEta);
    effeta.printEff(txtfile);
    txtfile << endl;
    effeta.printErrLow(txtfile);
    txtfile << endl;
    effeta.printErrHigh(txtfile);
    txtfile << endl;
    txtfile << endl;
  }
  
  if(grEffPhi) {
    effphi.loadEff(grEffPhi);
    effphi.printEff(txtfile);
    txtfile << endl;
    effphi.printErrLow(txtfile);
    txtfile << endl;
    effphi.printErrHigh(txtfile);
  }
  
  if(grEffNPV) {
    effnpv.loadEff(grEffNPV);
    effnpv.printEff(txtfile);
    txtfile << endl;
    effnpv.printErrLow(txtfile);
    txtfile << endl;
    effnpv.printErrHigh(txtfile);
  }

  if(grEffRho) {
    effrho.loadEff(grEffRho);
    effrho.printEff(txtfile);
    txtfile << endl;
    effrho.printErrLow(txtfile);
    txtfile << endl;
    effrho.printErrHigh(txtfile);
  }
  txtfile.close();
  
  cout << endl;
  cout << "  <> Output saved in " << outputDir << "/" << endl;    
  cout << endl;  
      
  gBenchmark->Show("plotEff"); 
}  


//=== FUNCTION DEFINITIONS ======================================================================================

//--------------------------------------------------------------------------------------------------
void makeHTML(const TString outDir)
{

  ofstream htmlfile;
  htmlfile.open(Form("%s/plots.html",outDir.Data()));

  htmlfile << "<!DOCTYPE html" << endl;
  htmlfile << "    PUBLIC \"-//W3C//DTD HTML 3.2//EN\">" << endl;
  htmlfile << "<html>" << endl;

  htmlfile << "<body bgcolor=\"EEEEEE\">" << endl;

  htmlfile << "<table border=\"0\" cellspacing=\"5\" width=\"100%\">" << endl;  
  htmlfile << "<tr>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"plots/effpt.png\"><img src=\"plots/effpt.png\" alt=\"plots/effpt.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"plots/effeta.png\"><img src=\"plots/effeta.png\" alt=\"plots/effeta.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"plots/effeta2.png\"><img src=\"plots/effeta2.png\" alt=\"plots/effeta2.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"plots/effphi.png\"><img src=\"plots/effphi.png\" alt=\"plots/effphi.png\" width=\"100%\"></a></td>" << endl;  
  htmlfile << "</tr>" << endl;
  htmlfile << "<tr>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"pt.html\">pT bins</a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"eta.html\">&eta; bins</a></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"phi.html\">&phi; bins</a></td>" << endl;  
  htmlfile << "</tr>" << endl;    
  htmlfile << "<tr>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"plots/effetapt.png\"><img src=\"plots/effetapt.png\" alt=\"plots/effetapt.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"plots/errletapt.png\"><img src=\"plots/errletapt.png\" alt=\"plots/errletapt.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"plots/errhetapt.png\"><img src=\"plots/errhetapt.png\" alt=\"plots/errhetapt.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "</tr>" << endl;    
  htmlfile << "<tr>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"etapt.html\">&eta;-pT bins</a></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "</tr>" << endl;
  htmlfile << "<tr>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"plots/effetaphi.png\"><img src=\"plots/effetaphi.png\" alt=\"plots/effetaphi.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"plots/errletaphi.png\"><img src=\"plots/errletaphi.png\" alt=\"plots/errletaphi.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"plots/errhetaphi.png\"><img src=\"plots/errhetaphi.png\" alt=\"plots/errhetaphi.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "</tr>" << endl;    
  htmlfile << "<tr>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"etaphi.html\">&eta;-&phi; bins</a></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "</tr>" << endl; 
  htmlfile << "<tr>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"plots/effnpv.png\"><img src=\"plots/effnpv.png\" alt=\"plots/effnpv.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;  
  htmlfile << "</tr>" << endl;
  htmlfile << "<tr>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"npv.html\">N_PV bins</a></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;  
  htmlfile << "</tr>" << endl; 
  htmlfile << "<tr>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"plots/effrho.png\"><img src=\"plots/effrho.png\" alt=\"plots/effrho.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;  
  htmlfile << "</tr>" << endl;
  htmlfile << "<tr>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"rho.html\">Rho bins</a></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;  
  htmlfile << "</tr>" << endl; 
  htmlfile << "</table>" << endl;
  htmlfile << "<hr />" << endl;
    
  htmlfile << "</body>" << endl;
  htmlfile << "</html>" << endl;
  htmlfile.close(); 
}

void makeHTML(const TString outDir, const TString name, const Int_t n)
{
  ofstream htmlfile;
  htmlfile.open(Form("%s/%s.html",outDir.Data(),name.Data()));

  htmlfile << "<!DOCTYPE html" << endl;
  htmlfile << "    PUBLIC \"-//W3C//DTD HTML 3.2//EN\">" << endl;
  htmlfile << "<html>" << endl;

  htmlfile << "<body bgcolor=\"EEEEEE\">" << endl;

  htmlfile << "<table border=\"0\" cellspacing=\"5\" width=\"100%\">" << endl;    
  Int_t i;
  for(i=0; i<n; i++) {
    if(i%2==0) htmlfile << "<tr>" << endl;    
    htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"plots/pass" << name << "_" << i << ".png\"><img src=\"plots/pass" << name << "_" << i << ".png\"alt=\"plots/pass" << name << "_" << i << ".png\" width=\"100%\"></a></td>" << endl;
    htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"plots/fail" << name << "_" << i << ".png\"><img src=\"plots/fail" << name << "_" << i << ".png\"alt=\"plots/fail" << name << "_" << i << ".png\" width=\"100%\"></a></td>" << endl;
    if(i%2) htmlfile << "</tr>" << endl;
  }
  if(i%2) {
    htmlfile << "<td width=\"25%\"></td>" << endl;
    htmlfile << "<td width=\"25%\"></td>" << endl;
    htmlfile << "</tr>" << endl;
  }
  htmlfile << "</table>" << endl;
  htmlfile << "<hr />" << endl;
    
  htmlfile << "</body>" << endl;
  htmlfile << "</html>" << endl;
  htmlfile.close(); 
}

//--------------------------------------------------------------------------------------------------
TGraphAsymmErrors* makeEffGraph(const vector<Double_t> &edgesv, const vector<TTree*> &passv, const vector<TTree*> &failv, const Int_t method, 
				const TString name, const Double_t massLo, const Double_t massHi, const TString format, const Bool_t doAbsEta)
{
  const UInt_t n = edgesv.size()-1;
  Double_t xval[n], xerr[n];
  Double_t yval[n], yerrl[n], yerrh[n];
  
  TCanvas *cpass = MakeCanvas("cpass","cpass",720,540);
  cpass->SetWindowPosition(cpass->GetWindowTopX()+cpass->GetBorderSize()+800,0);
  TCanvas *cfail = MakeCanvas("cfail","cfail",720,540); 
  cfail->SetWindowPosition(cfail->GetWindowTopX()+cfail->GetBorderSize()+800,cpass->GetWindowTopX()+cfail->GetBorderSize()+540);
    
  for(UInt_t ibin=0; ibin<n; ibin++) {
    xval[ibin] = 0.5*(edgesv[ibin+1] + edgesv[ibin]);
    xerr[ibin] = 0.5*(edgesv[ibin+1] - edgesv[ibin]);
    
    Double_t eff, errl, errh;
    performCount(eff, errl, errh, ibin, edgesv[ibin], edgesv[ibin+1], 0, 0,
	         passv[ibin], failv[ibin], method, 
	         name, massLo, massHi, format, doAbsEta,
	         cpass, cfail);
    
    yval[ibin]  = eff;
    yerrl[ibin] = errl;
    yerrh[ibin] = errh;
  }
  delete cpass;
  delete cfail;
  
  return new TGraphAsymmErrors(n,xval,yval,xerr,xerr,yerrl,yerrh);
}

TGraphAsymmErrors* makeEffGraph(const vector<Double_t> &edgesv, const vector<TTree*> &passv, const vector<TTree*> &failv,
                                const Int_t sigpass, const Int_t bkgpass, const Int_t sigfail, const Int_t bkgfail, 
		                const TString name, const Double_t massLo, const Double_t massHi, const TString format, const Bool_t doAbsEta)
{

  const UInt_t n = edgesv.size()-1;
  Double_t xval[n], xerr[n];
  Double_t yval[n], yerrl[n], yerrh[n];
  
  TCanvas *cpass = MakeCanvas("cpass","cpass",720,540);
  cpass->SetWindowPosition(cpass->GetWindowTopX()+cpass->GetBorderSize()+800,0);
  TCanvas *cfail = MakeCanvas("cfail","cfail",720,540); 
  cfail->SetWindowPosition(cfail->GetWindowTopX()+cfail->GetBorderSize()+800,cpass->GetWindowTopX()+cfail->GetBorderSize()+540);

  for(UInt_t ibin=0; ibin<n; ibin++) {
    xval[ibin] = 0.5*(edgesv[ibin+1] + edgesv[ibin]);
    xerr[ibin] = 0.5*(edgesv[ibin+1] - edgesv[ibin]);

    Double_t eff, errl, errh;


      ifstream inf(Form("%s/fitres%s_%i.txt",CPlot::sOutDir.Data(),name.Data(),ibin));
      if (inf.is_open()) {
        cout << "Bin " << ibin << " : Opened input file " << Form("%s/fitres%s_%i.txt",CPlot::sOutDir.Data(),name.Data(),ibin)
             << " to load saved efficiency information.\n";
        string line;
        Int_t state=0;
        while(getline(inf,line)) {
          stringstream ss1(line);
          string tmp;
          ss1 >> tmp;
          if (tmp == "eff") {
            string tmp1;
            ss1 >> tmp1;
            ss1 >> eff;
            ss1 >> tmp1;
            ss1 >> errl;
            errh = errl;
            break;
          } 
        }
        cout << "Loaded Efficiency: " << eff << " +/- " << errl << endl;
      } else {
        cout << "Bin " << ibin << " : No saved information. Do the FIT.\n";
        //If fit result doesn't exist, do the fit
        performFit(eff, errl, errh, ibin, edgesv[ibin], edgesv[ibin+1], 0, 0,
                   passv[ibin], failv[ibin],
                   sigpass, bkgpass, sigfail, bkgfail, 
                   name, massLo, massHi, format, doAbsEta,
                   cpass, cfail);
      }



    
    yval[ibin]  = eff;
    yerrl[ibin] = errl;
    yerrh[ibin] = errh; 
  }
  delete cpass;
  delete cfail;    
  
  return new TGraphAsymmErrors(n,xval,yval,xerr,xerr,yerrl,yerrh);
}

//--------------------------------------------------------------------------------------------------
// Counting
//--------------------------------------------------------------------------------------------------
void makeEffHist2D(TH2D *hEff, TH2D *hErrl, TH2D *hErrh, const vector<TTree*> &passv, const vector<TTree*> &failv, const Int_t method,
                   const TString name, const Double_t massLo, const Double_t massHi, const TString format, const Bool_t doAbsEta)
{
  TCanvas *cpass = MakeCanvas("cpass","cpass",720,540);
  cpass->SetWindowPosition(cpass->GetWindowTopX()+cpass->GetBorderSize()+800,0);
  TCanvas *cfail = MakeCanvas("cfail","cfail",720,540); 
  cfail->SetWindowPosition(cfail->GetWindowTopX()+cfail->GetBorderSize()+800,cpass->GetWindowTopX()+cfail->GetBorderSize()+540);
    
  for(Int_t iy=0; iy<hEff->GetNbinsY(); iy++) {
    for(Int_t ix=0; ix<hEff->GetNbinsX(); ix++) {
      Int_t ibin = iy*(hEff->GetNbinsX()) + ix;
      
      Double_t eff, errl, errh;
      performCount(eff, errl, errh, ibin, 
                   hEff->GetXaxis()->GetBinLowEdge(ix+1), hEff->GetXaxis()->GetBinLowEdge(ix+2),
		   hEff->GetYaxis()->GetBinLowEdge(iy+1), hEff->GetYaxis()->GetBinLowEdge(iy+2),
		   passv[ibin], failv[ibin], method, 
		   name, massLo, massHi, format, doAbsEta,
		   cpass, cfail);
      
      hEff ->SetBinContent(ix+1, iy+1, eff);
      hErrl->SetBinContent(ix+1, iy+1, errl);
      hErrh->SetBinContent(ix+1, iy+1, errh);
    }    
  }  
  delete cpass;
  delete cfail;  
}

//--------------------------------------------------------------------------------------------------
// Fitting
//--------------------------------------------------------------------------------------------------
void makeEffHist2D(TH2D *hEff, TH2D *hErrl, TH2D *hErrh, const vector<TTree*> &passv, const vector<TTree*> &failv, 
                   const Int_t sigpass, const Int_t bkgpass, const Int_t sigfail, const Int_t bkgfail, 
		   const TString name, const Double_t massLo, const Double_t massHi, const TString format, const Bool_t doAbsEta)
{  
  TCanvas *cpass = MakeCanvas("cpass","cpass",720,540);
  cpass->SetWindowPosition(cpass->GetWindowTopX()+cpass->GetBorderSize()+800,0);
  TCanvas *cfail = MakeCanvas("cfail","cfail",720,540); 
  cfail->SetWindowPosition(cfail->GetWindowTopX()+cfail->GetBorderSize()+800,cpass->GetWindowTopX()+cfail->GetBorderSize()+540);
  
  cout << "nbins: " << hEff->GetNbinsY() << " " << hEff->GetNbinsX() << endl;

  for(Int_t iy=0; iy<hEff->GetNbinsY(); iy++) {
    for(Int_t ix=0; ix<hEff->GetNbinsX(); ix++) {
      Int_t ibin = iy*(hEff->GetNbinsX()) + ix;

      Double_t eff, errl, errh;

      ifstream inf(Form("%s/fitres%s_%i.txt",CPlot::sOutDir.Data(),name.Data(),ibin));
      if (inf.is_open()) {
        cout << "Bin " << ibin << " : Opened input file " << Form("%s/fitres%s_%i.txt",CPlot::sOutDir.Data(),name.Data(),ibin)
             << " to load saved efficiency information.\n";
        string line;
        Int_t state=0;
        while(getline(inf,line)) {
          stringstream ss1(line);
          string tmp;
          ss1 >> tmp;
          if (tmp == "eff") {
            string tmp1;
            ss1 >> tmp1;
            ss1 >> eff;
            ss1 >> tmp1;
            ss1 >> errl;
            errh = errl;
            break;
          }  
        }
        cout << "Loaded Efficiency: " << eff << " +/- " << errl << endl;
      } else {
        cout << "Bin " << ibin << " : No saved information. Do the FIT.\n";
        //If fit result doesn't exist, do the fit
        performFit(eff, errl, errh, ibin, 
                   hEff->GetXaxis()->GetBinLowEdge(ix+1), hEff->GetXaxis()->GetBinLowEdge(ix+2),
                   hEff->GetYaxis()->GetBinLowEdge(iy+1), hEff->GetYaxis()->GetBinLowEdge(iy+2),
                   passv[ibin], failv[ibin],
                   sigpass, bkgpass, sigfail, bkgfail, 
                   name, massLo, massHi, format, doAbsEta,
                   cpass, cfail);
      }

      inf.close();
      hEff ->SetBinContent(ix+1, iy+1, eff);
      hErrl->SetBinContent(ix+1, iy+1, errl);
      hErrh->SetBinContent(ix+1, iy+1, errh);
    }  
  }  
  delete cpass;
  delete cfail;
}

//--------------------------------------------------------------------------------------------------
void generateHistTemplates(const TString infilename,
                           const TString outfilename,
                           TH1F *puWeights, 
                           const vector<Double_t> &ptEdgesv, const vector<Double_t> &etaEdgesv, const vector<Double_t> &phiEdgesv, 
                           const vector<Double_t> &npvEdgesv, const vector<Double_t> &rhoEdgesv,
		           const Double_t massLo, const Double_t massHi, const Bool_t doAbsEta, const Int_t charge, Bool_t collapseEtaBins)
{
  cout << "Creating histogram templates... "; cout.flush();

  char hname[50];
  
  const Double_t binsize = 2;
  //use these bigger bins for the ZeeGamma fit
  //   const Double_t binsize = 5;
  
  const UInt_t ptNbins  = ptEdgesv.size()-1;
  const UInt_t etaNbins = etaEdgesv.size()-1;
  const UInt_t phiNbins = phiEdgesv.size()-1;
  const UInt_t npvNbins = npvEdgesv.size()-1;
  const UInt_t rhoNbins = rhoEdgesv.size()-1;
  
  TH1D* passPt[ptNbins];
  TH1D* failPt[ptNbins];
  for(UInt_t ibin=0; ibin<ptNbins; ibin++) {
    sprintf(hname,"passpt_%i",ibin);
    passPt[ibin] = new TH1D(hname,"",Int_t((massHi-massLo)/binsize),massLo,massHi);
    passPt[ibin]->SetDirectory(0);
    sprintf(hname,"failpt_%i",ibin);
    failPt[ibin] = new TH1D(hname,"",Int_t((massHi-massLo)/binsize),massLo,massHi);
    failPt[ibin]->SetDirectory(0);
  }
  
  TH1D* passEta[etaNbins];
  TH1D* failEta[etaNbins];
  for(UInt_t ibin=0; ibin<etaNbins; ibin++) {
    sprintf(hname,"passeta_%i",ibin);
    passEta[ibin] = new TH1D(hname,"",Int_t((massHi-massLo)/binsize),massLo,massHi);
    passEta[ibin]->SetDirectory(0);
    sprintf(hname,"faileta_%i",ibin);
    failEta[ibin] = new TH1D(hname,"",Int_t((massHi-massLo)/binsize),massLo,massHi);
    failEta[ibin]->SetDirectory(0);
  }
  
  TH1D* passPhi[phiNbins];  
  TH1D* failPhi[phiNbins];
  for(UInt_t ibin=0; ibin<phiNbins; ibin++) {
    sprintf(hname,"passphi_%i",ibin);
    passPhi[ibin] = new TH1D(hname,"",Int_t((massHi-massLo)/binsize),massLo,massHi);
    passPhi[ibin]->SetDirectory(0);
    sprintf(hname,"failphi_%i",ibin);
    failPhi[ibin] = new TH1D(hname,"",Int_t((massHi-massLo)/binsize),massLo,massHi);
    failPhi[ibin]->SetDirectory(0);
  }
  
  TH1D* passEtaPt[etaNbins*ptNbins];  
  TH1D* failEtaPt[etaNbins*ptNbins];
  for(UInt_t ibin=0; ibin<(etaNbins*ptNbins); ibin++) {
    sprintf(hname,"passetapt_%i",ibin);
    passEtaPt[ibin] = new TH1D(hname,"",Int_t((massHi-massLo)/binsize),massLo,massHi);
    passEtaPt[ibin]->SetDirectory(0);
    sprintf(hname,"failetapt_%i",ibin);
    failEtaPt[ibin] = new TH1D(hname,"",Int_t((massHi-massLo)/binsize),massLo,massHi);
    failEtaPt[ibin]->SetDirectory(0);
  }
  
  TH1D* passEtaPhi[etaNbins*phiNbins];
  TH1D* failEtaPhi[etaNbins*phiNbins];
  for(UInt_t ibin=0; ibin<(etaNbins*phiNbins); ibin++) {
    sprintf(hname,"passetaphi_%i",ibin); 
    passEtaPhi[ibin] = new TH1D(hname,"",Int_t((massHi-massLo)/binsize),massLo,massHi);
    passEtaPhi[ibin]->SetDirectory(0);
    sprintf(hname,"failetaphi_%i",ibin);
    failEtaPhi[ibin] = new TH1D(hname,"",Int_t((massHi-massLo)/binsize),massLo,massHi);
    failEtaPhi[ibin]->SetDirectory(0);
  }

  TH1D* passNPV[npvNbins];  
  TH1D* failNPV[npvNbins];
  for(UInt_t ibin=0; ibin<npvNbins; ibin++) {
    sprintf(hname,"passnpv_%i",ibin);
    passNPV[ibin] = new TH1D(hname,"",Int_t((massHi-massLo)/binsize),massLo,massHi);
    passNPV[ibin]->SetDirectory(0);
    sprintf(hname,"failnpv_%i",ibin);
    failNPV[ibin] = new TH1D(hname,"",Int_t((massHi-massLo)/binsize),massLo,massHi);
    failNPV[ibin]->SetDirectory(0);
  }
    
  TH1D* passRho[rhoNbins];  
  TH1D* failRho[rhoNbins];
  for(UInt_t ibin=0; ibin<rhoNbins; ibin++) {
    sprintf(hname,"passrho_%i",ibin);
    passRho[ibin] = new TH1D(hname,"",Int_t((massHi-massLo)/binsize),massLo,massHi);
    passRho[ibin]->SetDirectory(0);
    sprintf(hname,"failrho_%i",ibin);
    failRho[ibin] = new TH1D(hname,"",Int_t((massHi-massLo)/binsize),massLo,massHi);
    failRho[ibin]->SetDirectory(0);
  }
    
  TagAndProbePair *inputData = new TagAndProbePair;
  inputData->LoadTree(infilename);
  
  for(UInt_t ientry=0; ientry< inputData->tree_->GetEntries(); ientry++) {
    inputData->tree_->GetEntry(ientry);
    
    double weight = inputData->weight;
    if (puWeights) {      
      Int_t npuxbin = puWeights->GetXaxis()->FindFixBin(TMath::Min(double(inputData->NPU_0), 49.499));      
      //Int_t npuxbin = puWeights->GetXaxis()->FindFixBin(TMath::Min(double(inputData->NPV), 30.499));
      weight = puWeights->GetBinContent(npuxbin);
    }

    if((inputData->charge)*charge < 0) continue;
    
    Int_t ipt=-1;
    for(UInt_t ibin=0; ibin<ptNbins; ibin++)
      if((inputData->pt >= ptEdgesv[ibin]) && (inputData->pt < ptEdgesv[ibin+1]))
        ipt = ibin;
    if(ipt<0) continue;
    
    Int_t ieta=-1;
    for(UInt_t ibin=0; ibin<etaNbins; ibin++) {
      if(doAbsEta) {
        assert(etaEdgesv[ibin]>=0);
        if((fabs(inputData->eta) >= etaEdgesv[ibin]) && (fabs(inputData->eta) < etaEdgesv[ibin+1]))
          ieta = ibin;
      } else {
        if((inputData->eta >= etaEdgesv[ibin]) && (inputData->eta < etaEdgesv[ibin+1]))
          ieta = ibin;
      }
    }
    if(ieta<0) continue;
	
    Int_t iphi=-1;
    for(UInt_t ibin=0; ibin<phiNbins; ibin++)
      if((inputData->phi >= phiEdgesv[ibin]) && (inputData->phi < phiEdgesv[ibin+1]))
        iphi = ibin;
    if(iphi<0) continue;

    Int_t inpv=-1;
    for(UInt_t ibin=0; ibin<npvNbins; ibin++)
      if((inputData->NPV >= npvEdgesv[ibin]) && (inputData->NPV < npvEdgesv[ibin+1]))
        inpv = ibin;
    if(inpv<0) continue;
        
    Int_t irho=-1;
    for(UInt_t ibin=0; ibin<rhoNbins; ibin++)
      if((inputData->Rho >= rhoEdgesv[ibin]) && (inputData->Rho < rhoEdgesv[ibin+1]))
        irho = ibin;
    if(irho<0) continue;
        
    if(inputData->pass) {
      passPt[ipt]->Fill(inputData->mass,weight);
      passEta[ieta]->Fill(inputData->mass,weight);
      passPhi[iphi]->Fill(inputData->mass,weight);

      if (!collapseEtaBins) {
        passEtaPt[ipt*etaNbins + ieta]->Fill(inputData->mass,weight);
      } 
      else {
      //fill all eta bins
        for(UInt_t ibin=0; ibin<etaNbins; ibin++) {
          passEtaPt[ipt*etaNbins + ibin]->Fill(inputData->mass,weight);
        }
      }

      passEtaPhi[iphi*etaNbins + ieta]->Fill(inputData->mass,weight);
      passNPV[inpv]->Fill(inputData->mass,weight);
      passRho[irho]->Fill(inputData->mass,weight);
    } else {
      failPt[ipt]->Fill(inputData->mass,weight);
      failEta[ieta]->Fill(inputData->mass,weight);
      failPhi[iphi]->Fill(inputData->mass,weight);

       if (!collapseEtaBins) {
         failEtaPt[ipt*etaNbins + ieta]->Fill(inputData->mass,weight);
       } 
       else {
         //fill all eta bins
         for(UInt_t ibin=0; ibin<etaNbins; ibin++) {        
           failEtaPt[ipt*etaNbins + ibin]->Fill(inputData->mass,weight);
         }
       }

       failEtaPhi[iphi*etaNbins + ieta]->Fill(inputData->mass,weight);
       failNPV[inpv]->Fill(inputData->mass,weight);
       failRho[irho]->Fill(inputData->mass,weight);
    }    
  }
 
  TFile outfile(Form("%s/%s",CPlot::sOutDir.Data(),outfilename.Data()), "RECREATE");
  cout << "Output Template file: " << Form("%s/%s",CPlot::sOutDir.Data(),outfilename.Data()) << endl;
  for(UInt_t ibin=0; ibin<ptNbins; ibin++) {
    passPt[ibin]->Write();
    failPt[ibin]->Write();
    delete passPt[ibin];
    delete failPt[ibin];
  }
  for(UInt_t ibin=0; ibin<etaNbins; ibin++) { 
    passEta[ibin]->Write();
    failEta[ibin]->Write();
    delete passEta[ibin];
    delete failEta[ibin];
  }
  for(UInt_t ibin=0; ibin<phiNbins; ibin++) {
    passPhi[ibin]->Write();
    failPhi[ibin]->Write();
    delete passPhi[ibin];
    delete failPhi[ibin];
  }
  for(UInt_t ibin=0; ibin<(etaNbins*ptNbins); ibin++) {
    passEtaPt[ibin]->Write();
    failEtaPt[ibin]->Write();
    delete passEtaPt[ibin];
    delete failEtaPt[ibin];
  }
  for(UInt_t ibin=0; ibin<(etaNbins*phiNbins); ibin++) {
    passEtaPhi[ibin]->Write();
    failEtaPhi[ibin]->Write();
    delete passEtaPhi[ibin];
    delete failEtaPhi[ibin];
  }
  for(UInt_t ibin=0; ibin<npvNbins; ibin++) {
    passNPV[ibin]->Write();
    failNPV[ibin]->Write();
    delete passNPV[ibin];
    delete failNPV[ibin];
  }
  for(UInt_t ibin=0; ibin<rhoNbins; ibin++) {
    passRho[ibin]->Write();
    failRho[ibin]->Write();
    delete passRho[ibin];
    delete failRho[ibin];
  }
  outfile.Write();
  outfile.Close(); 

  cout << "Done!" << endl;
}

//--------------------------------------------------------------------------------------------------
void generateDataTemplates(const TString infilename,
                           const vector<Double_t> &ptEdgesv, const vector<Double_t> &etaEdgesv, const vector<Double_t> &phiEdgesv, 
                           const vector<Double_t> &npvEdgesv, const vector<Double_t> &rhoEdgesv,
		           const Double_t massLo, const Double_t massHi, const Bool_t doAbsEta, const Int_t charge)
{
  cout << "Creating data templates... "; cout.flush();

  char tname[50];
  
  const UInt_t ptNbins  = ptEdgesv.size()-1;
  const UInt_t etaNbins = etaEdgesv.size()-1;
  const UInt_t phiNbins = phiEdgesv.size()-1;
  const UInt_t npvNbins = npvEdgesv.size()-1;
  const UInt_t rhoNbins = rhoEdgesv.size()-1;
  
  Float_t mass;
  
  TTree* passPt[ptNbins];
  TTree* failPt[ptNbins];
  for(UInt_t ibin=0; ibin<ptNbins; ibin++) {
    sprintf(tname,"passpt_%i",ibin);
    passPt[ibin] = new TTree(tname,"");
    passPt[ibin]->Branch("m",&mass,"m/F");
    passPt[ibin]->SetDirectory(0);    
    sprintf(tname,"failpt_%i",ibin);
    failPt[ibin] = new TTree(tname,"");
    failPt[ibin]->Branch("m",&mass,"m/F");
    failPt[ibin]->SetDirectory(0);
  }
  
  TTree* passEta[etaNbins];
  TTree* failEta[etaNbins];
  for(UInt_t ibin=0; ibin<etaNbins; ibin++) {
    sprintf(tname,"passeta_%i",ibin);
    passEta[ibin] = new TTree(tname,"");
    passEta[ibin]->Branch("m",&mass,"m/F");
    passEta[ibin]->SetDirectory(0); 
    sprintf(tname,"faileta_%i",ibin);
    failEta[ibin] = new TTree(tname,"");
    failEta[ibin]->Branch("m",&mass,"m/F");
    failEta[ibin]->SetDirectory(0); 
  }
  
  TTree* passPhi[phiNbins];  
  TTree* failPhi[phiNbins];
  for(UInt_t ibin=0; ibin<phiNbins; ibin++) {
    sprintf(tname,"passphi_%i",ibin);
    passPhi[ibin] = new TTree(tname,"");
    passPhi[ibin]->Branch("m",&mass,"m/F");
    passPhi[ibin]->SetDirectory(0); 
    sprintf(tname,"failphi_%i",ibin);
    failPhi[ibin] = new TTree(tname,"");
    failPhi[ibin]->Branch("m",&mass,"m/F");
    failPhi[ibin]->SetDirectory(0);
  }
  
  TTree* passEtaPt[etaNbins*ptNbins];  
  TTree* failEtaPt[etaNbins*ptNbins];
  for(UInt_t ibin=0; ibin<(etaNbins*ptNbins); ibin++) {
    sprintf(tname,"passetapt_%i",ibin);
    passEtaPt[ibin] = new TTree(tname,"");
    passEtaPt[ibin]->Branch("m",&mass,"m/F");
    passEtaPt[ibin]->SetDirectory(0); 
    sprintf(tname,"failetapt_%i",ibin);
    failEtaPt[ibin] = new TTree(tname,"");
    failEtaPt[ibin]->Branch("m",&mass,"m/F");
    failEtaPt[ibin]->SetDirectory(0);
  }
  
  TTree* passEtaPhi[etaNbins*phiNbins];
  TTree* failEtaPhi[etaNbins*phiNbins];
  for(UInt_t ibin=0; ibin<(etaNbins*phiNbins); ibin++) {
    sprintf(tname,"passetaphi_%i",ibin); 
    passEtaPhi[ibin] = new TTree(tname,"");
    passEtaPhi[ibin]->Branch("m",&mass,"m/F");
    passEtaPhi[ibin]->SetDirectory(0); 
    sprintf(tname,"failetaphi_%i",ibin);
    failEtaPhi[ibin] = new TTree(tname,"");
    failEtaPhi[ibin]->Branch("m",&mass,"m/F");
    failEtaPhi[ibin]->SetDirectory(0);
  }

  TTree* passNPV[npvNbins];  
  TTree* failNPV[npvNbins];
  for(UInt_t ibin=0; ibin<npvNbins; ibin++) {
    sprintf(tname,"passnpv_%i",ibin);
    passNPV[ibin] = new TTree(tname,"");
    passNPV[ibin]->Branch("m",&mass,"m/F");
    passNPV[ibin]->SetDirectory(0); 
    sprintf(tname,"failnpv_%i",ibin);
    failNPV[ibin] = new TTree(tname,"");
    failNPV[ibin]->Branch("m",&mass,"m/F");
    failNPV[ibin]->SetDirectory(0);
  }

  TTree* passRho[rhoNbins];  
  TTree* failRho[rhoNbins];
  for(UInt_t ibin=0; ibin<rhoNbins; ibin++) {
    sprintf(tname,"passrho_%i",ibin);
    passRho[ibin] = new TTree(tname,"");
    passRho[ibin]->Branch("m",&mass,"m/F");
    passRho[ibin]->SetDirectory(0); 
    sprintf(tname,"failrho_%i",ibin);
    failRho[ibin] = new TTree(tname,"");
    failRho[ibin]->Branch("m",&mass,"m/F");
    failRho[ibin]->SetDirectory(0);
  }
  
  TagAndProbePair *inputData = new TagAndProbePair;
  inputData->LoadTree(infilename);
   
  for(UInt_t ientry=0; ientry < inputData->tree_->GetEntries(); ientry++) {
    inputData->tree_->GetEntry(ientry);
    
    if((inputData->charge)*charge < 0) continue;
    if(inputData->mass < massLo)  continue;
    if(inputData->mass > massHi)  continue;
    
    mass = inputData->mass;
    
    Int_t ipt=-1;
    for(UInt_t ibin=0; ibin<ptNbins; ibin++)
      if((inputData->pt >= ptEdgesv[ibin]) && (inputData->pt < ptEdgesv[ibin+1]))
        ipt = ibin;
    if(ipt<0) continue;
    
    Int_t ieta=-1;
    for(UInt_t ibin=0; ibin<etaNbins; ibin++) {
      if(doAbsEta) {
        assert(etaEdgesv[ibin]>=0);
        if((fabs(inputData->eta) >= etaEdgesv[ibin]) && (fabs(inputData->eta) < etaEdgesv[ibin+1]))
          ieta = ibin;
      } else {
        if((inputData->eta >= etaEdgesv[ibin]) && (inputData->eta < etaEdgesv[ibin+1]))
          ieta = ibin;
      }
    }
    if(ieta<0) continue;
	
    Int_t iphi=-1;
    for(UInt_t ibin=0; ibin<phiNbins; ibin++)
      if((inputData->phi >= phiEdgesv[ibin]) && (inputData->phi < phiEdgesv[ibin+1]))
        iphi = ibin;
    if(iphi<0) continue;
	
    Int_t inpv=-1;
    for(UInt_t ibin=0; ibin<npvNbins; ibin++)
      if((inputData->NPV >= npvEdgesv[ibin]) && (inputData->NPV < npvEdgesv[ibin+1]))
        inpv = ibin;
    if(inpv<0) continue;

    Int_t irho=-1;
    for(UInt_t ibin=0; ibin<rhoNbins; ibin++)
      if((inputData->Rho >= rhoEdgesv[ibin]) && (inputData->Rho < rhoEdgesv[ibin+1]))
        irho = ibin;
    if(irho<0) continue;
        
    if(inputData->pass) {
      passPt[ipt]->Fill();
      passEta[ieta]->Fill();
      passPhi[iphi]->Fill();
      passEtaPt[ipt*etaNbins + ieta]->Fill();
      passEtaPhi[iphi*etaNbins + ieta]->Fill();
      passNPV[inpv]->Fill();
      passRho[inpv]->Fill();
    } else {
      failPt[ipt]->Fill();
      failEta[ieta]->Fill();
      failPhi[iphi]->Fill();
      failEtaPt[ipt*etaNbins + ieta]->Fill();
      failEtaPhi[iphi*etaNbins + ieta]->Fill();
      failNPV[inpv]->Fill();
      failRho[inpv]->Fill();
    }    
  }
 
  TFile outfile(Form("%s/dataTemplates.root",CPlot::sOutDir.Data()), "RECREATE");
  for(UInt_t ibin=0; ibin<ptNbins; ibin++) {
    passPt[ibin]->Write();
    failPt[ibin]->Write();
    delete passPt[ibin];
    delete failPt[ibin];
  }
  for(UInt_t ibin=0; ibin<etaNbins; ibin++) { 
    passEta[ibin]->Write();
    failEta[ibin]->Write();
    delete passEta[ibin];
    delete failEta[ibin];
  }
  for(UInt_t ibin=0; ibin<phiNbins; ibin++) {
    passPhi[ibin]->Write();
    failPhi[ibin]->Write();
    delete passPhi[ibin];
    delete failPhi[ibin];
  }
  for(UInt_t ibin=0; ibin<(etaNbins*ptNbins); ibin++) {
    passEtaPt[ibin]->Write();
    failEtaPt[ibin]->Write();
    delete passEtaPt[ibin];
    delete failEtaPt[ibin];
  }
  for(UInt_t ibin=0; ibin<(etaNbins*phiNbins); ibin++) {
    passEtaPhi[ibin]->Write();
    failEtaPhi[ibin]->Write();
    delete passEtaPhi[ibin];
    delete failEtaPhi[ibin];
  }
  for(UInt_t ibin=0; ibin<npvNbins; ibin++) {
    passNPV[ibin]->Write();
    failNPV[ibin]->Write();
    delete passNPV[ibin];
    delete failNPV[ibin];
  }
  for(UInt_t ibin=0; ibin<rhoNbins; ibin++) {
    passRho[ibin]->Write();
    failRho[ibin]->Write();
    delete passRho[ibin];
    delete failRho[ibin];
  }
  outfile.Write();
  outfile.Close(); 

  cout << "Done!" << endl;
}

//--------------------------------------------------------------------------------------------------
void performCount(Double_t &resEff, Double_t &resErrl, Double_t &resErrh,
                  const Int_t ibin, const Double_t xbinLo, const Double_t xbinHi, const Double_t ybinLo, const Double_t ybinHi,
		  TTree *passTree, TTree *failTree, const Int_t method, 
		  const TString name, const Double_t massLo, const Double_t massHi, const TString format, const Bool_t doAbsEta,
		  TCanvas *cpass, TCanvas *cfail)
{
  Float_t m,w;
  char pname[50];
  char binlabelx[100];
  char binlabely[100];
  char yield[50];
  char effstr[100];    
  
 Double_t npass=0, ntotal=0;
  passTree->SetBranchAddress("m",&m);
  passTree->SetBranchAddress("w",&w);
  for(UInt_t ientry=0; ientry<passTree->GetEntries(); ientry++) {
    passTree->GetEntry(ientry);
    if(m<76 || m>106) continue;
    //if(m<60 || m>120) continue;
    npass+=w;
    ntotal+=w;
  }
  failTree->SetBranchAddress("m",&m);
  failTree->SetBranchAddress("w",&w);
  for(UInt_t ientry=0; ientry<failTree->GetEntries(); ientry++) {
    failTree->GetEntry(ientry);
    //if(m<76 || m>106) continue;
    if(m<60 || m>120) continue;
    ntotal+=w;
  }
  resEff  = npass/ntotal;
  if(method==0) {
    resErrl = resEff - TEfficiency::ClopperPearson((UInt_t)ntotal, (UInt_t)npass, 0.68269, kFALSE);
    resErrh = TEfficiency::ClopperPearson((UInt_t)ntotal, (UInt_t)npass, 0.68269, kTRUE) - resEff;
  }
//*/    
  if(name.CompareTo("pt")==0) {
    sprintf(binlabelx,"%i GeV/c < p_{T} < %i GeV/c",Int_t(xbinLo),Int_t(xbinHi));
  
  } else if(name.CompareTo("eta")==0) { 
    if(doAbsEta) sprintf(binlabelx,"%.1f < |#eta| < %.1f",xbinLo,xbinHi);
    else	 sprintf(binlabelx,"%.1f < #eta < %.1f",xbinLo,xbinHi);
  
  } else if(name.CompareTo("phi")==0) { 
    sprintf(binlabelx,"%.1f < #phi < %.1f",xbinLo,xbinHi); 
  
  } else if(name.CompareTo("etapt")==0) {
    if(doAbsEta) sprintf(binlabelx,"%.1f < |#eta| < %.1f",xbinLo,xbinHi);
    else         sprintf(binlabelx,"%.1f < #eta < %.1f",xbinLo,xbinHi);    
    sprintf(binlabely,"%i GeV/c < p_{T} < %i GeV/c",Int_t(ybinLo),Int_t(ybinHi));
  
  } else if(name.CompareTo("etaphi")==0) {
    if(doAbsEta) sprintf(binlabelx,"%.1f < |#eta| < %.1f",xbinLo,xbinHi);
    else         sprintf(binlabelx,"%.1f < #eta < %.1f",xbinLo,xbinHi);					   
    sprintf(binlabely,"%.1f < #phi < %.1f",ybinLo,ybinHi);
  
  } else if(name.CompareTo("npv")==0) { 
    sprintf(binlabelx,"%i #leq N_{PV} < %i",(Int_t)xbinLo,(Int_t)xbinHi); 
  
  } else if(name.CompareTo("rho")==0) { 
    sprintf(binlabelx,"%i #leq #rho < %i",(Int_t)xbinLo,(Int_t)xbinHi); 
  
  }
  sprintf(effstr,"#varepsilon = %.4f_{ -%.4f}^{ +%.4f}",resEff,resErrl,resErrh);
  
  //
  // Plot passing probes
  //
  TH1D *hpass = new TH1D("hpass","",Int_t(massHi-massLo),massLo,massHi);
  passTree->SetBranchAddress("m",&m);
  passTree->SetBranchAddress("w",&w);
  hpass->Sumw2();
  for(UInt_t ientry=0; ientry<passTree->GetEntries(); ientry++) {
    passTree->GetEntry(ientry);
    hpass->Fill(m,w);
  }
  sprintf(pname,"pass%s_%i",name.Data(),ibin);
  sprintf(yield,"%i Events",(UInt_t)npass);
  CPlot plotPass(pname,"Passing probes","tag-probe mass [GeV/c^{2}]","Events / 1.0 GeV/c^{2}");
  plotPass.AddHist1D(hpass,"E");
  plotPass.AddTextBox(binlabelx,0.21,0.85,0.51,0.90,0,kBlack,-1);
  if((name.CompareTo("etapt")==0) || (name.CompareTo("etaphi")==0)) {
    plotPass.AddTextBox(binlabely,0.21,0.80,0.51,0.85,0,kBlack,-1);        
    plotPass.AddTextBox(yield,0.21,0.76,0.51,0.80,0,kBlack,-1);    
  } else {
    plotPass.AddTextBox(yield,0.21,0.81,0.51,0.85,0,kBlack,-1);
  }
  plotPass.AddTextBox(effstr,0.70,0.85,0.95,0.90,0,kBlack,-1);
  plotPass.Draw(cpass,kTRUE,format);
  
  //
  // Plot failing probes
  //
  TH1D *hfail = new TH1D("hfail","",Int_t(massHi-massLo)/2,massLo,massHi);
  hfail->Sumw2();
  failTree->SetBranchAddress("m",&m);
  failTree->SetBranchAddress("w",&w);
  for(UInt_t ientry=0; ientry<failTree->GetEntries(); ientry++) {
    failTree->GetEntry(ientry);
    hfail->Fill(m,w);
  }
  sprintf(pname,"fail%s_%i",name.Data(),ibin);
  sprintf(yield,"%i Events",(UInt_t)(ntotal-npass));
  CPlot plotFail(pname,"Failing probes","tag-probe mass [GeV/c^{2}]","Events / 2.0 GeV/c^{2}");
  plotFail.AddHist1D(hfail,"E");
  plotFail.AddTextBox(binlabelx,0.21,0.85,0.51,0.90,0,kBlack,-1);
  if((name.CompareTo("etapt")==0) || (name.CompareTo("etaphi")==0)) {
    plotFail.AddTextBox(binlabely,0.21,0.80,0.51,0.85,0,kBlack,-1);    
    plotFail.AddTextBox(yield,0.21,0.76,0.51,0.80,0,kBlack,-1);    
  } else {
    plotFail.AddTextBox(yield,0.21,0.81,0.51,0.85,0,kBlack,-1);
  }
  plotFail.AddTextBox(effstr,0.70,0.85,0.95,0.90,0,kBlack,-1);
  plotFail.Draw(cfail,kTRUE,format);
  
  delete hpass;
  delete hfail;
}

//--------------------------------------------------------------------------------------------------
void performFit(Double_t &resEff, Double_t &resErrl, Double_t &resErrh,
                const Int_t ibin, const Double_t xbinLo, const Double_t xbinHi, const Double_t ybinLo, const Double_t ybinHi,
		TTree *passTree, TTree *failTree,
		const Int_t sigpass, const Int_t bkgpass, const Int_t sigfail, const Int_t bkgfail, 
		const TString name, const Double_t massLo, const Double_t massHi, const TString format, const Bool_t doAbsEta,
		TCanvas *cpass, TCanvas *cfail)
{

  RooRealVar m("m","mass",massLo,massHi);
  m.setBins(10000);
  
  char pname[50];
  char binlabelx[100];
  char binlabely[100];
  char yield[50];
  char effstr[100];
  char nsigstr[100];
  char nbkgstr[100];
  char chi2str[100];
  
  Int_t nflpass=0, nflfail=0;
    
  TFile *signalhistfile = 0;
  if(sigpass==2 || sigfail==2) {
    signalhistfile = new TFile(Form("%s/signalHistTemplates.root",CPlot::sOutDir.Data()));
    assert(signalhistfile);
  }
  TFile *datfile = 0;
  if(sigpass==4 || sigfail==4) {
    datfile = new TFile(Form("%s/dataTemplates.root",CPlot::sOutDir.Data()));
    assert(datfile);
  }

  // Define categories
  RooCategory sample("sample","");
  sample.defineType("Pass",1);
  sample.defineType("Fail",2);
  
  RooAbsData *dataPass=0;
  RooAbsData *dataFail=0;
  TH1D histPass("histPass","",Int_t(massHi-massLo),massLo,massHi); 
  TH1D histFail("histFail","",Int_t(massHi-massLo)/2,massLo,massHi);
  RooAbsData *dataCombined=0;
  
  const Bool_t doBinned = kTRUE;//(passTree->GetEntries()>1000 && failTree->GetEntries()>1000);
  
  if(doBinned) {
    passTree->Draw("m>>histPass");
    failTree->Draw("m>>histFail");
    dataPass = new RooDataHist("dataPass","dataPass",RooArgSet(m),&histPass);
    dataFail = new RooDataHist("dataFail","dataFail",RooArgSet(m),&histFail);
    m.setBins(70);  
   
    dataCombined = new RooDataHist("dataCombined","dataCombined",RooArgList(m),
                                   RooFit::Index(sample),
				   RooFit::Import("Pass",*((RooDataHist*)dataPass)),
				   RooFit::Import("Fail",*((RooDataHist*)dataFail)));  
  
  } else {
    dataPass = new RooDataSet("dataPass","dataPass",passTree,RooArgSet(m));
    dataFail = new RooDataSet("dataFail","dataFail",failTree,RooArgSet(m));
    
    dataCombined = new RooDataSet("dataCombined","dataCombined",RooArgList(m),
      				  RooFit::Index(sample),
  	  			  RooFit::Import("Pass",*((RooDataSet*)dataPass)),
  				  RooFit::Import("Fail",*((RooDataSet*)dataFail))); 
  }
  
  // Define signal and background models
  CSignalModel     *sigPass = 0;
  CBackgroundModel *bkgPass = 0;
  CSignalModel     *sigFail = 0;
  CBackgroundModel *bkgFail = 0;
  
  //global energy scale and resolution parameters for fail sample
  RooRealVar *meanFail = 0;
  RooRealVar *sigmaFail = 0;

  if (sigfail == 2 && ( bkgfail == 6 || bkgfail == 7)) {
    meanFail = new RooRealVar("meanFail","meanFail", 0, -5, 5);
    sigmaFail = new RooRealVar("sigmaFail","sigmaFail", 1, 0, 5);
  }

  if(sigpass==1) {
    sigPass = new CBreitWignerConvCrystalBall(m,kTRUE);
    nflpass += 4;
  
  } else if(sigpass==2) { 
    char hname[50];
    sprintf(hname,"pass%s_%i",name.Data(),ibin);
    cout << "Load signal template " << hname << endl;
    TH1D *h = (TH1D*)signalhistfile->Get(hname);
    assert(h);
    sigPass = new CMCTemplateConvGaussian(m,h,kTRUE);
    nflpass += 2;
  
  } else if(sigpass==3) {
    sigPass = new CVoigtianCBShape(m,kTRUE);
    nflpass += 4;
  
  } else if(sigpass==4) {
    char tname[50];
    sprintf(tname,"pass%s_%i",name.Data(),ibin);
    TTree *t = (TTree*)datfile->Get(tname);
    assert(t);
    sigPass = new CMCDatasetConvGaussian(m,t,kTRUE);
    nflpass += 2;
  }
   

  if(bkgpass==1) { 
    bkgPass = new CExponential(m,kTRUE);
    nflpass += 1;
  
  } else if(bkgpass==2) {
    bkgPass = new CErfExpo(m,kTRUE);
    nflpass += 3;
     
  } else if(bkgpass ==3) {
    bkgPass = new CDoubleExp(m,kTRUE);
    nflpass += 3;
  } else if(bkgpass==4) {
    bkgPass = new CLinearExp(m,kTRUE);
    nflpass += 2;
  } else if(bkgpass==5) {
    bkgPass = new CQuadraticExp(m,kTRUE);
    nflpass += 3;  
  }


  if(sigfail==1) {
    sigFail = new CBreitWignerConvCrystalBall(m,kFALSE);
    nflfail += 4;
  
  } else if(sigfail==2) {
    char hname[50];
    sprintf(hname,"fail%s_%i",name.Data(),ibin);
    cout << "Load signal template " << hname << endl;
    TH1D *h = (TH1D*)signalhistfile->Get(hname);
    assert(h);
    if (bkgfail == 6 || bkgfail == 7) {
      sigFail = new CMCTemplateConvGaussian(m,h,kFALSE, meanFail, sigmaFail);
    } else {
      sigFail = new CMCTemplateConvGaussian(m,h,kFALSE);
    }
    nflfail += 2;
  
  } else if(sigfail==3) {
    sigFail = new CVoigtianCBShape(m,kFALSE);
    nflfail += 4;
  
  } else if(sigfail==4) {
    char tname[50];
    sprintf(tname,"fail%s_%i",name.Data(),ibin);
    TTree *t = (TTree*)datfile->Get(tname);
    assert(t);
    sigFail = new CMCDatasetConvGaussian(m,t,kFALSE);
    nflfail += 2;
  }

  // reco to loose ID (electron)
  //if(bkgfail==1 || ibin>=12) {
  // muon
  //if(bkgfail==1 || ibin>=8) { 
  if(bkgfail==1) { 
    bkgFail = new CExponential(m,kFALSE);
    nflfail += 1;
  } else if(bkgfail==2) {
    // reco to loose ID electron
    //} else if(bkgfail==2 && !(ibin>=12)) {
    // muon
    //} else if(bkgfail==2 && !(ibin>=8)) {
    cout << "right background model" << endl;
    bkgFail = new CErfExpo(m,kFALSE); 
    nflfail += 3;
  } else if(bkgfail==3) {
    bkgFail = new CDoubleExp(m,kFALSE);
    nflfail += 3;
  } else if(bkgfail==4) {
    bkgFail = new CLinearExp(m,kFALSE);
    nflfail += 2;
  } else if(bkgfail==5) {
    bkgFail = new CQuadraticExp(m,kFALSE);
    nflfail += 3;  
  }  else if(bkgfail==6) {
    bkgFail = new CQuadratic(m,kFALSE);
    nflfail += 2;  
  } 

  cout << "here1\n";

  // Define free parameters
  Double_t NsigMax     = doBinned ? histPass.Integral()+histFail.Integral() : passTree->GetEntries()+failTree->GetEntries();
  Double_t NbkgFailMax = doBinned ? histFail.Integral() : failTree->GetEntries();
  Double_t NbkgPassMax = doBinned ? histPass.Integral() : passTree->GetEntries();
  RooRealVar Nsig("Nsig","Signal Yield",0.80*NsigMax,0,NsigMax);
  RooRealVar eff("eff","Efficiency",0.8,0,1.0);
  RooRealVar NbkgPass("NbkgPass","Background count in PASS sample",0,0,NbkgPassMax);
  if(bkgpass==0) NbkgPass.setVal(0);
  RooRealVar NbkgFail("NbkgFail","Background count in FAIL sample",0.6*NbkgFailMax,0,NbkgFailMax);
  cout << "here2\n";

  RooFormulaVar NsigPass("NsigPass","eff*Nsig",RooArgList(eff,Nsig));
  RooFormulaVar NsigFail("NsigFail","(1.0-eff)*Nsig",RooArgList(eff,Nsig));
  RooAddPdf *modelPass=0, *modelFail=0;
  RooExtendPdf *esignalPass=0, *ebackgroundPass=0, *esignalFail=0, *ebackgroundFail=0;
  //m.setRange("signalRange",85, 95);
  m.setRange("signalRange",76, 106);
  //m.setRange("signalRange",60, 120);
    
  esignalPass     = new RooExtendPdf("esignalPass","esignalPass",*(sigPass->model),NsigPass,"signalRange");
  ebackgroundPass = new RooExtendPdf("ebackgroundPass","ebackgroundPass",(bkgpass>0) ? *(bkgPass->model) : *(sigPass->model),NbkgPass,"signalRange");
  modelPass       = new RooAddPdf("modelPass","Model for PASS sample",(bkgpass>0) ? RooArgList(*esignalPass,*ebackgroundPass) : RooArgList(*esignalPass));    
  
  esignalFail     = new RooExtendPdf("esignalFail","esignalFail",*(sigFail->model),NsigFail,"signalRange");
  ebackgroundFail = new RooExtendPdf("ebackgroundFail","ebackgroundFail",*(bkgFail->model),NbkgFail,"signalRange");
  modelFail       = new RooAddPdf("modelFail","Model for FAIL sample", (bkgfail>0) ? RooArgList(*esignalFail,*ebackgroundFail) : RooArgList(*esignalFail));
  
  cout << "here3\n";

  RooSimultaneous totalPdf("totalPdf","totalPdf",sample);
  totalPdf.addPdf(*modelPass,"Pass");  
  totalPdf.addPdf(*modelFail,"Fail");

  //some custom massaging

  // reco to ID
  //if (ibin==9 || ibin==10) {
  // reco to tight ID
  //Nsig.setVal(9900.0);
  //eff.setVal(0.8);
  //((CErfExpo*)bkgFail)->alfa->setVal(170);
  //((CErfExpo*)bkgFail)->beta->setVal(0.038);
  //((CErfExpo*)bkgFail)->gamma->setVal(0.26);
  // reco to loose ID
  //Nsig.setVal(9000.0);
  //eff.setVal(0.9);
  //((CErfExpo*)bkgFail)->alfa->setVal(66);
  //((CErfExpo*)bkgFail)->beta->setVal(0.15);
  //((CErfExpo*)bkgFail)->gamma->setVal(0.01);
  //}

  // loose reco to ID
  //if (ibin==4 || ibin==5 || ibin==6) {
  //Nsig.setVal(400.0);
  //}

  //if (ibin==7) {
  //  Nsig.setVal(600);
  //  NbkgPass.setVal(10);
  //  NbkgFail.setVal(10);
  //  eff.setVal(0.95);
  //  ((CExponential*)bkgFail)->t->setVal(-0.02);
  //}
  //
  //if (ibin==9 || ibin==10) {
  //  Nsig.setVal(8800);
  //  NbkgPass.setVal(130);
  //  NbkgFail.setVal(85);
  //  eff.setVal(0.95);
  //  ((CExponential*)bkgFail)->t->setVal(-0.02);
  //}

  //muon reco to tight
  //if (ibin==3) {
  //NbkgFail.setVal(200);
  //eff.setVal(0.88);
  //}

  cout << "here4\n";

  RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING) ;

  RooFitResult *fitResult=0;
  fitResult = totalPdf.fitTo(*dataCombined,
			     RooFit::Extended(),
  			     //RooFit::Strategy(2),
			     RooFit::Strategy(1),
  			     //RooFit::Minos(RooArgSet(eff)),
  			     RooFit::Save());

 
  // Refit w/o MINOS if MINOS errors are strange...
  if((fabs(eff.getErrorLo())<5e-5) || (eff.getErrorHi()<5e-5))
    fitResult = totalPdf.fitTo(*dataCombined, RooFit::Extended(), RooFit::Strategy(1), RooFit::Save());

  //reco to tight ID
  //if (ibin==12) fitResult = totalPdf.fitTo(*dataCombined, RooFit::Extended(), RooFit::Strategy(1), RooFit::Save());
  
  cout << "here5\n";

  resEff  = eff.getVal();
  resErrl = fabs(eff.getErrorLo());
  resErrh = eff.getErrorHi();
    
  if(name.CompareTo("pt")==0) {
    sprintf(binlabelx,"%i GeV/c < p_{T} < %i GeV/c",Int_t(xbinLo),Int_t(xbinHi));
  
  } else if(name.CompareTo("eta")==0) { 
    if(doAbsEta) sprintf(binlabelx,"%.1f < |#eta| < %.1f",xbinLo,xbinHi);
    else	 sprintf(binlabelx,"%.1f < #eta < %.1f",xbinLo,xbinHi);
  
  } else if(name.CompareTo("phi")==0) { 
    sprintf(binlabelx,"%.1f < #phi < %.1f",xbinLo,xbinHi); 
  
  } else if(name.CompareTo("etapt")==0) {
    if(doAbsEta) sprintf(binlabelx,"%.1f < |#eta| < %.1f",xbinLo,xbinHi);
    else         sprintf(binlabelx,"%.1f < #eta < %.1f",xbinLo,xbinHi);    
    sprintf(binlabely,"%i GeV/c < p_{T} < %i GeV/c",Int_t(ybinLo),Int_t(ybinHi));
  
  } else if(name.CompareTo("etaphi")==0) {
    if(doAbsEta) sprintf(binlabelx,"%.1f < |#eta| < %.1f",xbinLo,xbinHi);
    else         sprintf(binlabelx,"%.1f < #eta < %.1f",xbinLo,xbinHi);					   
    sprintf(binlabely,"%.1f < #phi < %.1f",ybinLo,ybinHi);
  
  } else if(name.CompareTo("npv")==0) { 
    sprintf(binlabelx,"%i #leq N_{PV} < %i",(Int_t)xbinLo,(Int_t)xbinHi); 
  
  } else if(name.CompareTo("rho")==0) { 
    sprintf(binlabelx,"%i #leq #rho < %i",(Int_t)xbinLo,(Int_t)xbinHi); 
  
  } 
  sprintf(effstr,"#varepsilon = %.4f_{ -%.4f}^{ +%.4f}",eff.getVal(),fabs(eff.getErrorLo()),eff.getErrorHi());

  RooPlot *mframePass = m.frame(Bins(Int_t(massHi-massLo)));
  dataPass->plotOn(mframePass,MarkerStyle(kFullCircle),MarkerSize(0.8),DrawOption("ZP"));    
  modelPass->plotOn(mframePass);
  if(bkgpass>0)   
    modelPass->plotOn(mframePass,Components("ebackgroundPass"),LineStyle(kDashed),LineColor(kRed));

  
  RooPlot *mframeFail = m.frame(Bins(Int_t(massHi-massLo)/2));
  dataFail->plotOn(mframeFail,MarkerStyle(kFullCircle),MarkerSize(0.8),DrawOption("ZP"));
  modelFail->plotOn(mframeFail);
  modelFail->plotOn(mframeFail,Components("ebackgroundFail"),LineStyle(kDashed),LineColor(kRed));
  if (bkgfail==7) {
    modelFail->plotOn(mframeFail,Components("bkgexpFail"),LineStyle(kDashed),LineColor(kGreen+2));
  }

  //
  // Plot passing probes
  //
  sprintf(pname,"pass%s_%i",name.Data(),ibin);
  sprintf(yield,"%u Events",(Int_t)passTree->GetEntries());
  sprintf(nsigstr,"N_{sig} = %.1f #pm %.1f",NsigPass.getVal(),NsigPass.getPropagatedError(*fitResult));
  sprintf(chi2str,"#chi^{2}/DOF = %.3f",mframePass->chiSquare(nflpass));
  if(bkgpass>0)
    sprintf(nbkgstr,"N_{bkg} = %.1f #pm %.1f",NbkgPass.getVal(),NbkgPass.getPropagatedError(*fitResult));
  CPlot plotPass(pname,mframePass,"Passing probes","tag-probe mass [GeV/c^{2}]","Events / 1.0 GeV/c^{2}");
  plotPass.AddTextBox(binlabelx,0.21,0.85,0.51,0.90,0,kBlack,-1);
  if((name.CompareTo("etapt")==0) || (name.CompareTo("etaphi")==0)) {
    plotPass.AddTextBox(binlabely,0.21,0.80,0.51,0.85,0,kBlack,-1);        
    plotPass.AddTextBox(yield,0.21,0.76,0.51,0.80,0,kBlack,-1);    
  } else {
    plotPass.AddTextBox(yield,0.21,0.81,0.51,0.85,0,kBlack,-1);
  }
  plotPass.AddTextBox(effstr,0.70,0.85,0.94,0.90,0,kBlack,-1);
  if(bkgpass>0)
    plotPass.AddTextBox(0.70,0.68,0.94,0.83,0,kBlack,-1,2,nsigstr,nbkgstr);//,chi2str);
  else
    plotPass.AddTextBox(0.70,0.73,0.94,0.83,0,kBlack,-1,1,nsigstr);//,chi2str);
  plotPass.Draw(cpass,kTRUE,format);
 
  //
  // Plot failing probes
  //
  sprintf(pname,"fail%s_%i",name.Data(),ibin);
  sprintf(yield,"%u Events",(Int_t)failTree->GetEntries());
  sprintf(nsigstr,"N_{sig} = %.1f #pm %.1f",NsigFail.getVal(),NsigFail.getPropagatedError(*fitResult));
  sprintf(nbkgstr,"N_{bkg} = %.1f #pm %.1f",NbkgFail.getVal(),NbkgFail.getPropagatedError(*fitResult));
  sprintf(chi2str,"#chi^{2}/DOF = %.3f",mframePass->chiSquare(nflfail));
  CPlot plotFail(pname,mframeFail,"Failing probes","tag-probe mass [GeV/c^{2}]","Events / 2.0 GeV/c^{2}");
  plotFail.AddTextBox(binlabelx,0.21,0.85,0.51,0.90,0,kBlack,-1);
  if((name.CompareTo("etapt")==0) || (name.CompareTo("etaphi")==0)) {
    plotFail.AddTextBox(binlabely,0.21,0.80,0.51,0.85,0,kBlack,-1);    
    plotFail.AddTextBox(yield,0.21,0.76,0.51,0.80,0,kBlack,-1);    
  } else {
    plotFail.AddTextBox(yield,0.21,0.81,0.51,0.85,0,kBlack,-1);
  }
  plotFail.AddTextBox(effstr,0.70,0.85,0.94,0.90,0,kBlack,-1);  
  plotFail.AddTextBox(0.70,0.68,0.94,0.83,0,kBlack,-1,2,nsigstr,nbkgstr);//,chi2str);
  plotFail.Draw(cfail,kTRUE,format);  

  //
  // Write fit results
  //
  ofstream txtfile;
  char txtfname[100];    
  sprintf(txtfname,"%s/fitres%s_%i.txt",CPlot::sOutDir.Data(),name.Data(),ibin);
  txtfile.open(txtfname);
  assert(txtfile.is_open()); 
  fitResult->printStream(txtfile,RooPrintable::kValue,RooPrintable::kVerbose);
  txtfile << endl;
  printCorrelations(txtfile, fitResult);
  txtfile.close();
  
  //
  // Write workspace
  //
  RooWorkspace *w = new RooWorkspace("MassFitWorkspace");
  w->import(totalPdf);
  w->import(*dataCombined);
  TFile *workspaceFile = new TFile( Form("%s/FitWorkspaceFile_%s_%i.root",CPlot::sOutDir.Data(),name.Data(),ibin), "RECREATE");
  workspaceFile->WriteTObject(w, w->GetName(), "WriteDelete");
  workspaceFile->Close();
  delete workspaceFile;



  delete dataCombined;
  delete sigPass;
  delete bkgPass;
  delete sigFail;
  delete bkgFail;        
  delete signalhistfile;
  delete datfile;   
}

//--------------------------------------------------------------------------------------------------
void printCorrelations(ostream& os, RooFitResult *res)
{
  ios_base::fmtflags flags = os.flags();
  const RooArgList *parlist = res->correlation("eff");
  
  os << "  Correlation Matrix" << endl;
  os << " --------------------" << endl;
  for(Int_t i=0; i<parlist->getSize(); i++) {
    for(Int_t j=0; j<parlist->getSize(); j++) 
      os << "  " << setw(7) << setprecision(4) << fixed << res->correlationMatrix()(i,j);    
    os << endl;
  }
  os.flags(flags);
}
