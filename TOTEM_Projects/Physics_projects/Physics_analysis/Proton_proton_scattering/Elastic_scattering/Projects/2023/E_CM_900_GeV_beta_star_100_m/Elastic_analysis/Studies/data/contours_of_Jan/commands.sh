myoutput="/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/Elastic_analysis/Studies/data/contours_of_Jan/contours.txt"
grep "contour_.*L  *=" /afs/cern.ch/work/f/fnemes/main_workspace/2023/Elastic_analysis/E_CM_900_GeV_beta_star_100_m/CMSSW_10_6_17/src/ElasticAnalysis_Jan/analysis_elastic.450GeV.beta11/config_base.py | sed 's/\[//g' | sed 's/\]//g' | sed 's/,//g' > $myoutput

