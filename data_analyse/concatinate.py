import os
import ROOT
import uproot


c1 = ROOT.TCanvas("c1", "pic", 2426, 1433)
files = os.listdir("data")
folders = ("model_test", "x12", "x42", "y11", "y41")
for f in files:
    if f not in folders:
        start_file = os.path.join("data", f)
        break
file = ROOT.TFile(start_file)
file_up = uproot.open(start_file)
keys =  file_up.keys()
type_of_stand = "nirs"
for key in keys:
    print(key)
    h1 = file.Get(key)
    path_to_data = "data"
    for f in os.listdir(path_to_data):
        if os.path.join(path_to_data, f) != start_file and "output" in f and f not in folders:
            f = ROOT.TFile(os.path.join(path_to_data, f))
            h1.Add(f.Get(key))
            f.Close()
    if "PhotonsCount" in key:
        x_axis = "Photons count per event"
        y_axis = "Number of events"
        output = "PhotonsCount"
    # if "Electrons" in key:
    #     x_axis = "Electrons energy, keV"
    #     y_axis = "Number of events"
    #     output = "Electrons energy"
    elif "PhotonsEnergy" in key:
        x_axis = "Energy, eV"
        y_axis = "Number of events"
        output = "PhotonsEnergy"
    # elif "CherenkovCount" in key:
    #     x_axis = "Cherenkov photons count per event"
    #     y_axis = "Number of events"
    #     output = "CherenkovCount"
    # elif "CherenkovEnergy" in key:
    #     x_axis = "Cherenkov photons energy"
    #     y_axis = "Number of events"
    #     output = "CherenkovEnergy"
    else:
        continue
    
    h1.GetXaxis().SetTitle(x_axis)
    h1.GetYaxis().SetTitle(y_axis)
    h1.Draw()
    full_file = ROOT.TFile(os.path.join("combined", f"{output}{type_of_stand}.root"), "RECREATE")
    h1.Write()
    full_file.Write()
    c1.SaveAs(os.path.join("pics", f"{output}{type_of_stand}.png"))
    full_file.Close()
