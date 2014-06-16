
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
//#include <omp.h>

int main(int argc, char* argv[])
{

    // FILES FOR RESULTS
    std::ofstream CV("CV_Output.txt");
    std::ofstream Profiles("CV_Profiles.txt");
    std::ofstream Grid("CV_checking.txt");

    int Guardar = 1000;//Npoints to save in file

    //Set system parameters
    double K0 = 1.e4;//dimensionless (= k0*re/DA)
    double alpha = 0.5;//transfer coefficient
    double dB = 1.;//dimensionles (=DB/DA)
    double CB = 0.;//dimensionless (=cB*/cA*)
    int NSpecies = 2;

    // Specify simulation parameters
    double theta_i = 6.;//dimensionaless initial potential (=F(Ei-E0)/RT)
    double theta_v = -6.;//dimensionless vertex potential (=F(Ev-E0)/RT)
    // double sigma = 0.1;//dimensionless scan rate (=v(F/RT)(re^2/D))
    double sigma = 0.1;//dimensionless scan rate (=v(F/RT)(re^2/D))


    double h0 = 1.e-4;//first dimensionless interval
    double omega = 1.08;//expanding factor
    // double deltaTheta = 0.005;//"time resolution"
    double deltaTheta = 0.005;//"time resolution"


    // Determine other parameters
    double deltaT = deltaTheta / sigma;
    double maxT = 2*fabs(theta_v - theta_i) / sigma;
    double maxR = 6 * sqrt(maxT) + 1;
    double maxZ = 6 * sqrt(maxT);
    int t = (int)( maxT / deltaT );  // number of timesteps
    int CadaCuantos = int(t/Guardar);
    //***********************************************************************


    //SPATIAL GRIDS
    // Make Z grid
    std::vector<double> Z;
    double h = h0;
    Z.push_back(0.0);
    while( Z.back() <= maxZ ) {
        Z.push_back( Z.back() + h );
        h *= omega;
    }
    int m = Z.size(); // number of spacesteps (Z)

    // Make R grid
    std::vector<double> R;
    h = h0;
    R.push_back(0);
    while( R.back() < 0.5 )
    {
        R.push_back( R.back() + h );
        h *= omega;
    }
    R.back() = 0.5;


    for(int i = R.size()-2; i>=0; i--) {
        R.push_back( 1 - R[i] );
    }
    int n_e = R.size(); // number spacesteps over electrode

    h = h0;
    while( R.back() <= maxR ) {
        R.push_back( R.back() + h );
        h *= omega;
    }
    int n = R.size(); //total number of spacesteps (R)


    //print out grid
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<m*NSpecies; j++)
        {
            Grid << R[i] << "\t" << Z[j] << "\n";
        }
    }


    // CONCENTRATION MATRICES (and initial conc. values)
    double** Ck = new double*[n];
    double** C_ = new double*[n];

    for(int i=0; i<n; ++i)
    {
        Ck[i] = new double[m*NSpecies];//new timestep
        C_[i] = new double[m*NSpecies];//previous timestep

        // set initial concentrations
        for(int j=0; j<(m*NSpecies); j++)
        {
            if(j<m)
            {
            Ck[i][j] = 1.0;
            C_[i][j] = 1.0;
            }
            else
            {
            Ck[i][j] = CB;//species B
            C_[i][j] = CB;
            }
        }
    }


    // THOMAS COEFFICIENTS
    std::vector<double> z_al(m*NSpecies,0.0), z_be(m*NSpecies,0.0), z_ga(m*NSpecies,0.0);
    std::vector<double> z2_al(m*NSpecies,0.0), z2_be(m*NSpecies,0.0), z2_ga(m*NSpecies,0.0);
    std::vector<double> ga_modZ1(m*NSpecies, 0.0), ga_modZ2(m*NSpecies, 0.0);

    std::vector<double> r_al(n,0.0), r_be(n,0.0), r_ga(n,0.0);
    std::vector<double> r_alB(n,0.0), r_beB(n,0.0), r_gaB(n,0.0);
    std::vector<double> ga_modR(n, 0.0);
    std::vector<double> ga_modRB(n, 0.0);


    //Z-direction (A and B coupled problems)
    //species A ("inverted" coefficients)
    z_be[0] = 1.;//corresponding to j=m-1 for A
    z_ga[0] = 0.;
    z2_be[0] = 1.;
    z2_ga[0] = 0.;
    for(int j=1; j<m-1; j++)
    {
        z_ga[m-j-1] = 2.0 / ( (Z[j+1]-Z[j-1]) * (Z[j] - Z[j-1]) );
        z_al[m-j-1] = 2.0 / ( (Z[j+1]-Z[j-1]) * (Z[j+1] - Z[j]) );
        z_be[m-j-1] = -z_al[m-j-1] - z_ga[m-j-1] - 2.0 / deltaT;

        z2_ga[m-j-1] = 2.0 / ( (Z[j+1]-Z[j-1]) * (Z[j] - Z[j-1]) );
        z2_al[m-j-1] = 2.0 / ( (Z[j+1]-Z[j-1]) * (Z[j+1] - Z[j]) );
        z2_be[m-j-1] = -z2_al[m-j-1] - z2_ga[m-j-1] - 2.0 / deltaT;
    }
    //species B
    for(int j=1; j<m-1; j++)
    {
        z_al[j+m] = dB*(2.0 / ( (Z[j+1]-Z[j-1]) * (Z[j] - Z[j-1]) ));
        z_ga[j+m] = dB*(2.0 / ( (Z[j+1]-Z[j-1]) * (Z[j+1] - Z[j]) ));
        z_be[j+m] = (-z_al[j+m] - z_ga[j+m]) - 2.0 / deltaT;

        z2_al[j+m] = dB*(2.0 / ( (Z[j+1]-Z[j-1]) * (Z[j] - Z[j-1]) ));
        z2_ga[j+m] = dB*(2.0 / ( (Z[j+1]-Z[j-1]) * (Z[j+1] - Z[j]) ));
        z2_be[j+m] = (-z2_al[j+m] - z2_ga[j+m]) - 2.0 / deltaT;
    }
    z2_al[m*NSpecies-1] = 0.;//corresponding to j=m-1 for B
    z2_be[m*NSpecies-1] = 1.;


    //R-direction (A and B independent problems)
    //species A
    r_be[0] = -1.;
    r_ga[0] = 1.;
    for(int i=1; i<n-1; i++)
    {
        r_al[i] = ( 1.0 / (R[i+1]-R[i-1]) )
            *  ( 2.0/(R[i] - R[i-1]) - 1.0/R[i] );
        r_ga[i] = ( 1.0 / (R[i+1]-R[i-1]) )
            *  ( 2.0/(R[i+1] - R[i]) + 1.0/R[i] );
        r_be[i] = -r_al[i] - r_ga[i] - 2.0 / deltaT;
    }
    r_al[n-1] = 0.;
    r_be[n-1] = 1.;
    //species B
    r_beB[0] = -1.;
    r_gaB[0] = 1.;
    for(int i=1; i<n-1; i++)
    {
        r_alB[i] = dB*( 1.0 / (R[i+1]-R[i-1]) )
            *  ( 2.0/(R[i] - R[i-1]) - 1.0/R[i] );
        r_gaB[i] = dB*( 1.0 / (R[i+1]-R[i-1]) )
            *  ( 2.0/(R[i+1] - R[i]) + 1.0/R[i] );
        r_beB[i] = (-r_alB[i] - r_gaB[i]) - 2.0 / deltaT;
    }
    r_alB[n-1] = 0.;
    r_beB[n-1] = 1.;

    // THOMAS: gamma' coefficients - Z sweep (potential independent, independent problems)
    //electroinactive zone
    //species A
    z2_al[m-1] = 1.;
    z2_be[m-1] = -1.;
    z2_ga[m-1] = 0.;
    ga_modZ2[0] =  z2_ga[0]/z2_be[0]; // bulk condition
    for(int j=1; j<m; j++)
    {
        ga_modZ2[j] = z2_ga[j]
            / (z2_be[j] - ga_modZ2[j-1] * z2_al[j]);
    }
    //species B
    z2_al[m] = 0; // no flux boundary condition
    z2_be[m] = -1.;
    z2_ga[m] = 1.;
    for(int j=0; j<m-1; j++)
    {
        ga_modZ2[j+m] = z2_ga[j+m]
            / (z2_be[j+m] - ga_modZ2[j+m-1] * z2_al[j+m]);
    }

    //species A - R sweep
    ga_modR[0] = -1; // boundary condition (zero flux)
    for(int i=1; i<n-1; i++)
    {
        ga_modR[i] = r_ga[i]
            / (r_be[i] - ga_modR[i-1] * r_al[i]);
    }
    //species B - R sweep
    ga_modRB[0] = -1; // boundary condition (zero flux)
    for(int i=1; i<n-1; i++)
    {
        ga_modRB[i] = r_gaB[i]
            / (r_beB[i] - ga_modRB[i-1] * r_alB[i]);
    }

/************************************************************************/
/************************************************************************/
    // BEGIN SIMULATION
    int stop = t;
    double Theta = theta_i;

    #pragma acc data 
    {
    for(int k=0; k<stop; k++)
    {
        if( k < t/2 ) { Theta -= deltaTheta; }
        else          { Theta += deltaTheta; }


        // THOMAS: gamma' coefficients for Z sweep
        //electroactive zone
        //species A ("inverted")
        z_al[m-1] = -1.;
        z_be[m-1] = 1.+h0*expl(-alpha*Theta)*K0;
        z_ga[m-1] = -h0*expl((1.-alpha)*Theta)*K0;
        ga_modZ1[0] =  z_ga[0]/z_be[0]; // bulk condition
        for(int j=1; j<m; j++)
        {
            ga_modZ1[j] = z_ga[j]
                / (z_be[j] - ga_modZ1[j-1] * z_al[j]);
        }
        //species B
        z_al[m] = -h0*expl(-alpha*Theta)*K0/dB;
        z_be[m] = 1. + h0*expl((1.-alpha)*Theta)*K0/dB;
        z_ga[m] = -1.;
        for(int j=0; j<m-1; j++)
        {
            ga_modZ1[j+m] = z_ga[j+m]
                / (z_be[j+m] - ga_modZ1[j+m-1] * z_al[j+m]);
        }


        //copy concentration grid
        for(int i=0; i<n; i++)
        {
            for(int j=0; j<(m*NSpecies); j++)
            {
                C_[i][j] = Ck[i][j];
            }
        }

        //--- Z SWEEP---
        //#pragma omp parallel for
        #pragma acc kernels loop independent
        for(int i=1; i<n-1; i++)
        {
            //Delta species A
            Ck[i][0] = 1.0;//bulk concentration
            for(int j=1; j<m-1; j++)
            {
                Ck[i][j] = - C_[i-1][j] * r_al[i]
                    - C_[i][j] * (-r_al[i] - r_ga[i])
                    - C_[i][j] * 2.0/deltaT
                    - C_[i+1][j] * r_ga[i];
            }
             Ck[i][m-1] = 0.;//surface conditions species A

            //Delta species B
            Ck[i][m] = 0.;//surface conditions species B
            for(int j=1; j<m-1; j++)
            {
                Ck[i][j+m] = - C_[i-1][j+m] * r_alB[i]
                    - C_[i][j+m] * (-r_alB[i] - r_gaB[i])
                    - C_[i][j+m] * 2.0/deltaT
                    - C_[i+1][j+m] * r_gaB[i];
            }
            Ck[i][m*NSpecies-1] = CB;

            // Set Delta'[0] and pointer to gamma_mod
            //std::vector<double>* ga_modZ;
            if(i < n_e)
            {
                //Ck[i][0] = 1.0 / (1.0 + exp(-Theta));//Nernst
                Ck[i][0] = 1.;//bulk condition (inverted coefficients)
                //ga_modZ = &ga_modZ1;
            }
            else
            {
                Ck[i][0] = 1.;//bulk condition (inverted coefficients)
                //ga_modZ = &ga_modZ2;
            }

            // Delta'[m>0] (we use the same vector as for concentrations...)
            //species A
            for(int j=1; j<m; j++)
            {
                if(i < n_e)
                {
                /* Ck[i][j] = ( Ck[i][j] - Ck[i][j-1] * z_al[j] )
                    / ( z_be[j] - (*ga_modZ)[j-1] * z_al[j] ); */
                Ck[i][j] = ( Ck[i][j] - Ck[i][j-1] * z_al[j] )
                    / ( z_be[j] - ga_modZ1[j-1] * z_al[j] );
                }
                else
                {
                /* Ck[i][j] = ( Ck[i][j] - Ck[i][j-1] * z2_al[j] )
                    / ( z2_be[j] - (*ga_modZ)[j-1] * z2_al[j] ); */
                 Ck[i][j] = ( Ck[i][j] - Ck[i][j-1] * z2_al[j] )
                    / ( z2_be[j] - ga_modZ2[j-1] * z2_al[j] ); 
                }
            }
            //species B
            for(int j=0; j<m; j++)
            {
                if(i < n_e)
                {
                /*Ck[i][j+m] = ( Ck[i][j+m] - Ck[i][j+m-1] * z_al[j+m] )
                    / ( z_be[j+m] - (*ga_modZ)[j+m-1] * z_al[j+m] );*/
                Ck[i][j+m] = ( Ck[i][j+m] - Ck[i][j+m-1] * z_al[j+m] )
                    / ( z_be[j+m] - ga_modZ1[j+m-1] * z_al[j+m] );
                }
                else
                {
                /* Ck[i][j+m] = ( Ck[i][j+m] - Ck[i][j+m-1] * z2_al[j+m] )
                    / ( z2_be[j+m] - (*ga_modZ)[j+m-1] * z2_al[j+m] ); */
                 Ck[i][j+m] = ( Ck[i][j+m] - Ck[i][j+m-1] * z2_al[j+m] )
                    / ( z2_be[j+m] - ga_modZ2[j+m-1] * z2_al[j+m] ); 
                }
            }


            //solve by back substitution
            Ck[i][m*NSpecies-1] = CB;
            for(int j=m*NSpecies-2; j>=0; j--)
            {
                if (i < n_e)
                  Ck[i][j] = Ck[i][j]-ga_modZ1[j]*Ck[i][j+1];
                else
                  Ck[i][j] = Ck[i][j]-ga_modZ2[j]*Ck[i][j+1];
            }

        }

        //copy concentration grid
        for(int i=0; i<n; i++)
        {
            for(int j=0; j<m*NSpecies; j++)
            {
                C_[i][j] = Ck[i][j];
            }
        }

        if (k % 100 == 0) {
        for(int i=0; i<n; i++)
            for(int j=0; j<m*NSpecies; j++)
                  Profiles << R[i] << "\t" << Z[j] << "\t" << Ck[i][j] << "\t" << Ck[i][j*NSpecies-1] << "\n";
        } 

        //Output current
        if (k % CadaCuantos == 0)
           {
            double flux = 0.0;
            for(int i=1; i<n_e; i++)
            {
                 double J2 = (Ck[i][m-2] - Ck[i][m-1]) * R[i];
                 double J1 = (Ck[i-1][m-2] - Ck[i-1][m-1])*R[i-1];
                flux -= (0.5/h0)*(J2+J1)*(R[i] - R[i-1]);
            }
            CV << Theta << "\t" << flux << "\n";
            }


        //Output concentration profiles (only at the end)
        if(k == stop - 1)
        {
            for(int j=0; j<m; j++)
            {
                for(int i=0; i<n; i++)
                 {
                     Profiles << R[i] << "\t" << Z[j] << "\t" << Ck[i][m-j-1] << "\t" << Ck[i][j+m] << "\n";
                 }
            }
        }


        //--- R SWEEP---
        //#pragma omp parallel for
        //species A
        for(int j=1; j<m-1; j++)
        {
            // set Deltas
            Ck[0][j] = 0;
            Ck[n-1][j] = 1.0;
            for(int i=1; i<n-1; i++)
            {
                if(i < n_e)
                {
                Ck[i][j] = - C_[i][j-1] * z_al[j]
                    - C_[i][j] * (-z_al[j] - z_ga[j])
                    - C_[i][j] * 2.0/deltaT
                    - C_[i][j+1] * z_ga[j];
                }
                else
                {
                Ck[i][j] = - C_[i][j-1] * z2_al[j]
                    - C_[i][j] * (-z2_al[j] - z2_ga[j])
                    - C_[i][j] * 2.0/deltaT
                    - C_[i][j+1] * z2_ga[j];
                }
            }

            // modify deltas
            for(int i=1; i<n-1; i++)
            {
                Ck[i][j] = ( Ck[i][j] - Ck[i-1][j] * r_al[i] )
                     / ( r_be[i] - ga_modR[i-1] * r_al[i] );
            }

            //solve by back substitution
            for(int i=n-2; i>=0; i--)
               {
                Ck[i][j] = Ck[i][j] - ga_modR[i]*Ck[i+1][j];
               }
        }

        //species B
        for(int j=1; j<m-1; j++)
        {
            // set Deltas
            Ck[0][j+m] = 0;
            Ck[n-1][j+m] = CB;
            for(int i=1; i<n-1; i++)
            {
                if(i < n_e)
                {
                Ck[i][j+m] = - C_[i][j+m-1] * z_al[j+m]
                    - C_[i][j+m] * (-z_al[j+m] - z_ga[j+m])
                    - C_[i][j+m] * 2.0/deltaT
                    - C_[i][j+m+1] * z_ga[j+m];
                }
                else
                {
                Ck[i][j+m] = - C_[i][j+m-1] * z2_al[j+m]
                    - C_[i][j+m] * (-z2_al[j+m] - z2_ga[j+m])
                    - C_[i][j+m] * 2.0/deltaT
                    - C_[i][j+m+1] * z2_ga[j+m];
                }
             }

            // modify deltas
            for(int i=1; i<n-1; i++)
            {
                Ck[i][j+m] = ( Ck[i][j+m] - Ck[i-1][j+m] * r_alB[i] )
                     / ( r_beB[i] - ga_modRB[i-1] * r_alB[i] );
            }

            //solve by back substitution
            for(int i=n-2; i>=0; i--)
            {
                Ck[i][j+m] = Ck[i][j+m] - ga_modRB[i]*Ck[i+1][j+m];
            }
        }


    }

    } // end of pragma acc data 
/****************** end simulation *************************************/
}
