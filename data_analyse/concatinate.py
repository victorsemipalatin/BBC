import os
import ROOT
import uproot


c1 = ROOT.TCanvas()
start_file = os.path.join("data", "output_27.04.2025_19:08:23.root")
file = ROOT.TFile(start_file)
file_up = uproot.open(start_file)
keys =  file_up.keys()
for key in keys:
    print(key)
    h1 = file.Get(key)
    path_to_data = "data"
    for f in os.listdir(path_to_data):
        if os.path.join(path_to_data, f) != start_file and "output" in f:
            f = ROOT.TFile(os.path.join(path_to_data, f))
            h1.Add(f.Get(key))
            f.Close()
    if "PhotonsCount" in key:
        x_axis = "Photons Count per Event"
        y_axis = "Number of events"
        output = "PhotonsCount"
    elif "PhotonsEnergy" in key:
        x_axis = "Energy, eV"
        y_axis = "Number of events"
        output = "PhotonsEnergy"
    elif "CherenkovCount" in key:
        x_axis = "Cherenkov photons count per event"
        y_axis = "Number of events"
        output = "CherenkovCount"
    else:
        continue
    h1.GetXaxis().SetTitle(x_axis)
    h1.GetYaxis().SetTitle(y_axis)
    h1.Draw()
    full_file = ROOT.TFile(os.path.join("combined", f"{output}.root"), "RECREATE")
    h1.Write()
    full_file.Write()
    c1.SaveAs(os.path.join("pics", f"{output}.png"))
    full_file.Close()
