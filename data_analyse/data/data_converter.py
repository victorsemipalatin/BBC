import os
import ROOT
import uproot


folders = ["x12", "x42", "y11", "y41"]
with open("res.txt", 'w') as result:
    for folder in folders:
        coordindates = os.listdir(folder)
        for coordinate in coordindates:
            curr_dir = os.path.join(folder, coordinate)
            files = os.listdir(curr_dir)
            startFile = os.path.join(curr_dir, files[0])
            tFile = ROOT.TFile(startFile)
            file_up = uproot.open(startFile)
            keys =  file_up.keys()
            for el in keys:
                if "PhotonsCount" in el:
                    key = el
            h1 = tFile.Get(key)
            for i in range(1, len(files)):
                tmpF = ROOT.TFile(os.path.join(curr_dir, files[i]))
                h1.Add(tmpF.Get(key))
                tmpF.Close()
            mean = h1.GetMean()
            if "x" in folder:
                if mean == 0:
                    result.write(f"-{folder[1:]} {coordinate} {1e-10}\n")
                else:
                    result.write(f"-{folder[1:]} {coordinate} {mean}\n")
            else:
                if mean == 0:
                    result.write(f"{-int(coordinate)} {folder[1:]} {1e-10}\n")
                else:
                    result.write(f"{-int(coordinate)} {folder[1:]} {mean}\n")
        
