#!/bin/sh --login

#BSUB -q 1nh

WorkDir=/afs/cern.ch/user/c/chenyi/work/PhysicsWorkspace/JetEnergyCorrection/MCTruthStudies/17362_SanityClosure

export SCRAM_ARCH=slc6_amd64_gcc630
cd ~/work/CMSSW/CMSSW_9_4_3/src
eval `scramv1 runtime -sh`
cd - > /dev/null

cp $WorkDir/*txt .

echo Input files are: /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_1.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_10.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_100.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_101.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_102.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_103.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_104.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_105.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_106.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_107.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_108.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_109.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_11.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_110.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_111.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_112.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_113.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_114.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_115.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_116.root

hadd -k -f Input.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_1.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_10.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_100.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_101.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_102.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_103.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_104.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_105.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_106.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_107.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_108.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_109.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_11.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_110.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_111.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_112.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_113.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_114.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_115.root /eos/cms/store/group/phys_jetmet/magda/AK4chs_PU_GT/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/PU2017/180306_134748/0000/JRA_116.root

$WorkDir/Execute -input Input.root -output Output.root -tree ak4pfchs/t \
   -l1 Fall17_17Nov2017_V8_MC_L1FastJet_AK4PFchs.txt -l2l3 Fall17_17Nov2017_V8_MC_L2Relative_AK4PFchs.txt

cp Output.root /eos/cms/store/cmst3/user/chenyi//BatchOutput/JetEnergyCorrection/MCTruthStudies/17362_SanityClosure//Output_0.root
