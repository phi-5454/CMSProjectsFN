#include <iostream>

#include "Projects/Glueball_analysis/interface/GlueballAnalysis.h"

#include "TFile.h"

using namespace std;


int main(int argc, char* argv[])
{
  //get input arguments
  TString in(""),out(""),method("");
  bool debug(false), skimtree(false);
  float xsec(1.0);
  for(int i=1;i<argc;i++){
    string arg(argv[i]);
    if(arg.find("--in")!=string::npos && i+1<argc)              { in=argv[i+1]; i++;}
    else if(arg.find("--out")!=string::npos && i+1<argc)        { out=argv[i+1]; i++;}
    else if(arg.find("--debug")!=string::npos)                  { debug=true; }
    else if(arg.find("--skimtree")!=string::npos)               { skimtree=true; }
    else if(arg.find("--method")!=string::npos && i+1<argc)     { method=argv[i+1]; i++;}
    else if(arg.find("--xsec")!=string::npos && i+1<argc)       { sscanf(argv[i+1],"%f",&xsec); i++;}
  }

  if(debug) cout << "INFO: Debug mode is active" << endl;

  //check input/output
  if(in=="" || out=="")
    {
      cout << "Check input/output=" << in << "/" << out << endl;
      return -1;
    }

  //check method to run
  if(method=="GlueballAnalysis") {
    RunGlueballAnalysis(in,out,skimtree,debug);
  }
  else {
    cout << "Check method =" << method <<endl;
    return -1;
  }
  
  //all done
  return 0;
}  





