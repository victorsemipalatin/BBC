import os
import ROOT


c1 = ROOT.TCanvas()
file = ROOT.TFile("data/output0.root")
name = "PhotonsCount"
h1 = file.Get(name)
path_to_data = "data"
for f in os.listdir(path_to_data):
    if f != "output0.root":
        f = ROOT.TFile(os.path.join(path_to_data, f))
        h1.Add(f.Get(name))
h1.GetXaxis().SetTitle("Photons Count per Event");
h1.GetYaxis().SetTitle("Number of events");
h1.Draw()
full_file = ROOT.TFile("data/full.root", "new")
h1.Write()
full_file.Write()
c1.SaveAs("photonsCount.png")
