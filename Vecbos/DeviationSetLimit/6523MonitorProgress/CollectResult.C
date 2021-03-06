#include <vector>
#include <string>
using namespace std;

#include "TGraph.h"
#include "TGraphErrors.h"
#include "TCanvas.h"

#include "DataHelper.h"
#include "PlotHelper.h"

void CollectResult()
{
   vector<int> JetThresholds;
   JetThresholds.push_back(15);
   JetThresholds.push_back(16);
   JetThresholds.push_back(17);
   JetThresholds.push_back(18);
   JetThresholds.push_back(19);
   JetThresholds.push_back(20);
   JetThresholds.push_back(22);
   JetThresholds.push_back(24);
   JetThresholds.push_back(26);
   JetThresholds.push_back(28);
   JetThresholds.push_back(30);
   JetThresholds.push_back(35);
   JetThresholds.push_back(40);

   PsFileHelper AllPsFile("Summary.ps");
   AllPsFile.AddTextPage("Overview of fitting progress for all jet thresholds");

   for(int a = 0; a < (int)JetThresholds.size(); a++)
   {
      int JetThreshold = JetThresholds[a];

      char JetThresholdString[100] = "";
      sprintf(JetThresholdString, "%d", JetThreshold);
   
      DataHelper DHFile("Database.dh");
   
      PsFileHelper PsFile(string("MinLLResult_") + JetThresholdString + ".ps");
      PsFile.AddTextPage(string("Attempt at setting limit, jet threshold ") + JetThresholdString);
   
      TGraph MinLLGraph;
      MinLLGraph.SetNameTitle("MinLLGraph", Form("MinLLGraph, Jet Threshold %d", JetThreshold));
      
      TGraph DeltaMinLLGraph;
      DeltaMinLLGraph.SetNameTitle("DeltaMinLLGraph", Form("DeltaMinLLGraph, Threshold %d", JetThreshold));
   
      TGraph LimitGraph;
      LimitGraph.SetNameTitle("LimitGraph", "LimitGraph");
      LimitGraph.SetPoint(0, -200, 1.96);
      LimitGraph.SetPoint(1, 600, 1.96);
   
      TGraphErrors AlphaGraph;
      AlphaGraph.SetNameTitle("AlphaGraph", "AlphaGraph");
      
      TGraphErrors BetaGraph;
      BetaGraph.SetNameTitle("BetaGraph", "BetaGraph");
   
      TGraph CorrelationGraph;
      CorrelationGraph.SetNameTitle("Correlation", "Correlation");
   
      double MinNLL = 10000000;
   
      vector<string> States = DHFile.GetListOfKeys();
      for(int i = 0; i < (int)States.size(); i++)
      {
         if(JetThreshold != DHFile[States[i]]["JetThreshold"].GetInteger())
            continue;
         if(MinNLL > DHFile[States[i]]["MinNLL"].GetDouble())
            MinNLL = DHFile[States[i]]["MinNLL"].GetDouble();
      }
   
      int EntryCount = 0;
   
      for(int i = 0; i < (int)States.size(); i++)
      {
         if(JetThreshold != DHFile[States[i]]["JetThreshold"].GetInteger())
            continue;
         
         CorrelationGraph.SetPoint(i, DHFile[States[i]]["ExtraTerm"].GetDouble(),
            DHFile[States[i]]["AlphaBetaCorrelation"].GetDouble());
         
         double Correlation = DHFile[States[i]]["AlphaBetaCorrelation"].GetDouble();
         // if(Correlation > -0.5)
         //    continue;
   
         MinLLGraph.SetPoint(EntryCount, DHFile[States[i]]["ExtraTerm"].GetDouble(),
            DHFile[States[i]]["MinNLL"].GetDouble());
         
         DeltaMinLLGraph.SetPoint(EntryCount, DHFile[States[i]]["ExtraTerm"].GetDouble(),
            DHFile[States[i]]["MinNLL"].GetDouble() - MinNLL);
   
         AlphaGraph.SetPoint(EntryCount, DHFile[States[i]]["ExtraTerm"].GetDouble(),
            DHFile[States[i]]["Alpha"].GetDouble());
         AlphaGraph.SetPointError(EntryCount, 0, DHFile[States[i]]["AlphaError"].GetDouble());
         
         BetaGraph.SetPoint(EntryCount, DHFile[States[i]]["ExtraTerm"].GetDouble(),
            DHFile[States[i]]["Beta"].GetDouble());
         BetaGraph.SetPointError(EntryCount, 0, DHFile[States[i]]["BetaError"].GetDouble());
         
         EntryCount = EntryCount + 1;
      }
   
      MinLLGraph.SetMarkerStyle(20);
      PsFile.AddPlot(MinLLGraph, "ap");
      
      TCanvas DeltaMinLLCanvas("DeltaMinLLCanvas", "DeltaMinLLCanvas", 0, 0, 1024, 768);
      DeltaMinLLGraph.SetMarkerStyle(20);
      DeltaMinLLGraph.Draw("ap");
      DeltaMinLLGraph.GetHistogram()->GetXaxis()->SetTitle("Extra term in the 4-jet (inclusive) bin");
      DeltaMinLLGraph.GetHistogram()->GetYaxis()->SetTitle("#Delta log(likelihood)");
      if(DeltaMinLLGraph.GetHistogram()->GetMaximum() > 10)
         DeltaMinLLGraph.GetHistogram()->SetMaximum(10);
      LimitGraph.Draw("l");
      PsFile.AddCanvas(DeltaMinLLCanvas);

      AlphaGraph.SetMarkerStyle(20);
      AlphaGraph.SetLineWidth(2);
      PsFile.AddPlot(AlphaGraph, "ap");
      
      BetaGraph.SetMarkerStyle(20);
      BetaGraph.SetLineWidth(2);
      PsFile.AddPlot(BetaGraph, "ap");
   
      CorrelationGraph.SetMarkerStyle(20);
      PsFile.AddPlot(CorrelationGraph, "ap");
   
      PsFile.AddTimeStampPage();
      PsFile.Close();

      AllPsFile.AddPlot(MinLLGraph, "ap");
   }

   AllPsFile.AddTimeStampPage();
   AllPsFile.Close();
}





