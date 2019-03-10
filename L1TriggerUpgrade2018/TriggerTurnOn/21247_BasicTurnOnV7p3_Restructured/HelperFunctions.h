#include <iostream>
#include <vector>
using namespace std;

#include "fastjet/ClusterSequence.hh"

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TDirectory.h"

#include "Code/TauHelperFunctions3.h"

#include "Messenger.h"

#include "Histograms.h"

#define QUAL_NONE -1
#define QUAL_ODD -2
#define QUAL_12 -3
#define QUAL_BARRELODDENDCAP2 -4
#define QUAL_USEFLAG -5
#define QUAL_BARRELNONEENDCAP2 -6

GenericObject BestInRange(vector<GenericObject> &List, double AbsEta);
GenericObject BestInRange(vector<GenericObject> &List, double AbsEtaMin, double AbsEtaMax);
GenericObject BestInRange(vector<GenericObject> &List, FourVector &Reference, double MaxEta = 999, double DRMax = 0.5, int Qual = QUAL_NONE, int Type = -1, double IsoBB = -1, double IsoEC = -1);
FourVector CalculateHT(vector<FourVector> &GenJets, double Eta, double PT);
FourVector CalculateMHT(vector<FourVector> &GenJets, double Eta, double PT);
FourVector CalculateHT(vector<GenericObject> &GenJets, double Eta, double PT);
FourVector CalculateMHT(vector<GenericObject> &GenJets, double Eta, double PT);
void FillHistograms(Histograms *Histogram, FourVector &Object, FourVector &Reference, double Isolation = -1, int Type = -1, double DXY = -1);
void FillHistograms(Histograms *Histogram, FourVector &Object, double Isolation = -1, int Type = -1, double DXY = -1);
void FillHistograms(Histograms &Histogram, FourVector &Object, FourVector &Reference, double Isolation = -1, int Type = -1, double DXY = -1);
void FillHistograms(Histograms &Histogram, FourVector &Object, double Isolation = -1, int Type = -1, double DXY = -1);
vector<FourVector> ClusterJets(vector<FourVector> &Particles);
double CalculateGenIsolation(FourVector &P, vector<FourVector> &Particles, double DR = 0.3);
vector<FourVector> CleanUpTaus(vector<FourVector> &Taus, vector<FourVector> &Avoid, double DR = 0.3);
FourVector GetVisTau(L1GenMessenger &MGen, int Index);
FourVector GetVisTauAdhoc(L1GenMessenger &MGen, int Index);

