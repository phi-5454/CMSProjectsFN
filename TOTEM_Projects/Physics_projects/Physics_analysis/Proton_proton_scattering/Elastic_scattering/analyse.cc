#include <iostream>
#include <vector>

using namespace std;

#include "stdlib.h"

#include "TNtuple.h"

#include "include/TProject.h"

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		cout << "Usage: analyse <configuration_file | project file>" << endl ; 
		exit(1) ;
	}	

	string config_or_project_filename(argv[1]) ;

	TProject Project("Project", "Project", config_or_project_filename) ;

	TAnalysisObject::PrintAll() ;

	Project.Execute() ;

	// TAnalysisObject::PrintAll() ;

	exit(0) ;
}
