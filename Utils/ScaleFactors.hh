
#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>                  // access to gROOT, entry point to ROOT system
#include <TSystem.h>                // interface to OS
#include <TFile.h>                  // file handle class
#include <TBenchmark.h>             // class to track macro running statistics
#include <TGraphAsymmErrors.h>      // graphs
#include <TH2F.h>                   // 2D histograms
#include <TMath.h>                  // ROOT math library
#include <TLegend.h>          
#include <TLatex.h>          
#include <vector>                   // STL vector class
#include <iostream>                 // standard I/O
#include <iomanip>                  // functions to format standard I/O
#include <fstream>                  // functions for file I/O
#endif

//=== Functions =================================================================================================

void makeEfficiencyScaleFactors(string DataFilename,
				string MCFilename, 
				string outputDir, 
				string histName,
				string Label,
				double PtLowRange = 5,
				double PtHighRange = 500,
				double EffLowRange = 0.8,
				double EffHighRange = 1.15
				)
{
  
  string label = Label;
  if (Label != "") label = "_" + Label;

  //--------------------------------------------------------------------------------------------------------------
  // Settings 
  //============================================================================================================== 
  TString outfname = (outputDir + "/eff_table.txt").c_str();
  TFile mcfile(MCFilename.c_str());
  TFile datafile2(DataFilename.c_str());
  
  //--------------------------------------------------------------------------------------------------------------
  // Main analysis code 
  //==============================================================================================================   
  
  TH2F *hMCEff=0,    *hMCErrl=0,    *hMCErrh=0;
  TH2F *hDataEff1=0, *hDataErrl1=0, *hDataErrh1=0;
  TH2F *hDataEff2=0, *hDataErrl2=0, *hDataErrh2=0;
  
  hMCEff  = (TH2F*)mcfile.Get("hEffEtaPt");
  hMCErrl = (TH2F*)mcfile.Get("hErrlEtaPt");
  hMCErrh = (TH2F*)mcfile.Get("hErrhEtaPt");
  
  hDataEff2  = (TH2F*)datafile2.Get("hEffEtaPt");
  hDataErrl2 = (TH2F*)datafile2.Get("hErrlEtaPt");
  hDataErrh2 = (TH2F*)datafile2.Get("hErrhEtaPt");
  
  TGraphAsymmErrors* MCEffVsRho   = ( TGraphAsymmErrors*)mcfile.Get("grEffRho");
  TGraphAsymmErrors* DataEffVsRho = ( TGraphAsymmErrors*)datafile2.Get("grEffRho");
 
  TGraphAsymmErrors* MCEffVsNVtx   = ( TGraphAsymmErrors*)mcfile.Get("grEffNPV");
  TGraphAsymmErrors* DataEffVsNVtx = ( TGraphAsymmErrors*)datafile2.Get("grEffNPV");


  //--------------------------------------------------------------------------------------------------------------
  // Update root file histograms
  //==============================================================================================================   
  const Int_t nx = hMCEff->GetNbinsX();
  const Int_t ny = hMCEff->GetNbinsY();

  TFile *outputFile = new TFile(("efficiency_results"+label+".root").c_str(), "UPDATE");

  //Do Binning
  Double_t *ptbins = new Double_t[ny+1];
  Double_t *etabins = new Double_t[nx+1];
  for(Int_t iy=1; iy<=ny+1; iy++) {
      ptbins[iy-1] = hMCEff->GetYaxis()->GetBinLowEdge(iy);
  }
  for(Int_t ix=1; ix<=nx+1; ix++) {
    etabins[ix-1]= hMCEff->GetXaxis()->GetBinLowEdge(ix);
  }

  //use this to reduce the boundary of the last bin
  //ptbins[ny] = 250;

 
  TH2F *h2_results_selection = new TH2F(histName.c_str(),"",ny,ptbins,nx,etabins);
  for(Int_t iy=0; iy<=ny+2; iy++) {
    for(Int_t ix=0; ix<=nx+2; ix++) {
      h2_results_selection->SetBinContent(iy,ix, 1.0);
      h2_results_selection->SetBinError(iy,ix, 0.0);
    }
  }

  cout << "ptbins : ";
  for(Int_t i=0; i<ny+1;++i) {
    cout << ptbins[i] << " ";
  }
  cout << endl;
  cout << "etabins : ";
  for(Int_t i=0; i<nx+1;++i) {
    cout << etabins[i] << " ";
  }
  cout << endl;




  //--------------------------------------------------------------------------------------------------------------
  // Produce 1D comparative plots vs Pt
  //==============================================================================================================   
  for(Int_t ix=1; ix<=nx; ix++) {
    string etaBinString = Form("%.1f #leq |#eta| #leq %.1f",  hMCEff->GetXaxis()->GetBinLowEdge(ix),  hMCEff->GetXaxis()->GetBinUpEdge(ix));
    Double_t *ptbins = new Double_t[ny];
    Double_t *ptbinsLowErr = new Double_t[ny];
    Double_t *ptbinsHighErr = new Double_t[ny];
    Double_t *effMC = new Double_t[ny];
    Double_t *effMCLowErr = new Double_t[ny];
    Double_t *effMCHighErr = new Double_t[ny];
    Double_t *effData = new Double_t[ny];
    Double_t *effDataLowErr = new Double_t[ny];
    Double_t *effDataHighErr = new Double_t[ny];
    Double_t *SF = new Double_t[ny];
    Double_t *SFLowErr = new Double_t[ny];
    Double_t *SFHighErr = new Double_t[ny];
    for(Int_t iy=1; iy<=ny; iy++) {
      ptbins[iy-1] = hMCEff->GetYaxis()->GetBinCenter(iy);
      ptbinsLowErr[iy-1] = hMCEff->GetYaxis()->GetBinCenter(iy) - hMCEff->GetYaxis()->GetBinLowEdge(iy);
      ptbinsHighErr[iy-1] = hMCEff->GetYaxis()->GetBinUpEdge(iy) - hMCEff->GetYaxis()->GetBinCenter(iy) ;
      effMC[iy-1] = hMCEff->GetBinContent(ix,iy);
      effMCLowErr[iy-1] = hMCErrl->GetBinContent(ix,iy);
      effMCHighErr[iy-1] = hMCErrh->GetBinContent(ix,iy);
      effData[iy-1] = hDataEff2->GetBinContent(ix,iy);
      effDataLowErr[iy-1] = hDataErrl2->GetBinContent(ix,iy);
      effDataHighErr[iy-1] = min( hDataErrh2->GetBinContent(ix,iy) , 1.0 - effData[iy-1] );
      if ( effMC[iy-1] > 0 && effData[iy-1]>0) {
	SF[iy-1] = effData[iy-1] / effMC[iy-1];
	SFLowErr[iy-1] = SF[iy-1]*sqrt( pow(effMCLowErr[iy-1]/effMC[iy-1],2) + pow(effDataLowErr[iy-1]/effData[iy-1],2));
	SFHighErr[iy-1]= SF[iy-1]*sqrt( pow(effMCHighErr[iy-1]/effMC[iy-1],2) + pow(effDataHighErr[iy-1]/effData[iy-1],2));
      } else {
	SF[iy-1] = 0;
	SFLowErr[iy-1] = 0;
	SFHighErr[iy-1]= 0;
      }

      cout << "pt: " << iy << " : " << ptbins[iy-1] << " : " << effMC[iy-1] << " " << effMCLowErr[iy-1] << " " << effMCHighErr[iy-1] << "\n";
      cout << "pt: " << iy << " : " << ptbins[iy-1] << " : " << effData[iy-1] << " " << effDataLowErr[iy-1] << " " << effDataHighErr[iy-1] << "\n";

    }

    TGraphAsymmErrors* MCEffVsPt = new TGraphAsymmErrors(ny+1, ptbins, effMC, ptbinsLowErr, ptbinsHighErr, effMCLowErr, effMCHighErr);
    TGraphAsymmErrors* DataEffVsPt = new TGraphAsymmErrors(ny+1, ptbins, effData, ptbinsLowErr, ptbinsHighErr, effDataLowErr, effDataHighErr);
    TGraphAsymmErrors* SFVsPt = new TGraphAsymmErrors(ny+1, ptbins, SF, ptbinsLowErr, ptbinsHighErr, SFLowErr, SFHighErr);

    TCanvas *cv = new TCanvas("cv","cv", 800,600);
    cv->SetHighLightColor(2);
    cv->SetFillColor(0);
    cv->SetBorderMode(0);
    cv->SetBorderSize(2);
    cv->SetLeftMargin(0.10);
    cv->SetRightMargin(0.3);
    cv->SetTopMargin(0.07);
    cv->SetBottomMargin(0.12);
    cv->SetFrameBorderMode(0);  

    TLegend *legend = new TLegend(0.6,0.7,0.85,0.9);
    legend->SetTextSize(0.05);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    
    TPad *pad1 = new TPad("pad1","pad1", 0,0.40,1,1);
    pad1->SetBottomMargin(0.025);
    pad1->SetLeftMargin(0.10);
    pad1->SetRightMargin(0.04);
    pad1->Draw();
    pad1->cd();
    
   
    cout << "Bin : " << etaBinString << "\n";
    MCEffVsPt->SetTitle("");
    MCEffVsPt->Draw("ap");
    MCEffVsPt->GetXaxis()->SetRangeUser(PtLowRange,PtHighRange);
    MCEffVsPt->GetYaxis()->SetRangeUser(EffLowRange,EffHighRange);
    MCEffVsPt->GetXaxis()->SetLabelSize(0);
    MCEffVsPt->GetXaxis()->SetTitle("");
    MCEffVsPt->GetYaxis()->SetTitle("Efficiency");
    MCEffVsPt->GetYaxis()->SetTitleSize(0.07);
    MCEffVsPt->GetYaxis()->SetTitleOffset(0.65);
    DataEffVsPt->SetMarkerStyle(23);
    DataEffVsPt->SetMarkerColor(kRed);
    DataEffVsPt->SetLineColor(kRed);
    DataEffVsPt->Draw("psame");

    TLatex *tex = new TLatex();
    tex->SetNDC();
    tex->SetTextSize(0.060);
    tex->SetTextFont(42);
    tex->SetTextColor(kBlack);   
    tex->DrawLatex(0.17, 0.80, etaBinString.c_str());
 

    legend->AddEntry(MCEffVsPt, "Simulation","LP");
    legend->AddEntry(DataEffVsPt, "Data","LP");
    legend->Draw();
    //pad1->SetLogx();


    cv->cd();
    cv->Update();
    
    TPad *pad2 = new TPad("pad2","pad2", 0,0,1,0.4);
    pad2->SetTopMargin(0.05);
    pad2->SetLeftMargin(0.10);
    pad2->SetBottomMargin(0.25);
    pad2->SetRightMargin(0.04);    
    pad2->Draw();
    pad2->cd();

    SFVsPt->SetTitle("");
    SFVsPt->Draw("ap");
    SFVsPt->GetXaxis()->SetRangeUser(PtLowRange,PtHighRange);
    SFVsPt->GetYaxis()->SetTitle("Data/MC Scale Factor");
    SFVsPt->GetYaxis()->SetRangeUser(0.8,1.2);
    SFVsPt->GetYaxis()->SetTitleSize(0.085);
    SFVsPt->GetYaxis()->SetTitleOffset(0.5);
    SFVsPt->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    SFVsPt->GetXaxis()->SetTitleSize(0.11);
    SFVsPt->GetXaxis()->SetLabelSize(0.10);
    SFVsPt->GetXaxis()->SetTitleOffset(0.8);
    //pad2->SetLogx();

    cv->SaveAs(Form("%s/EfficiencyComparison_EtaBin%d.gif",outputDir.c_str(),ix));

  }


  //--------------------------------------------------------------------------------------------------------------
  // Produce 1D comparative plots vs Eta
  //==============================================================================================================   
  for(Int_t iy=1; iy<=ny; iy++) {
    string ptBinString = Form("%.0f #leq p_{T} #leq %.0f",  hMCEff->GetYaxis()->GetBinLowEdge(iy),  hMCEff->GetYaxis()->GetBinUpEdge(iy));
    Double_t *etabins = new Double_t[nx];
    Double_t *etabinsLowErr = new Double_t[nx];
    Double_t *etabinsHighErr = new Double_t[nx];
    Double_t *effMC = new Double_t[nx];
    Double_t *effMCLowErr = new Double_t[nx];
    Double_t *effMCHighErr = new Double_t[nx];
    Double_t *effData = new Double_t[nx];
    Double_t *effDataLowErr = new Double_t[nx];
    Double_t *effDataHighErr = new Double_t[nx];
    Double_t *SF = new Double_t[nx];
    Double_t *SFLowErr = new Double_t[nx];
    Double_t *SFHighErr = new Double_t[nx];
    for(Int_t ix=1; ix<=nx; ix++) {
      etabins[ix-1] = hMCEff->GetXaxis()->GetBinCenter(ix);
      etabinsLowErr[ix-1] = hMCEff->GetXaxis()->GetBinCenter(ix) - hMCEff->GetXaxis()->GetBinLowEdge(ix);
      etabinsHighErr[ix-1] = hMCEff->GetXaxis()->GetBinUpEdge(ix) - hMCEff->GetXaxis()->GetBinCenter(ix) ;
      effMC[ix-1] = hMCEff->GetBinContent(ix,iy);
      effMCLowErr[ix-1] = hMCErrl->GetBinContent(ix,iy);
      effMCHighErr[ix-1] = hMCErrh->GetBinContent(ix,iy);
      effData[ix-1] = hDataEff2->GetBinContent(ix,iy);
      effDataLowErr[ix-1] = hDataErrl2->GetBinContent(ix,iy);
      effDataHighErr[ix-1] = min( hDataErrh2->GetBinContent(ix,iy) , 1.0 - effData[ix-1] );
      if ( effMC[ix-1] > 0 && effData[ix-1]>0) {
	SF[ix-1] = effData[ix-1] / effMC[ix-1];
	SFLowErr[ix-1] = SF[ix-1]*sqrt( pow(effMCLowErr[ix-1]/effMC[ix-1],2) + pow(effDataLowErr[ix-1]/effData[ix-1],2));
	SFHighErr[ix-1]= SF[ix-1]*sqrt( pow(effMCHighErr[ix-1]/effMC[ix-1],2) + pow(effDataHighErr[ix-1]/effData[ix-1],2));
      } else {
	SF[ix-1] = 0;
	SFLowErr[ix-1] = 0;
	SFHighErr[ix-1]= 0;
      }

      cout << "eta: " << ix << " : " << etabins[ix-1] << " " << effMC[ix-1] << " " << effMCLowErr[ix-1] << " " << effMCHighErr[ix-1] << "\n";
      cout << "eta: " << ix << " : " << etabins[ix-1] << " " << effData[ix-1] << " " << effDataLowErr[ix-1] << " " << effDataHighErr[ix-1] << "\n";

    }

    TGraphAsymmErrors* MCEffVsEta = new TGraphAsymmErrors(nx+1, etabins, effMC, etabinsLowErr, etabinsHighErr, effMCLowErr, effMCHighErr);
    TGraphAsymmErrors* DataEffVsEta = new TGraphAsymmErrors(nx+1, etabins, effData, etabinsLowErr, etabinsHighErr, effDataLowErr, effDataHighErr);
    TGraphAsymmErrors* SFVsEta = new TGraphAsymmErrors(nx+1, etabins, SF, etabinsLowErr, etabinsHighErr, SFLowErr, SFHighErr);

    TCanvas *cv = new TCanvas("cv","cv", 800,600);
    cv->SetHighLightColor(2);
    cv->SetFillColor(0);
    cv->SetBorderMode(0);
    cv->SetBorderSize(2);
    cv->SetLeftMargin(0.10);
    cv->SetRightMargin(0.3);
    cv->SetTopMargin(0.07);
    cv->SetBottomMargin(0.12);
    cv->SetFrameBorderMode(0);  

    TLegend *legend = new TLegend(0.6,0.7,0.85,0.9);
    legend->SetTextSize(0.05);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    
    TPad *pad1 = new TPad("pad1","pad1", 0,0.40,1,1);
    pad1->SetBottomMargin(0.025);
    pad1->SetLeftMargin(0.10);
    pad1->SetRightMargin(0.04);
    pad1->Draw();
    pad1->cd();
    
   
    cout << "Bin : " << ptBinString << "\n";
    MCEffVsEta->SetTitle("");
    MCEffVsEta->Draw("ap");
    MCEffVsEta->GetXaxis()->SetRangeUser(0,2.5);
    MCEffVsEta->GetYaxis()->SetRangeUser(EffLowRange,EffHighRange);
    MCEffVsEta->GetXaxis()->SetLabelSize(0);
    MCEffVsEta->GetXaxis()->SetTitle("");
    MCEffVsEta->GetYaxis()->SetTitle("Efficiency");
    MCEffVsEta->GetYaxis()->SetTitleSize(0.07);
    MCEffVsEta->GetYaxis()->SetTitleOffset(0.65);
    DataEffVsEta->SetMarkerStyle(23);
    DataEffVsEta->SetMarkerColor(kRed);
    DataEffVsEta->SetLineColor(kRed);
    DataEffVsEta->Draw("psame");

    TLatex *tex = new TLatex();
    tex->SetNDC();
    tex->SetTextSize(0.060);
    tex->SetTextFont(42);
    tex->SetTextColor(kBlack);   
    tex->DrawLatex(0.17, 0.80, ptBinString.c_str());
 
    legend->AddEntry(MCEffVsEta, "Simulation","LP");
    legend->AddEntry(DataEffVsEta, "Data","LP");
    legend->Draw();

    cv->cd();
    cv->Update();
    
    TPad *pad2 = new TPad("pad2","pad2", 0,0,1,0.4);
    pad2->SetTopMargin(0.05);
    pad2->SetLeftMargin(0.10);
    pad2->SetBottomMargin(0.25);
    pad2->SetRightMargin(0.04);    
    pad2->Draw();
    pad2->cd();

    SFVsEta->SetTitle("");
    SFVsEta->Draw("ap");
    SFVsEta->GetXaxis()->SetRangeUser(0,2.5);
    SFVsEta->GetYaxis()->SetTitle("Data/MC Scale Factor");
    SFVsEta->GetYaxis()->SetRangeUser(0.8,1.2);
    SFVsEta->GetYaxis()->SetTitleSize(0.085);
    SFVsEta->GetYaxis()->SetTitleOffset(0.5);
    SFVsEta->GetXaxis()->SetTitle("#eta");
    SFVsEta->GetXaxis()->SetTitleSize(0.11);
    SFVsEta->GetXaxis()->SetLabelSize(0.10);
    SFVsEta->GetXaxis()->SetTitleOffset(0.8);

    cv->SaveAs(Form("%s/EfficiencyComparison_PtBin%d.gif",outputDir.c_str(),iy));

  }



  // //--------------------------------------------------------------------------------------------------------------
  // // Produce 1D comparative plots vs NPV
  // //==============================================================================================================   
 
  // TCanvas *cv = new TCanvas("cv","cv", 800,600);
  
  // TLegend *legend = new TLegend(0.6,0.7,0.85,0.9);
  // legend->SetTextSize(0.05);
  // legend->SetBorderSize(0);
  // legend->SetFillStyle(0);

  // MCEffVsNVtx->SetTitle("");
  // MCEffVsNVtx->Draw("AP");
  // DataEffVsNVtx->Draw("psame");

  // MCEffVsNVtx->GetXaxis()->SetRangeUser(0,30);
  // MCEffVsNVtx->GetXaxis()->SetTitle("Number of Reconstructed Primary Vertices");
  // MCEffVsNVtx->GetXaxis()->SetTitleOffset(1.0);
  // MCEffVsNVtx->GetYaxis()->SetRangeUser(0.90,1.05);
  // MCEffVsNVtx->GetYaxis()->SetTitle("Efficiency");
  // MCEffVsNVtx->GetYaxis()->SetTitleOffset(1.4);

  // DataEffVsNVtx->SetMarkerColor(kRed);
  // DataEffVsNVtx->SetLineColor(kRed);

  // legend->AddEntry(MCEffVsNVtx, "Simulation","LP");
  // legend->AddEntry(DataEffVsNVtx, "Data","LP");
  // legend->Draw();

  // cv->SaveAs(Form("%s/EfficiencyComparison_vs_NVtx.gif",outputDir.c_str()));


  //--------------------------------------------------------------------------------------------------------------
  // Produce Text file table
  //==============================================================================================================   

  ofstream txtfile;
  txtfile.open(outfname.Data());
  assert(txtfile.is_open());
    
  
  txtfile << " pT        ";
  txtfile << " eta           ";
  txtfile << "    MC T&P           ";
  txtfile << "    Data T&P         ";  
  txtfile << "    Scale factor     ";
  txtfile << endl;
  txtfile << "----------------------------------------------------------------------------------------------------------------------------------------------------";
  txtfile << endl;
  for(Int_t iy=1; iy<=ny; iy++) {
    for(Int_t ix=1; ix<=nx; ix++) {
      txtfile << "[" << setw(4) << hMCEff->GetYaxis()->GetBinLowEdge(iy) << "," << setw(4) << hMCEff->GetYaxis()->GetBinLowEdge(iy+1) << "]";
      txtfile << "[" << setw(6) << hMCEff->GetXaxis()->GetBinLowEdge(ix) << "," << setw(6) << hMCEff->GetXaxis()->GetBinLowEdge(ix+1) << "]";      
      ios_base::fmtflags flags = txtfile.flags();
      txtfile.precision(4);
      
      Double_t mceff  = hMCEff->GetBinContent(ix,iy);
      Double_t mcerrl = hMCErrl->GetBinContent(ix,iy);
      Double_t mcerrh = hMCErrh->GetBinContent(ix,iy);
      txtfile << " " << setw(9) << fixed << mceff << " +/- " << TMath::Max(mcerrl,mcerrh);

      Double_t dataeff  = hDataEff2->GetBinContent(ix,iy);
      Double_t dataerrl = hDataErrl2->GetBinContent(ix,iy);
      Double_t dataerrh = hDataErrh2->GetBinContent(ix,iy);
      txtfile << " " << setw(9) << fixed << dataeff << " +/- " << TMath::Max(dataerrl,dataerrh);
      
      Double_t scale     = (hDataEff2->GetBinContent(ix,iy))/(hMCEff->GetBinContent(ix,iy));
      Double_t scaleErrl = scale*sqrt(mcerrl*mcerrl/mceff/mceff + dataerrl*dataerrl/dataeff/dataeff);
      Double_t scaleErrh = scale*sqrt(mcerrh*mcerrh/mceff/mceff + dataerrh*dataerrh/dataeff/dataeff);

      txtfile << " " << setw(9) << fixed << scale << " +/- " << TMath::Max(scaleErrl,scaleErrh);

      txtfile << endl;
      txtfile.flags(flags);

      cout << "Set " << iy << " " << ix << " " << scale << endl;
      h2_results_selection->SetBinContent(iy,ix, scale);
      h2_results_selection->SetBinError(iy,ix, (scaleErrl + scaleErrh)/2);
      //fill overflow bins with the same values as last bin
      if (ix == nx) {
      cout << "Set " << iy << " " << ix+1 << " " << scale << endl;
        h2_results_selection->SetBinContent(iy,nx+1, scale);
        h2_results_selection->SetBinError(iy,nx+1, (scaleErrl + scaleErrh)/2);
      }
    }
    if (iy == ny) {
      for(Int_t ix=1; ix<=nx; ix++) {
        Double_t mceff  = hMCEff->GetBinContent(ix,iy);
        Double_t mcerrl = hMCErrl->GetBinContent(ix,iy);
        Double_t mcerrh = hMCErrh->GetBinContent(ix,iy);
        Double_t dataeff  = hDataEff2->GetBinContent(ix,iy);
        Double_t dataerrl = hDataErrl2->GetBinContent(ix,iy);
        Double_t dataerrh = hDataErrh2->GetBinContent(ix,iy);
        Double_t scale     = (hDataEff2->GetBinContent(ix,iy))/(hMCEff->GetBinContent(ix,iy));
        Double_t scaleErrl = scale*sqrt(mcerrl*mcerrl/mceff/mceff + dataerrl*dataerrl/dataeff/dataeff);
        Double_t scaleErrh = scale*sqrt(mcerrh*mcerrh/mceff/mceff + dataerrh*dataerrh/dataeff/dataeff);
        
        cout << "Set " << iy+1 << " " << ix << " " << scale << endl;
        h2_results_selection->SetBinContent(iy+1,ix, scale);
        h2_results_selection->SetBinError(iy+1,ix, (scaleErrl + scaleErrh)/2);
        //fill overflow bins with the same values as last bin
        if (ix == nx) {
          cout << "Set " << iy+1 << " " << ix+1 << " " << scale << endl;
          h2_results_selection->SetBinContent(iy+1,nx+1, scale);
          h2_results_selection->SetBinError(iy+1,nx+1, (scaleErrl + scaleErrh)/2);
        }
      }
    }

    txtfile << endl;
  }
  txtfile.close();
  
  cout << outfname << " created!" << endl;
  



  //--------------------------------------------------------------------------------------------------------------
  // Create TEX table
  //==============================================================================================================   

  ofstream texfile;
  texfile.open((outputDir + "/eff_table.tex").c_str());
  assert(texfile.is_open());

  texfile << " \\begin{table}[!ht]" << endl;
  texfile << " \\begin{center} " << endl;
  texfile << " \\begin{tabular}{|c|c|c|c|}" << endl;
  texfile << " \\hline\n";


  texfile << " $p_{T}$ / $\\eta$ bin    &  Monte Carlo Efficiency    &  Data Efficiency   &  MC to Data Scale Factor \\\\  ";
  texfile << " \\hline           ";
  texfile << endl;
  for(Int_t iy=1; iy<=ny; iy++) {
    for(Int_t ix=1; ix<=nx; ix++) {

      string binLabel = Form("$%5.1f < p_{T} \\le %5.1f$ , $%5.1f  \\le |\\eta| < %5.1f$", 
                             hMCEff->GetYaxis()->GetBinLowEdge(iy), hMCEff->GetYaxis()->GetBinLowEdge(iy+1),
                             hMCEff->GetXaxis()->GetBinLowEdge(ix), hMCEff->GetXaxis()->GetBinLowEdge(ix+1));
      if (iy == ny) {
        binLabel = Form("$%5.1f < p_{T} $ , $%5.1f  \\le |\\eta| < %5.1f$", 
                        hMCEff->GetYaxis()->GetBinLowEdge(iy), 
                        hMCEff->GetXaxis()->GetBinLowEdge(ix), hMCEff->GetXaxis()->GetBinLowEdge(ix+1));
      }
      
      texfile << binLabel;
      texfile << "   &   ";

      ios_base::fmtflags flags = texfile.flags();
      texfile.precision(4);
      
      Double_t mceff  = hMCEff->GetBinContent(ix,iy);
      Double_t mcerrl = hMCErrl->GetBinContent(ix,iy);
      Double_t mcerrh = hMCErrh->GetBinContent(ix,iy);
      texfile << " " << setw(9) << fixed << mceff << " +/- " << TMath::Max(mcerrl,mcerrh);
      texfile << "   &   ";

      Double_t dataeff  = hDataEff2->GetBinContent(ix,iy);
      Double_t dataerrl = hDataErrl2->GetBinContent(ix,iy);
      Double_t dataerrh = hDataErrh2->GetBinContent(ix,iy);
      texfile << " " << setw(9) << fixed << dataeff << " +/- " << TMath::Max(dataerrl,dataerrh);
      texfile << "   &   ";
     
      Double_t scale     = (hDataEff2->GetBinContent(ix,iy))/(hMCEff->GetBinContent(ix,iy));
      Double_t scaleErrl = scale*sqrt(mcerrl*mcerrl/mceff/mceff + dataerrl*dataerrl/dataeff/dataeff);
      Double_t scaleErrh = scale*sqrt(mcerrh*mcerrh/mceff/mceff + dataerrh*dataerrh/dataeff/dataeff);
      texfile << " " << setw(9) << fixed << scale << " +/- " << TMath::Max(scaleErrl,scaleErrh);
      texfile << "   \\\\   ";
      texfile << endl;
      texfile << "\\hline";
      texfile << endl;

    }
  }
  texfile << "\\end{tabular}" << endl;
  texfile << "\\caption{CAPTION.}" << endl;
  texfile << "\\label{tab:eff_ele_offline}" << endl;
  texfile << "\\end{center}" << endl;
  texfile << "\\end{table}" << endl;
  texfile.close();
  cout << outputDir + "/eff_table.tex" << " created!" << endl;




  //--------------------------------------------------------------------------------------------------------------
  // Create TWIKI table
  //==============================================================================================================   

  ofstream twikifile;
  twikifile.open((outputDir + "/eff_table.twiki").c_str());
  assert(twikifile.is_open());
  

  twikifile << "| *pT bin* | *eta bin* | *MC Efficiency* | *Data Efficiency* | *ScaleFactor* | \n";
  
  for(Int_t iy=1; iy<=ny; iy++) {
    for(Int_t ix=1; ix<=nx; ix++) {

      string binLabel = Form("| *%4.1f < pT \\le %4.1f* | *%3.1f  <= eta < %3.1f* | ", 
                             hMCEff->GetYaxis()->GetBinLowEdge(iy), hMCEff->GetYaxis()->GetBinLowEdge(iy+1),
                             hMCEff->GetXaxis()->GetBinLowEdge(ix), hMCEff->GetXaxis()->GetBinLowEdge(ix+1));
      if (iy == ny) {
        binLabel = Form("| *%4.1f < pT* | *%3.1f  <= eta < %3.1f* | ", 
                        hMCEff->GetYaxis()->GetBinLowEdge(iy), 
                        hMCEff->GetXaxis()->GetBinLowEdge(ix), hMCEff->GetXaxis()->GetBinLowEdge(ix+1));
      }
      
      twikifile << binLabel ;

      ios_base::fmtflags flags = twikifile.flags();
      twikifile.precision(4);
      
      Double_t mceff  = hMCEff->GetBinContent(ix,iy);
      Double_t mcerrl = hMCErrl->GetBinContent(ix,iy);
      Double_t mcerrh = hMCErrh->GetBinContent(ix,iy);
      twikifile << " " << setw(9) << fixed << mceff << " +/- " << TMath::Max(mcerrl,mcerrh);
      twikifile << " | ";

      Double_t dataeff  = hDataEff2->GetBinContent(ix,iy);
      Double_t dataerrl = hDataErrl2->GetBinContent(ix,iy);
      Double_t dataerrh = hDataErrh2->GetBinContent(ix,iy);
      twikifile << " " << setw(9) << fixed << dataeff << " +/- " << TMath::Max(dataerrl,dataerrh);
      twikifile << " | ";
     
      Double_t scale     = (hDataEff2->GetBinContent(ix,iy))/(hMCEff->GetBinContent(ix,iy));
      Double_t scaleErrl = scale*sqrt(mcerrl*mcerrl/mceff/mceff + dataerrl*dataerrl/dataeff/dataeff);
      Double_t scaleErrh = scale*sqrt(mcerrh*mcerrh/mceff/mceff + dataerrh*dataerrh/dataeff/dataeff);
      twikifile << " " << setw(9) << fixed << scale << " +/- " << TMath::Max(scaleErrl,scaleErrh);
      twikifile << " |\n";

    }
  }
  twikifile.close();
  cout << outputDir + "/eff_table.twiki" << " created!" << endl;

  outputFile->WriteTObject(h2_results_selection, h2_results_selection->GetName(), "WriteDelete");
  outputFile->Close();

}



void makeEfficiencyPlot(string filename,
		    string outputDir, 
		    string histName,
		    string Label,
		    double PtLowRange = 5,
		    double PtHighRange = 500,
		    double EffLowRange = 0.8,
		    double EffHighRange = 1.15
		    )
{
  
  string label = Label;
  if (Label != "") label = "_" + Label;

  //--------------------------------------------------------------------------------------------------------------
  // Settings 
  //============================================================================================================== 
  TFile datafile(filename.c_str());
  
  //--------------------------------------------------------------------------------------------------------------
  // Main analysis code 
  //==============================================================================================================   
  
  TH2F *hDataEff=0, *hDataErrl=0, *hDataErrh=0;  
  hDataEff  = (TH2F*)datafile.Get("hEffEtaPt");
  hDataErrl = (TH2F*)datafile.Get("hErrlEtaPt");
  hDataErrh = (TH2F*)datafile.Get("hErrhEtaPt");
  
  //--------------------------------------------------------------------------------------------------------------
  // Update root file histograms
  //==============================================================================================================   
  const Int_t nx = hDataEff->GetNbinsX();
  const Int_t ny = hDataEff->GetNbinsY();

  //Do Binning
  Double_t *ptbins = new Double_t[ny+1];
  Double_t *etabins = new Double_t[nx+1];
  for(Int_t iy=1; iy<=ny; iy++) {
    ptbins[iy-1] = fmax(fmin(hDataEff->GetYaxis()->GetBinLowEdge(iy),PtHighRange),PtLowRange);
  }
  for(Int_t ix=1; ix<=nx; ix++) {
    etabins[ix-1]= hDataEff->GetXaxis()->GetBinLowEdge(ix);
  }
  ptbins[ny] = 60;
  etabins[nx] = 2.5;
    
  cout << "ptbins : ";
  for(Int_t i=0; i<ny+1;++i) {
    cout << ptbins[i] << " ";
  }
  cout << endl;
  cout << "etabins : ";
  for(Int_t i=0; i<nx+1;++i) {
    cout << etabins[i] << " ";
  }
  cout << endl;




  //--------------------------------------------------------------------------------------------------------------
  // Produce 1D comparative plots vs Pt
  //==============================================================================================================   
  for(Int_t ix=1; ix<=nx; ix++) {
    string etaBinString = Form("%.1f #leq |#eta| #leq %.1f",  hDataEff->GetXaxis()->GetBinLowEdge(ix),  hDataEff->GetXaxis()->GetBinUpEdge(ix));
    Double_t *ptbins = new Double_t[ny];
    Double_t *ptbinsLowErr = new Double_t[ny];
    Double_t *ptbinsHighErr = new Double_t[ny];
    Double_t *effData = new Double_t[ny];
    Double_t *effDataLowErr = new Double_t[ny];
    Double_t *effDataHighErr = new Double_t[ny];
    for(Int_t iy=1; iy<=ny; iy++) {
      ptbins[iy-1] = hDataEff->GetYaxis()->GetBinCenter(iy);
      ptbinsLowErr[iy-1] = hDataEff->GetYaxis()->GetBinCenter(iy) - hDataEff->GetYaxis()->GetBinLowEdge(iy);
      ptbinsHighErr[iy-1] = hDataEff->GetYaxis()->GetBinUpEdge(iy) - hDataEff->GetYaxis()->GetBinCenter(iy) ;
      effData[iy-1] = hDataEff->GetBinContent(ix,iy);
      effDataLowErr[iy-1] = hDataErrl->GetBinContent(ix,iy);
      effDataHighErr[iy-1] = min( hDataErrh->GetBinContent(ix,iy) , 1.0 - effData[iy-1] ); 
      cout << "pt: " << iy << " : " << ptbins[iy-1] << " : " << effData[iy-1] << " " << effDataLowErr[iy-1] << " " << effDataHighErr[iy-1] << "\n";

    }
    TGraphAsymmErrors* DataEffVsPt = new TGraphAsymmErrors(ny+1, ptbins, effData, ptbinsLowErr, ptbinsHighErr, effDataLowErr, effDataHighErr);


    TCanvas *cv = new TCanvas("cv","cv", 800,600);
    // cv->SetHighLightColor(2);
    // cv->SetFillColor(0);
    // cv->SetBorderMode(0);
    // cv->SetBorderSize(2);
    // cv->SetLeftMargin(0.10);
    // cv->SetRightMargin(0.3);
    // cv->SetTopMargin(0.07);
    // cv->SetBottomMargin(0.12);
    // cv->SetFrameBorderMode(0);  

    TLegend *legend = new TLegend(0.6,0.7,0.85,0.9);
    legend->SetTextSize(0.05);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    
    cout << "Bin : " << etaBinString << "\n";
    DataEffVsPt->SetTitle("");
    DataEffVsPt->Draw("ap");
    DataEffVsPt->GetXaxis()->SetRangeUser(PtLowRange,PtHighRange);
    DataEffVsPt->GetYaxis()->SetRangeUser(EffLowRange,EffHighRange);
    //DataEffVsPt->GetXaxis()->SetLabelSize(0);
    DataEffVsPt->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    DataEffVsPt->GetYaxis()->SetTitle("Efficiency");
    DataEffVsPt->GetYaxis()->SetTitleSize(0.07);
    DataEffVsPt->GetYaxis()->SetTitleOffset(0.85);  

    TLatex *tex = new TLatex();
    tex->SetNDC();
    tex->SetTextSize(0.060);
    tex->SetTextFont(42);
    tex->SetTextColor(kBlack);   
    tex->DrawLatex(0.20, 0.83, etaBinString.c_str());
 
    //cv->SetLogx();
    cv->SaveAs(Form("%s/Efficiency_EtaBin%d.gif",outputDir.c_str(),ix));
  }


  //--------------------------------------------------------------------------------------------------------------
  // Produce 1D comparative plots vs Eta
  //==============================================================================================================   
  for(Int_t iy=1; iy<=ny; iy++) {
    string ptBinString = Form("%.0f #leq p_{T} #leq %.0f",  hDataEff->GetYaxis()->GetBinLowEdge(iy),  hDataEff->GetYaxis()->GetBinUpEdge(iy));
    Double_t *etabins = new Double_t[nx];
    Double_t *etabinsLowErr = new Double_t[nx];
    Double_t *etabinsHighErr = new Double_t[nx];
    Double_t *effData = new Double_t[nx];
    Double_t *effDataLowErr = new Double_t[nx];
    Double_t *effDataHighErr = new Double_t[nx];
    for(Int_t ix=1; ix<=nx; ix++) {
      etabins[ix-1] = hDataEff->GetXaxis()->GetBinCenter(ix);
      etabinsLowErr[ix-1] = hDataEff->GetXaxis()->GetBinCenter(ix) - hDataEff->GetXaxis()->GetBinLowEdge(ix);
      etabinsHighErr[ix-1] = hDataEff->GetXaxis()->GetBinUpEdge(ix) - hDataEff->GetXaxis()->GetBinCenter(ix) ;
      effData[ix-1] = hDataEff->GetBinContent(ix,iy);
      effDataLowErr[ix-1] = hDataErrl->GetBinContent(ix,iy);
      effDataHighErr[ix-1] = min( hDataErrh->GetBinContent(ix,iy) , 1.0 - effData[ix-1] );
      cout << "eta: " << ix << " : " << etabins[ix-1] << " " << effData[ix-1] << " " << effDataLowErr[ix-1] << " " << effDataHighErr[ix-1] << "\n";

    }

    TGraphAsymmErrors* DataEffVsEta = new TGraphAsymmErrors(nx+1, etabins, effData, etabinsLowErr, etabinsHighErr, effDataLowErr, effDataHighErr);

    TCanvas *cv = new TCanvas("cv","cv", 800,600);
    // cv->SetHighLightColor(2);
    // cv->SetFillColor(0);
    // cv->SetBorderMode(0);
    // cv->SetBorderSize(2);
    // cv->SetLeftMargin(0.10);
    // cv->SetRightMargin(0.3);
    // cv->SetTopMargin(0.07);
    // cv->SetBottomMargin(0.12);
    // cv->SetFrameBorderMode(0);  

    TLegend *legend = new TLegend(0.6,0.7,0.85,0.9);
    legend->SetTextSize(0.05);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    
    cout << "Bin : " << ptBinString << "\n";
    DataEffVsEta->SetTitle("");
    DataEffVsEta->Draw("ap");
    DataEffVsEta->GetXaxis()->SetRangeUser(0,2.5);
    DataEffVsEta->GetYaxis()->SetRangeUser(EffLowRange,EffHighRange);
    DataEffVsEta->GetXaxis()->SetLabelSize(0);
    DataEffVsEta->GetXaxis()->SetTitle("|#eta|");
    DataEffVsEta->GetYaxis()->SetTitle("Efficiency");
    DataEffVsEta->GetYaxis()->SetTitleSize(0.07);
    DataEffVsEta->GetYaxis()->SetTitleOffset(0.65);

    TLatex *tex = new TLatex();
    tex->SetNDC();
    tex->SetTextSize(0.060);
    tex->SetTextFont(42);
    tex->SetTextColor(kBlack);   
    tex->DrawLatex(0.17, 0.80, ptBinString.c_str());
 
    cv->SaveAs(Form("%s/Efficiency_PtBin%d.gif",outputDir.c_str(),iy));

  }



}
