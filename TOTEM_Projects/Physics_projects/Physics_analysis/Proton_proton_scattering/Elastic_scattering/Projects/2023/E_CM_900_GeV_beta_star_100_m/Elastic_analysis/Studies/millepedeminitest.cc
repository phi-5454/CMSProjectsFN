#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include "math.h"
#include "math.h"

using namespace std;

#include "stdlib.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TFile.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TGraphErrors.h"
#include "TLatex.h"
#include "TLine.h"
#include "TRandom3.h"
#include "TMinuit.h"
#include "TStyle.h"
#include "TFitResult.h"

class track
{
  public:

  double ax, bx ;
  double axe, bxe ;
  
  track() ;
  
  double Eval(double) ;
} ;

double track::Eval(double z) 
{
  double x = (ax * z) + bx ;
  
  return x ;
}

track::track(): ax(0.0), bx(0.0)
{
}

class myclass
{
  public:

  vector<double> vz ;
  vector<double> vx ;
  
  track mytrack ;

  myclass() ;
  void add(double, double) ;
  void print() ;
  double delta(int, int, int, double, double, double) ;

  void fit_track() ;
} ;

myclass::myclass()
{
} ;

void myclass::add(double z, double x) 
{
  vz.push_back(z) ;
  vx.push_back(x) ;
}

void myclass::print() 
{
  for(int i = 0 ; i < vz.size() ; ++i)
  {
    cout << "(" << vz[i] << ", " << vx[i] << "), " ;
  }
  cout << endl ;
}

const double res = 66e-6 ;

vector<myclass *> vector_of_myclass;

myclass *thisclass ;

double fit_m[3] ;

void fcn2(Int_t &npar, double *gin, double &f, double *par, int iflag)
{
  double chi2 = 0 ;

//  double ax = thisclass->mytrack.ax ;
//  double bx = thisclass->mytrack.bx ;

  double ax = par[0] ;
  double bx = par[1] ;

  for(int i = 0 ; i < thisclass->vz.size() ; ++i)
  {
    double x = (ax * thisclass->vz[i]) + bx ;
    
    double delta = (thisclass->vx[i] + fit_m[i] - x) / res ;
    
    chi2 += (delta * delta) ;
  }
  
  f = chi2 ;
}

void myclass::fit_track()
{
  thisclass = this ;
  
  TMinuit *gMinuit2 = new TMinuit(10);
  gMinuit2->SetFCN(fcn2);

  Double_t arglist[10];
  Int_t ierflg = 0 ;
  arglist[0] = 1 ;
  gMinuit2->mnexcm("SET ERR", arglist ,1,ierflg);

  gMinuit2->mnparm(0, "ax", 0, 0.1, -3, 3, ierflg);
  gMinuit2->mnparm(1, "bx", 0, 0.1, -3, 3, ierflg);

  arglist[0] = 0 ;
  arglist[1] = 3 ;
  arglist[2] = 1 ;

  gMinuit2->mnexcm("MIGRAD", arglist , 2, ierflg);
  
  gMinuit2->GetParameter(0, mytrack.ax, mytrack.axe) ;
  gMinuit2->GetParameter(0, mytrack.bx, mytrack.bxe) ;
  
}


double myclass::delta(int i, int j, int n, double m0, double m1, double m2)
{
    double m[3] ;

    m[0] = m0 ;
    m[1] = m1 ;
    m[2] = m2 ;

    double z0 = vz[i] ;
    double x0 = vx[i] + m[i];

    double z1 = vz[j] ;
    double x1 = vx[j] + m[j] ;
    
    double theta_x = (x1 - x0) / (z1 - z0) ;

    double z2 = vz[n] ;
    double x2 = vx[n] + m[n] ;

    double x2_propagated = x0 + ((z2 - z0) * theta_x) ;
    
    double delta = (x2_propagated - x2) / res ;
    
    return delta ;

}

class mytrack
{
  double ax, bx ;
} ;
 
 
void fit_tracks()
{
  for(int i = 0 ; i < vector_of_myclass.size() ; ++i)
  {
    vector_of_myclass[i]->fit_track() ;
  }
}

void fcn(Int_t &npar, double *gin, double &f, double *par, int iflag)
{

  double m[3] ;

  m[0] = par[0] ;
  m[1] = par[1] ;
  m[2] = par[2] ;

  fit_m[0] = par[0] ;
  fit_m[1] = par[1] ;
  fit_m[2] = par[2] ;

  double chi2 = 0 ;

  for(vector<myclass *>::iterator it = vector_of_myclass.begin() ; it != vector_of_myclass.end() ; ++it)
  {
    (*it)->fit_track() ;

    for(int j = 0 ; j < (*it)->vz.size() ; ++j) 
    {
      double z = (*it)->vz[j] ;
      double x = (*it)->mytrack.Eval(z) ;
      
      double deltax = ((*it)->vx[j] + m[j] - x) / res ; 
      
      chi2 += (deltax*deltax) ;
    }
  }

  cout << chi2 << endl ;

  f = chi2 ;
}

int minimize()
{
  TMinuit *gMinuit2 = new TMinuit(10);
  gMinuit2->SetFCN(fcn);

  Double_t arglist[10];
  Int_t ierflg = 0 ;
  arglist[0] = 1 ;
  gMinuit2->mnexcm("SET ERR", arglist ,1,ierflg);

  gMinuit2->mnparm(0, "m0", 0, 0.1, -3, 3, ierflg);
  gMinuit2->mnparm(1, "m1", 0, 0.1, -3, 3, ierflg);
  gMinuit2->mnparm(2, "m2", 0, 0.1, -3, 3, ierflg);

  arglist[0] = 0 ;
  arglist[1] = 3 ;
  arglist[2] = 1 ;

  gMinuit2->mnexcm("MIGRAD", arglist , 2, ierflg);
}

main()
{

  TRandom3 myrand ;

  int number_of_particles = 2 ;
  
  double Lx_m = 20 ;
  double dLxds_1 = -0.5 ;
  
  const double z0 = 220.0 ;
  const double z1 = 220.4 ;
  const double z2 = 227.0 ;

  for(int i = 0 ; i < number_of_particles ; ++i)
  {
    double theta_x_star = myrand.Gaus()*100e-6 ;
    
    double x0       = Lx_m * theta_x_star ;
    double theta_x0 = dLxds_1 * theta_x_star ;

    double x1 = x0 + ((z1 - z0) * theta_x0);
    double x2 = x0 + ((z2 - z0) * theta_x0);
    
    double m0 = 0.0 ;
    double m1 = -1.0 ;
    double m2 = 0.0 ;
    
    x0 += m0 ;
    x1 += m1 ;
    x2 += m2 ;
    
    double theta_rec_0 = (x1 - x0) / (z1 - z0) ;
    double theta_rec_1 = (x2 - x0) / (z2 - z0) ;
    double theta_rec_2 = (x2 - x1) / (z2 - z1) ;
    
    myclass *m = new myclass() ;
    m->add(z0, x0) ;
    m->add(z1, x1) ;
    m->add(z2, x2) ;
    
    vector_of_myclass.push_back(m) ;
    
    cout << endl ;
    cout << "partice " << i << endl ;
    cout << "theta " << theta_x0 << " " << theta_rec_0 << " " << theta_rec_1 << " " << theta_rec_2 << endl ;
    // cout << "theta " << theta_x0 - theta_x0 << " " << theta_rec_0 - theta_x0 << " " << theta_rec_1 - theta_x0 << " " << theta_rec_2 - theta_x0 << endl ;

    // cout << theta_x_star << " " << theta_x0 << " " << x0 << " " << x1 << " " << x2 << endl ;
    // cout << theta_x_star << " " << theta_x0 << " " << (x1 - x0) << " " << (x2 - x0) << endl ;
  }

  fit_m[0] = 0.0 ;
  fit_m[1] = 0.0 ;
  fit_m[2] = 0.0 ;

  fit_tracks() ;

  cout << endl ;
  cout << endl ;
  cout << endl ;
  cout << endl ;
  cout << "################## end of track fits" << endl ;
  cout << endl ;
  cout << endl ;
  cout << endl ;

  minimize() ;
}
