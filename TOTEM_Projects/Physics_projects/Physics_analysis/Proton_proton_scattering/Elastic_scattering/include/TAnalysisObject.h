#ifndef TAnalysisObject_h
#define TAnalysisObject_h

#include <vector>
#include "math.h"

class TVector2D
{
	private:

	double x, y ;	// Coordinates
	
	public:
	
	TVector2D(double x = 0, double y = 0) : x(x), y(y) { } ;
	
	void SetX(double ax) { x = ax ; } ;
	void SetY(double ay) { y = ay ; } ;	

	double GetX() const { return x ; } ;
	double GetY() const { return y ; } ;
	
	double abs() const { return (sqrt((x*x) + (y*y))) ; } ;
	double GetPrincipalAngle() const { return (atan (y/x)) ; } ;
	double Slope() const { return (y/x) ; } ;
	
	TVector2D &operator+=(const TVector2D &rhs)
	{
		x += rhs.x ;
		y += rhs.y ;
		
		return *this ;
	}	

	TVector2D &operator-=(const TVector2D &rhs)
	{
		x -= rhs.x ;
		y -= rhs.y ;
		
		return *this ;
	}

	TVector2D &operator*=(const double &rhs)
	{
		x *= rhs ;
		y *= rhs ;
		
		return *this ;
	}

	const TVector2D operator+(const TVector2D &other) const
	{
		TVector2D result = *this ;
		result += other ;
		return result ;
	}

	const TVector2D operator-(const TVector2D &other) const
	{
		TVector2D result = *this ;
		result -= other ;
		return result ;
	}

	const TVector2D operator*(const double &other) const
	{
		TVector2D result = *this ;
		result *= other ;
		return result ;
	}

	const double operator*(const TVector2D &other) const
	{
		return ((this->GetX() * other.GetX()) + (this->GetY() * other.GetY())) ;
	}

	void Print()
	{
		cout << " TVector2D x: " << GetX() << " y: " << GetY() ;
	}
	
	void Rotate(double) ;
} ;

void TVector2D::Rotate(double angle)
{
	double x_rot = (cos(angle) * x) - (sin(angle) * y) ;
	double y_rot = (sin(angle) * x) + (cos(angle) * y) ;

	x = x_rot ;
	y = y_rot ;
}

typedef map<string, double> map_from_string_to_double_type ;
typedef map<string, string> map_from_string_to_string_type ;

typedef map<string, vector<double> *> map_from_string_to_doubles_type ;
typedef map<string, vector<string> *> map_from_string_to_strings_type ;

class TAnalysisObject ; 

typedef vector<TAnalysisObject *> vector_of_TAnalysisObject_pointers_type ;

#include "TError.h"

class TAnalysisObject
{
	protected:

	string name, id ;
	
	private:

	static TError *Errors ;
	static vector_of_TAnalysisObject_pointers_type *AnalysisObjects ;	// Vector of every object inherited from TAnalysisObject (for debugging)
	
	public:
	
	TAnalysisObject(string, string) ;
	virtual ~TAnalysisObject() ;
	int GetErrorCode(string error_string) { return TAnalysisObject::Errors->GetErrorCode(error_string) ; } ;

	string GetName() { return name ; } ;
	string GetId() 	{ return id ; } ;

	virtual void Print() { } ;
	static int PrintAll() ;
} ;


TError *TAnalysisObject::Errors = NULL ;
vector_of_TAnalysisObject_pointers_type *TAnalysisObject::AnalysisObjects = NULL ;

TAnalysisObject::TAnalysisObject(string name, string id) : name(name), id(id)
{
	if(TAnalysisObject::Errors == NULL) TAnalysisObject::Errors = new TError() ;
	if(TAnalysisObject::AnalysisObjects == NULL) TAnalysisObject::AnalysisObjects = new vector_of_TAnalysisObject_pointers_type ;

	// Check whether the name is unique

	for (vector_of_TAnalysisObject_pointers_type::iterator it = AnalysisObjects->begin() ; it != AnalysisObjects->end(); ++it) 
	{
		if((*it)->id.compare(id) == 0)
		{
			cout << "TAnalysisObject::TAnalysisObject(string name): the TAnalysisObject id is not unique : " << id << "!" << endl ;
			exit(1) ;
		}
	}

	TAnalysisObject::AnalysisObjects->push_back(this) ;
}

TAnalysisObject::~TAnalysisObject()
{
	
}

int TAnalysisObject::PrintAll()
{
	for (vector_of_TAnalysisObject_pointers_type::iterator it = AnalysisObjects->begin() ; it != AnalysisObjects->end(); ++it) (*it)->Print() ;

	return 0 ;
}

#endif // #ifndef TAnalysisObject_h
