default: TestRun

TestRun: Execute
	time ./Execute --input HEMSamples/AllResult_PF.root --output PF.pdf \
		--goodinput HealthySamples/AllResult_PF.root --etamin -2.5 --etamax -2.0
	time ./Execute --input HEMSamples/AllResult_PFCHS.root --output PFCHS.pdf \
		--goodinput HealthySamples/AllResult_PFCHS.root --etamin -2.5 --etamax -2.0
	cp *.pdf ~/public/html

Execute: HEML1.cpp
	CompileRootMacro HEML1.cpp Execute
