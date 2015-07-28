TagAndProbeAnalysis
========================

Start from the CMSSW src directory: $CMSSW_BASE//src/

Run the computeEfficiency ROOT macro directly.
Make sure to include the rootlogon.C in the TagAndProbeAnalysis package because it
compiles some dependencies.

root -l -b -q TagAndProbeAnalysis/rootlogon.C TagAndProbeAnalysis/computeEfficiency.C+


=====================================
Description of Components
=====================================
The computeEfficiency macro is a control macro that you can use as a script that calls
other functions to perform specific efficiency calculations that you want to do.

The main efficiency calculations are done in the MakeEfficiencyPlots function defined here:
TagAndProbeAnalysis/Utils/EfficiencyTools.hh

The scale factors are calculated and collected by the makeEfficiencyScaleFactors function defined here:
TagAndProbeAnalysis/Utils/ScaleFactors.hh

=====================================
Procedure to Derive Scale-Factors
=====================================
To derive scale factors one needs to follow the following procedure:

(1) Compute the efficiency following the tag and probe procedure in the Monte Carlo Simulation
    using the MakeEfficiencyPlots function.

(2) Compute the efficiency following the tag and probe procedure in Data
    using the MakeEfficiencyPlots function.

(3) Compute the scale factors and make summary tables
    using the makeEfficiencyScaleFactors function.

=====================================
Output Results
=====================================
The output results will be summarized into an organized webpage located in the
output directory that is provided as a parameter. The output includes fit plots,
efficiency plots, and scale factor tables given in various formats (twiki, tex, txt).

