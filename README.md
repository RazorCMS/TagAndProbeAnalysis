TagAndProbeAnalysis
========================

Start from the CMSSW src directory: $CMSSW_BASE//src/

Run the computeEfficiency ROOT macro directly.
Make sure to include the rootlogon.C in the TagAndProbeAnalysis package because it
compiles some dependencies.

root -l -b -q TagAndProbeAnalysis/rootlogon.C TagAndProbeAnalysis/computeEfficiency.C+

