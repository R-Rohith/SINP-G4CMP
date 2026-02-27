import ROOT
import math, os, sys

def readTextFile(filename):
    recoilEn = []
    with open(filename) as inOpen:
        for lines in inOpen.readlines():
            digits = lines.rstrip().split()[0]
            try:
                recoilEn.append(float(digits))
            except:
                print("is not a digit: ", digits)

    return recoilEn


def main():
    inputFileName = sys.argv[1]
    energyVal = readTextFile(inputFileName)


    fileOut = ROOT.TFile("RecoilHistogram.root", "RECREATE")
    fileOut.cd()
    h1 = ROOT.TH1D("h_En","h_En; Energy [keV]; Events/bin",100,0,10)
    for val in energyVal:
        h1.Fill(val)
    h1.Write()
    fileOut.Close()


if __name__=="__main__":
    main()

