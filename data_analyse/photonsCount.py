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
h1.Draw()
c1.SaveAs("photonsCount.png")
