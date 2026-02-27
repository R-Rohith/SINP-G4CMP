import os, sys, time
# import argparse
import ROOT
import math
import numpy as np
from ROOT import TFile, TH1D, TH2D


def main():

   ### output root file
   fileOut = ROOT.TFile("Analytical_dsig_dT.root", "RECREATE")
   fileOut.cd()
#    h2 = ROOT.TH2D("h_En","Analytical dsigma_dT; Energy [keV]; dsig_dT",100,0,10000)
   h1 = ROOT.TH1D("h1",
               "Analytical dsigma_dT;Energy [keV];d#sigma/dT",
               100, 0, 10)
   ### folder where the input files live
   inputfolder = "/home/rohit-kumar/softwares/SINP-G4CMP/CEvNS"
   ### get the text file
   filename = "dsig_vs_T.txt"

   fullFileName = inputfolder +"/"+filename 


   ### filling up the histograms
   counter  = 0
   with open(fullFileName) as inFile:
       for lines in inFile.readlines():
           counter += 1
           if counter % 100 == 0:
            print("processed:", counter)

           eachLine = lines.rstrip().split()
           Recoil_energy = float(eachLine[0])
           dsig_dT = float(eachLine[1])
    
           h1.Fill(Recoil_energy, dsig_dT)
        #    h2.Fill(dsig_dT)

   fileOut.Write()
   fileOut.Close()


if __name__=="__main__":
   main()