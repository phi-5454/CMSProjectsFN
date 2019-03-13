string working_directory ;
int MADX_status = 0 ;

double x[3] ;
double y[3] ;

double y_new[3] ;
double y_new_error[3] ;
double y_new_error_constant[3] ;

void fcn(Int_t &npar, double *gin, double &f, double *MinuitParameter, int iflag)
{
   
    double chi2 = 0 ;
    
    for(int i = 0 ; i < 3 ; ++i)
    {
	// cout << "x: " << x[i] << endl ;
	// cout << "y: " << y[i] << endl ;

    	double y_loc = MinuitParameter[0] * pow(MinuitParameter[1], x[i]) ;
	//cout << "y: " << y << endl ;

	//cout << "pow " << pow(((y[i] - y) / (y[i]  * 0.1)),2) << endl ;
	//cout << "pow1 " << y[i] << endl ;
	//cout << "pow2 " << y << endl ;
	//cout << "pow3 " << (y[i]  * 0.1) << endl ;

    	// chi2 += pow(((y_new[i] - y_loc) / (y_new_error[i])),2) ;	// Pearson
    	// chi2 += pow(((y_new[i] - y_loc) / (sqrt(y_new[i]))),2) ;  	// Poissionian
    	// chi2 += pow(((y_new[i] - y_loc) / (5.0)),2) ;		// Constant

    	chi2 += pow(((y_new[i] - y_loc) / ((y_new_error_constant[i]))),2) ;  	// Poissionian
    	// chi2 += pow(((y_new[i] - y_loc) / (y_new_error_constant[i])),2) ;	// ?
    }

	cout << "chi2: " << chi2 << endl ;

    f=chi2  ;
}


void match()
{
    x[0] = 1.0 ;
    x[1] = 2.0 ;
    x[2] = 3.0 ;

    y[0] = 0.03 ;
    y[1] = 0.1 ;
    y[2] = 0.33 ;

    y_new[0] = 12 ;
    y_new[1] = 33 ;
    y_new[2] = 150 ;

    y_new_error[0] = 6 ;
    y_new_error[1] = 10 ;
    y_new_error[2] = 25 ;

    y_new_error_constant[0] = sqrt(13.0) ;
    y_new_error_constant[1] = sqrt(42.0) ;
    y_new_error_constant[2] = sqrt(140.0) ;

    TMinuit *gMinuit = new TMinuit(30);
    gMinuit->SetFCN(fcn);

    Double_t arglist[20];
    Int_t ierflg = 0 ;
    arglist[0] = 1 ;
    gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);

    gMinuit->mnparm(0,"A", 1 , 0.001, -5, 15, ierflg);
    gMinuit->mnparm(1,"B", 2.5 , 0.001, -5, 15, ierflg);

    arglist[0] = 1e-08 ;
    gMinuit->mnexcm("SET EPS", arglist ,1,ierflg);

    arglist[0] = 50000  ;
    arglist[1] = 1 ;

    gMinuit->mnexcm("MIGRAD", arglist ,2,ierflg);

    exit(0) ;

}
