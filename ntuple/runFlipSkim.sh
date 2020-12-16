#!/bin/bash

#python flipskim.py -d nanov5_2016 --Samples "SingleElectron" --Samples "DoubleEG" --Samples "DYJetsToLL_M-50-LO_ext2" --Samples "DYJetsToLL_M-50" -b
python flipskim.py -d nanov5_2017 --Samples "SingleElectron" --Samples "DoubleEG" --Samples "DYJetsToLL_M-50-LO_ext1" --Samples "DYJetsToLL_M-50_ext1" -b
python flipskim.py -d nanov5_2018 --Samples "EGamma" --Samples "DYJetsToLL_M-50-LO" --Samples "DYJetsToLL_M-50_ext2" -b

#python flipskim.py -d nanov5_2016 --Samples "SingleMuon" --Samples "WZTo3LNu_mllmin01_ext1" --Samples "MuonEG" --Samples "DoubleMuon" --Samples "SingleElectron" --Samples "DoubleEG"
