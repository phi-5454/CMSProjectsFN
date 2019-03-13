#include <set>

typedef set <TBranch *> *Pset_of_Pbranch ;
typedef set <TBranch *> set_of_Pbranch ;

typedef set <Bool_t *> *Pset_of_Pvalidities ;
typedef set <Bool_t *> set_of_Pvalidities ;

struct RP_struct_type
{
	Bool_t		*Pvalidity ;

	Double_t	*Px ;
	Double_t	*Py ;

	Double_t	*Pthx ;
	Double_t	*Pthy ;

	UInt_t		*PuPlanesOn ;
	UInt_t		*PvPlanesOn ;
} ;

class TBranchVectors
{
	public:

	TTree *tree ;

	Pset_of_Pbranch	     validity_branches_to_get ;
	Pset_of_Pvalidities  validity_branches ;
	Pset_of_Pbranch	     branches_to_get ;

	TBranchVectors() ;
	void Print() ;
	bool Test() ;
} ;

TBranchVectors::TBranchVectors()
{
	validity_branches_to_get = new set_of_Pbranch ;
	validity_branches = new set_of_Pvalidities ;
	branches_to_get = new set_of_Pbranch ;
} ;

void TBranchVectors::Print()
{
	for(set_of_Pbranch::iterator it = validity_branches_to_get->begin(); it != validity_branches_to_get->end(); ++it) cout << (*it) << ", " ;
	cout << endl ;

	for(set_of_Pvalidities::iterator it = validity_branches->begin(); it != validity_branches->end(); ++it) cout << (*it) << ", " ;
	cout << endl ;

	for(set_of_Pbranch::iterator it = branches_to_get->begin(); it != branches_to_get->end(); ++it) cout << (*it) << ", " ;
	cout << endl ;
} ;

bool TBranchVectors::Test()
{
	return ((validity_branches_to_get->size() ==  TConstants::total_number_of_validity_branches) && (validity_branches->size() ==  TConstants::total_number_of_validity_branches) && (branches_to_get->size() == (4 * TConstants::total_number_of_branches))) ;
}
