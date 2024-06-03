#!/bin/bash

cd YYYY
../../../../../../../../../analyse cfg.prj
mv *.root XXXX
mv out XXXX
mv err XXXX
mv cfg_analysis.log XXXX
rm cfg.tex
