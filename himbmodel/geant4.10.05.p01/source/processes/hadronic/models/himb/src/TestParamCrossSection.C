#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

//ROOT includes
#include "TROOT.h"
#include "TStyle.h"
#include "TString.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TF1.h"
#include "TF2.h"
#include "TMath.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "TVector.h"
#include "TList.h"

//Declare a function that is implemented below:
Double_t DiffCrossSectionHE(Double_t *x, Double_t *par);
Double_t DiffCrossSectionLE(Double_t *x, Double_t *par);
Double_t DiffCrossSection(Double_t *x, Double_t *par);
Double_t TotCrossSection(Double_t *x, Double_t *par);



void TestParamCrossSection()
{
	gROOT->Reset();
    gStyle->SetPalette(1);
    //gStyle->SetOptTitle(kFALSE);
    gStyle->SetOptStat(0);
    
    TF1 *fHE = new TF1("fHE",DiffCrossSectionHE,0,600,3);
    fHE->SetParameters(90,585,6);
    fHE->SetParNames("theta","Tp","Z");
    
    TF1 *fLE = new TF1("fLE",DiffCrossSectionLE,0,50,1);
    fLE->SetParameter(0,90);
    fLE->SetParNames("theta");
    
    TF1 *fComb = new TF1("fComb",DiffCrossSection,0,200,3);
    fComb->SetParameters(90,585,6);
    fComb->SetParNames("theta","Tp","Z");

    TF1 *fTot = new TF1("fTot",TotCrossSection,0,800,1);
    fTot->SetParameter(0,6);
    fTot->SetParNames("Z");
    
    
    fTot->Draw();

}

Double_t DiffCrossSectionHE(Double_t *x, Double_t *par){
    
    Double_t Tpi = x[0];
    Double_t theta = par[0];
    Double_t Tp = par[1];
    Double_t Z = par[2];
    
    Double_t knots[] = {0,0,0,30,70,180,180,180};
    Int_t deg = 3;
    Int_t length = 8;
    Double_t nums[] = {0,0,0,0,0,0,0,0};
    
    for (Int_t i=0; i<(length-1); ++i) {
        if (theta>=knots[i] && theta<knots[i+1]) {
            nums[i] = 1.;
        }
        else {
            nums[i] = 0.;
        }
    }
    
    for (Int_t k = 2; k<deg+1; ++k) {
        for (Int_t i=0; i<(length-k); ++i) {
            Double_t a1 = theta-knots[i];
            Double_t a2 = knots[i+k-1]-knots[i];
            Double_t b1 = knots[i+k]-theta;
            Double_t b2 = knots[i+k]-knots[i+1];
            Double_t a = 0;
            Double_t b = 0;
            if (a2!=0) {
                a = nums[i]*a1/a2;
            }
            if (b2!=0) {
                b = nums[i+1]*b1/b2;
            }
            nums[i] = a+b;
        }
    }
    
    Double_t A1 = TMath::Min(27.-4*((730-Tp)/(730.-585.))*((730-Tp)/(730.-585.)),27.0);
    Double_t A2 = 18.2;
    Double_t A3 = 8.0;
    Double_t A4 = 13+(Z-12)/10.0;
    Double_t A5 = 9+(Z-12)/10.0-(Tp-685)/20.0;
    Double_t Normalization = 0.8851*pow(log(Z),0)*pow(Z,1./3.)-0.1015*pow(log(Z),1)*pow(Z,1./3.)+0.1459*pow(log(Z),2)*pow(Z,1./3.)-0.0265*pow(log(Z),3)*pow(Z,1./3.);
    Double_t Amplitude = Normalization*(A1*nums[0]+A2*nums[1]+A3*nums[2]+A4*nums[3]+A5*nums[4]);
    
    Double_t numTA585 = 0;
    Double_t numTA730 = 0;
    Double_t numsigmaA585 = 0;
    Double_t numsigmaA730 = 0;
    if (Z==1) {
        numTA585 = 64.5;
        numTA730 = 53.0;
        numsigmaA585 = 155.0;
        numsigmaA730 = 127.0;
    }
    else if (Z>=2 && Z < 9){
        numTA585 = 28.9;
        numTA730 = 34.2;
        numsigmaA585 = 130.0;
        numsigmaA730 = 150.0;
    }
    else if (Z>=9 && Z < 92){
        numTA585 = 26.0;
        numTA730 = 29.9;
        numsigmaA585 = 135.0;
        numsigmaA730 = 160.0;
    }
    Double_t TA = (numTA730*(Tp-585)-numTA585*(Tp-730))/(730.-585.);
    Double_t sigmaA = (numsigmaA730*(Tp-585)-numsigmaA585*(Tp-730))/(730.-585.);
    
    Double_t Tbar = 48.+330.*exp(-theta/TA);
    Double_t sigma = sigmaA*exp(-theta/85.);
    Double_t B = 25.;
    Double_t TF = Tp-140.-2*B;
    
    Double_t DiffCrossSecHE = Amplitude*exp(-pow((Tbar-Tpi)/(sqrt(2)*sigma),2))*1/(1+exp((Tpi-TF)/B));
    
    return DiffCrossSecHE;
}


Double_t DiffCrossSectionLE(Double_t *x, Double_t *par){
    
    Double_t Tpi = x[0];
    Double_t theta = par[0];
    
    Double_t S01 = 15.3;
    Double_t T01 = 49.4;
    Double_t S02 = 5.6;
    Double_t T02 = 32.4;
    
    Double_t DiffCrossSecLE = S01*sin(TMath::Pi()*Tpi/2/T01) - S02*sin(TMath::Pi()*Tpi/2/T02)*cos(theta*TMath::Pi()/180);
    
    return DiffCrossSecLE;
    
}


Double_t DiffCrossSection(Double_t *x, Double_t *par){
    
    Double_t Tpi = x[0];
    Double_t theta = par[0];
    Double_t Tp = par[1];
    Double_t Z = par[2];
    
    Double_t SigSlope = 10.;
    Double_t SigCenter = 40.;
    Double_t frac = 1/(1+exp(-(Tpi-SigCenter)/SigSlope));
                       
    Double_t TpiRef = 40.;
    Double_t ParRef[3] = {90.,580.,6.};
    Double_t Pars[3] = {90.,Tp,Z};
    Double_t ScalingFactor = DiffCrossSectionHE(&TpiRef,Pars)/DiffCrossSectionHE(&TpiRef,ParRef);
    
    //cout << ScalingFactor << endl;
    
    Double_t TpiShift = 1+4./28.*Z;
    Double_t TpiLE = Tpi-TpiShift;
    Double_t ParLE = theta;
    
    Double_t DiffCrossSec = (1-frac)*ScalingFactor*DiffCrossSectionLE(&TpiLE,&ParLE) + frac*DiffCrossSectionHE(x,par);
    
    if (DiffCrossSec<0) {
        DiffCrossSec=0;
    }
    
    return DiffCrossSec;
}


Double_t TotCrossSection(Double_t *x, Double_t *par){
    
    Double_t Tp = x[0];
    Double_t Z = par[0];
    
    Double_t sig585 = 9.70;
    Double_t sig730 = 13.50;
    
    if (Z>1 && Z<12){
        sig585 = 28.5*pow(Z/6.,1./3.)*(0.77 + 0.039*Z);
        sig730 = 35.0*pow(Z/6.,1./3.)*(0.77 + 0.039*Z);
    }
    else if (Z>=12) {
        sig585 = 19.65*pow(Z,1./3.);
        sig730 = 24.50*pow(Z,1./3.);
    }
    
    Double_t TotCrossSec = 0;
    
    if (Tp>325 && Tp<=585) {
        TotCrossSec = sig585*(Tp - 325.)/(585. - 325.);
    }
    else if (Tp>585) {
        TotCrossSec = sig585 + (sig730 - sig585)*(Tp - 585.)/(730. - 585.);
    }
    
    return TotCrossSec;
}





