export SCRAM_ARCH=slc6_amd64_gcc630
cd ~/work/CMSSW/CMSSW_10_2_6/src
eval `scramv1 runtime -sh`
cd - > /dev/null

EventCount=20
InputFile=file:pickevents.root
