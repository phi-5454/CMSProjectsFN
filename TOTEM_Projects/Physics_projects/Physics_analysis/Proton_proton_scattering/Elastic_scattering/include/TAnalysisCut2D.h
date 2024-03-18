#ifndef TAnalysisCut2D_h
#define TAnalysisCut2D_h

#include "TAnalysisCut.h"

#include "TLine.h"
#include "TF1.h"
#include "TMath.h"
#include "TEllipse.h"

class TAnalysisCut2D : public TAnalysisCut
{

	TVector2D p1, p2, unit_direction_vector, unit_normal_vector ;
	TVector2D range1 ;
	TVector2D range2 ;
	double sigma_of_fit ;

	bool x_symmetric_cut ;
	
	double length ;
	
	public:
	
	TAnalysisCut2D(string , string, TProjectParameters *, string ) ;
	void Print() ;

	void CalculateLengthAndUnitAndNormalVector() ;
	bool IsThePointWithinRange(double x, double y) ;
	
	double DistanceFromCut(double, double) ;
	double PositionAlongCut(double x, double y) ;
	
	virtual bool IsTrue(double x, double y) ;
	void DrawCut() ;
	void DrawCutOnRotatedPlot(double , double ) ;

	virtual void Save() ;

	void SetP1X(double x) { p1.SetX(x) ; } ;
	void SetP1Y(double y) { p1.SetY(y) ; } ;

	void SetP2X(double x) { p2.SetX(x) ; } ;
	void SetP2Y(double y) { p2.SetY(y) ; } ;

	void SetP2ViaAngle(double angle) ;

	double GetP1X() { return p1.GetX() ; } ;
	double GetP1Y() { return p1.GetY() ; } ;

	double GetP2X() { return p2.GetX() ; } ;
	double GetP2Y() { return p2.GetY() ; } ;

	double GetCutPrincipalAngle() { return ((p2 - p1).GetPrincipalAngle()) ; } ;
	double GetSlope() { return ((p2 - p1).Slope()) ; } ;

	void Rotate(double, double) ;
	void Shift(double, double) ;
} ;

TAnalysisCut2D::TAnalysisCut2D(string name, string id, TProjectParameters *ProjectParameters, string cut_block_name) : TAnalysisCut(name, id, ProjectParameters, cut_block_name), x_symmetric_cut(false), sigma_of_fit(0)
{
	valid = false ;

	TProjectParameters *cut_block = ProjectParameters->GetBlock(cut_block_name) ;

	p1 = TVector2D(cut_block->GetParameterValue("x1"), cut_block->GetParameterValue("y1")) ;
	p2 = TVector2D(cut_block->GetParameterValue("x2"), cut_block->GetParameterValue("y2")) ;

	range1 = TVector2D(cut_block->GetParameterValue("range1_x"), cut_block->GetParameterValue("range1_y")) ;
	range2 = TVector2D(cut_block->GetParameterValue("range2_x"), cut_block->GetParameterValue("range2_y")) ;

	if(cut_block->IsParameterDefined("x_symmetric_cut"))
	{
		if(cut_block->GetParameterValue("x_symmetric_cut") == 1.0) x_symmetric_cut = true ;
	}
	

	CalculateLengthAndUnitAndNormalVector() ;

	valid = true ;
}

void TAnalysisCut2D::Print()
{
	cout << "<begin> " << name << "_block" << endl ;	 ;

	cout << "\t<parameter> 	x1\t\t" << p1.GetX() << endl ;
	cout << "\t<parameter> 	y1\t\t" << p1.GetY() << endl ;
	cout << "\t<parameter> 	x2\t\t" << p2.GetX() << endl ;
	cout << "\t<parameter> 	y2\t\t" << p2.GetY() << endl ;

	cout << "\t<parameter> range1_x\t\t" << range1.GetX() << endl ;
	cout << "\t<parameter> range1_y\t\t" << range1.GetY() << endl ;
	cout << "\t<parameter> range2_x\t\t" << range2.GetX() << endl ;
	cout << "\t<parameter> range2_y\t\t" << range2.GetY() << endl ;

	
	if(sigma_of_fit != 0) cout << "\t<parameter> sigma\t\t"	 << sigma_of_fit << endl ;
	else cout << "\t<parameter> sigma\t\t"	 << sigma << endl ;

	cout << "\t<parameter> number_of_sigmas\t\t" << number_of_sigmas << endl ;
	cout << "<end> " << name << "_block" << endl ;
	
	// Save it to Latex file

	string name_to_display = name ;

	for(int i = 0 ; i < name_to_display.length() ; ++i)
	{
		if(name_to_display[i] == '_') name_to_display[i] = ' ' ;
	}

	(*ProjectParameters->GetPLatexFile())	<< "Cut: " << name_to_display << " " << endl ;	 ;

	(*ProjectParameters->GetPLatexFile())	<< "parameter 	x1 " << p1.GetX() << endl ;
	(*ProjectParameters->GetPLatexFile())	<< "parameter 	y1 " << p1.GetY() << endl ;
	(*ProjectParameters->GetPLatexFile())	<< "parameter 	x2 " << p2.GetX() << endl ;
	(*ProjectParameters->GetPLatexFile())	<< "parameter 	y2 " << p2.GetY() << endl ;

	(*ProjectParameters->GetPLatexFile())	<< "parameter range1 x " << range1.GetX() << endl ;
	(*ProjectParameters->GetPLatexFile())	<< "parameter range1 y " << range1.GetY() << endl ;
	(*ProjectParameters->GetPLatexFile())	<< "parameter range2 x " << range2.GetX() << endl ;
	(*ProjectParameters->GetPLatexFile())	<< "parameter range2 y " << range2.GetY() << endl ;

	if(sigma_of_fit != 0) 	(*ProjectParameters->GetPLatexFile())	<< "parameter sigma "	 << sigma_of_fit << endl ;
	else 	(*ProjectParameters->GetPLatexFile())	<< "parameter sigma "	 << sigma << endl ;

	(*ProjectParameters->GetPLatexFile())	<< "parameter number of sigmas " << number_of_sigmas << endl ;
	(*ProjectParameters->GetPLatexFile())	<< "End cut " << name_to_display << "\\newline " << endl ;
}

void TAnalysisCut2D::CalculateLengthAndUnitAndNormalVector()
{
	length = (p2 - p1).abs() ;

	unit_direction_vector = (p2 - p1) ;
	double unit_direction_vector_length = unit_direction_vector.abs() ;

	unit_direction_vector *= (1 / unit_direction_vector_length) ;

	unit_normal_vector.SetX(-unit_direction_vector.GetY()) ;
	unit_normal_vector.SetY(unit_direction_vector.GetX()) ;

	if(((float)unit_direction_vector.abs()) != ((float)1.0))
	{
		cout << "Error: TAnalysisCut2D : public TAnalysisCut: unit_direction_vector length is not 1 ! The difference is: " << (unit_direction_vector.abs()-1)  << endl ;
		exit(1) ;
	}

	if(((float)unit_normal_vector.abs()) != ((float)1.0))
	{
		cout << "Error: TAnalysisCut2D : public TAnalysisCut: unit_normal_vector length is not 1 ! The difference is: " << (unit_normal_vector.abs()-1) << endl ;
		exit(1) ;
	}

	if(unit_normal_vector * unit_direction_vector != 0)
	{
		cout << "Error: TAnalysisCut2D : public TAnalysisCut: unit_normal_vector and unit_direction_vector are not orthogonal !" << endl ;
		exit(1) ;
	}
}

double TAnalysisCut2D::PositionAlongCut(double x, double y)
{
	TVector2D point(x,y) ;
	TVector2D difference_vector_wrt_p1 = (point - p1) ;

	return (difference_vector_wrt_p1 * unit_direction_vector) ;
}

void TAnalysisCut2D::SetP2ViaAngle(double angle)
{
	p2.SetX(p1.GetX() + (length * cos(angle))) ;
	p2.SetY(p1.GetY() + (length * sin(angle))) ;
	
	cout << "hello" << endl ;
}

bool TAnalysisCut2D::IsThePointWithinRange(double x, double y)
{
	TVector2D point(x,y) ;
	
	// cout << "Test this !!!" << endl ;

	double diff_vector_from_range1 = (range1 - point) * unit_direction_vector ;
	double diff_vector_from_range2 = (range2 - point) * unit_direction_vector ;
	
	return ((diff_vector_from_range1 * diff_vector_from_range2) < 0) ;
}

double TAnalysisCut2D::DistanceFromCut(double x, double y)
{
	TVector2D point(x,y) ;
	TVector2D difference_vector_wrt_p1 = (point - p1) ;

	return (difference_vector_wrt_p1 * unit_normal_vector) ;
}

bool TAnalysisCut2D::IsTrue(double x, double y)
{
	if(x_symmetric_cut)
	{
		return (((fabs(DistanceFromCut(x,y)) / sigma) < number_of_sigmas) || ((fabs(DistanceFromCut(-x,y)) / sigma) < number_of_sigmas))  ;
	}
	else return ((fabs(DistanceFromCut(x,y)) / sigma) < number_of_sigmas)  ;
}

void TAnalysisCut2D::DrawCut()
{
	TLine *cutline = new TLine(p1.GetX(), p1.GetY(), p2.GetX(), p2.GetY()) ;

	TVector2D normal_vector_one_sigma = unit_normal_vector * sigma ;

	TLine *plus_1sigma_line  = new TLine(p1.GetX() + normal_vector_one_sigma.GetX(), p1.GetY() + normal_vector_one_sigma.GetY(), p2.GetX() + normal_vector_one_sigma.GetX(), p2.GetY() + normal_vector_one_sigma.GetY()) ;
	TLine *minus_1sigma_line = new TLine(p1.GetX() - normal_vector_one_sigma.GetX(), p1.GetY() - normal_vector_one_sigma.GetY(), p2.GetX() - normal_vector_one_sigma.GetX(), p2.GetY() - normal_vector_one_sigma.GetY()) ;

	TVector2D normal_vector_cut = unit_normal_vector * (sigma * (double)number_of_sigmas) ;

	TLine *plus_cut_line = new TLine(p1.GetX() + normal_vector_cut.GetX(), p1.GetY() + normal_vector_cut.GetY(), p2.GetX() + normal_vector_cut.GetX(), p2.GetY() + normal_vector_cut.GetY()) ;
	TLine *minus_cut_line = new TLine(p1.GetX() - normal_vector_cut.GetX(), p1.GetY() - normal_vector_cut.GetY(), p2.GetX() - normal_vector_cut.GetX(), p2.GetY() - normal_vector_cut.GetY()) ;

	plus_1sigma_line->SetLineStyle(kDashed) ;
	minus_1sigma_line->SetLineStyle(kDashed) ;

	plus_cut_line->SetLineStyle(kDashed) ;
	minus_cut_line->SetLineStyle(kDashed) ;

	plus_cut_line->SetLineColor(kRed) ;
	minus_cut_line->SetLineColor(kRed) ;

	plus_1sigma_line->SetLineColor(TConstants::color_plus_1sigma_line) ;
	minus_1sigma_line->SetLineColor(TConstants::color_plus_1sigma_line) ;

	cutline->Draw() ;

	plus_1sigma_line->Draw() ;
	minus_1sigma_line->Draw() ;

	plus_cut_line->Draw() ;
	minus_cut_line->Draw() ;
	
	bool hollow_ellipse = true ;

	TEllipse *p1_circle = new TEllipse(p1.GetX(), p1.GetY(), sigma/5) ;
	if(hollow_ellipse) p1_circle->SetFillStyle(0) ;
	p1_circle->SetFillColor(kRed) ;
	p1_circle->SetLineColor(kRed) ;
	p1_circle->Draw() ;
	
	TLine *normal_vector_cut_line = new TLine(p1.GetX(), p1.GetY(), p1.GetX() + normal_vector_cut.GetX(), p1.GetY() + normal_vector_cut.GetY()) ;
	normal_vector_cut_line->SetLineColor(kRed) ;
	normal_vector_cut_line->Draw() ;
	
	// Range points and lines

	TEllipse *range1_circle = new TEllipse(range1.GetX(), range1.GetY(), sigma/5) ;
	if(hollow_ellipse) range1_circle->SetFillStyle(0) ;
	range1_circle->SetFillColor(kBlue) ;
	range1_circle->SetLineColor(kBlue) ;
	range1_circle->Draw() ;
	
	TLine *range1_line = new TLine(range1.GetX(), range1.GetY(), range1.GetX() + normal_vector_cut.GetX(), range1.GetY() + normal_vector_cut.GetY()) ;
	range1_line->SetLineColor(kBlue) ;
	range1_line->Draw() ;


	TEllipse *range2_circle = new TEllipse(range2.GetX(), range2.GetY(), sigma/5) ;
	if(hollow_ellipse) range2_circle->SetFillStyle(0) ;
	range2_circle->SetFillColor(kBlue) ;
	range2_circle->SetLineColor(kBlue) ;
	range2_circle->Draw() ;

	TLine *range2_line = new TLine(range2.GetX(), range2.GetY(), range2.GetX() + normal_vector_cut.GetX(), range2.GetY() + normal_vector_cut.GetY()) ;
	range2_line->SetLineColor(kBlue) ;
	range2_line->Draw() ;

}

void TAnalysisCut2D::DrawCutOnRotatedPlot(double x_lower_boundary, double x_upper_boundary)
{
	TLine *plus_1sigma_line  = new TLine(x_lower_boundary,   sigma, x_upper_boundary,  sigma) ;
	TLine *minus_1sigma_line  = new TLine(x_lower_boundary, -sigma, x_upper_boundary, -sigma) ;

	double cut_position = sigma * number_of_sigmas ;
	TLine *plus_cut_line  = new TLine(x_lower_boundary,   cut_position, x_upper_boundary,  cut_position) ;
	TLine *minus_cut_line  = new TLine(x_lower_boundary, -cut_position, x_upper_boundary, -cut_position) ;

	plus_1sigma_line->SetLineStyle(kDashed) ;
	minus_1sigma_line->SetLineStyle(kDashed) ;

	plus_cut_line->SetLineStyle(kDashed) ;
	minus_cut_line->SetLineStyle(kDashed) ;

	plus_cut_line->SetLineColor(kRed) ;
	minus_cut_line->SetLineColor(kRed) ;

	plus_1sigma_line->SetLineColor(TConstants::color_plus_1sigma_line) ;
	minus_1sigma_line->SetLineColor(TConstants::color_plus_1sigma_line) ;

	plus_1sigma_line->Draw("same") ;
	minus_1sigma_line->Draw("same") ;

	plus_cut_line->Draw("same") ;
	minus_cut_line->Draw("same") ;
}

void TAnalysisCut2D::Save()
{
}

void TAnalysisCut2D::Rotate(double angle, double a_sigma_of_fit)
{
	TVector2D dp = (p2 - p1) ;
	sigma_of_fit = a_sigma_of_fit ;

	dp.Rotate(angle) ;

	p2 = (p1 + dp) ;

	CalculateLengthAndUnitAndNormalVector() ;

	cout.precision(8) ;
	Print() ;
	cout.precision(6) ;
}

void TAnalysisCut2D::Shift(double mean, double a_sigma_of_fit)
{
	sigma_of_fit = a_sigma_of_fit ;
	TVector2D normal_vector_one_mean = unit_normal_vector * (mean) ;

	p1 = p1 + normal_vector_one_mean ;
	p2 = p2 + normal_vector_one_mean ;

	cout << endl << "Coordinates of shifted for cut: " << name << "    " << endl ;

	cout.precision(8) ;
	Print() ;
	cout.precision(6) ;

	cout << endl ;
}


#endif // #ifndef TAnalysisCut2D_h
