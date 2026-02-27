#include <cmath>
#include <random>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Target
{
    const char *name;
    int A;
    int Z;
    int N;
    double mass_GeV;
    double stoich; // CaWO4: Ca=1, W=1, O=4
};

static const std::vector<Target> targets = {
    {"O", 16, 8, 8, 14.899, 4.0},
    {"Ca", 40, 20, 20, 37.260, 1.0},
    {"W", 184, 74, 110, 171.400, 1.0}};

constexpr double GF = 1.1663787e-5; // GeV^-2
constexpr double pi = 3.141592653589793;

// spherical Bessel j1
inline double j1(double x)
{
    if (std::abs(x) < 1e-5)
        return x / 3.0;
    return (std::sin(x) / (x * x) - std::cos(x) / x);
}

// Helm form factor
double helmFF2(double q_GeV, int A)
{ 
    const double hbarc = 0.1973269804; // GeV*fm
    double q = q_GeV / hbarc;          // 1/fm

    double s = 0.9;                          // fm
    double R = 1.2 * std::pow(A, 1.0 / 3.0); // fm
    double R0 = std::sqrt(R * R - 5 * s * s);

    double x = q * R0;
    double F = ((3.0 * j1(x)) * (exp(-0.5 * q * q * s * s))) / x;

     return F * F;
     //return 1.0;
}

double dsig_dT(double Enu, const Target &t, double T)
{
    // all energies in GeV
    double M = t.mass_GeV;
    double Qw = t.N;

    double k = 1.0 - M * T / (2.0 * Enu * Enu);

    if (k <= 0.0)
        return 0.0;

    double q = std::sqrt(2.0 * M * T);
    double FormF = helmFF2(q, t.A);
   // std::cout << "The Form Factor is :" << FormF << std::endl;
    return (GF * GF / (4.0 * pi)) * Qw * Qw * M * k * helmFF2(q, t.A);
    // return (GF*GF/(4.0*pi)) * Qw*Qw * M * k;
}

struct Recoil
{
    const Target *target;
    double T; // recoil energy [GeV]
};

// main sampler
Recoil sampleCENNS(double Enu_GeV, std::mt19937 &rng)
{
    std::uniform_real_distribution<> U(0.0, 1.0);

    // --- first compute rate weights for each nucleus
    std::vector<double> weights;
    double sumw = 0.0;

    for (auto &t : targets)
    {

        double Tmax = 2.0 * Enu_GeV * Enu_GeV /
                      (t.mass_GeV + 2.0 * Enu_GeV);

        // cout << "Enu_GeV: " << Enu_GeV << " t.mass_GeV " << t.mass_GeV << endl;
        // cout << "Tmax" << Tmax << endl;

        // crude integral for relative rate
        int N = 1000;
        double w = 0.0;
        for (int i = 0; i < N; i++)
        {
            double T = Tmax * (i + 0.5) / N;
            //  cout << "i= " << i << " T: " << T << ": Tmax " << Tmax << " N: " << N << endl;
            //  cout << "initial w: " << w << " increment: " << dsig_dT(Enu_GeV, t, T) << endl;
            //  w += dsig_dT(Enu_GeV, t, T);
              w += dsig_dT(Enu_GeV, t, T)*(Tmax/N);
            //  cout << "final w: " << w << endl;
            // cout << "final w: " << w << endl;
        }
        //   cout << "After the loop w: " << w << endl;
        // cout << "t.stoich: " << t.stoich << endl;
        w *= t.stoich;
       // cout << "multiplied by t.stoich w: " << w << endl;
        weights.push_back(w);
        sumw += w;
        //  cout << "Sum of all nuclei " << sumw << endl;
    } 

    // --- choose target
    double r = U(rng) * sumw;
    const Target *tgt = nullptr;
    double acc = 0.0;
    for (size_t i = 0; i < targets.size(); i++)
    {
        acc += weights[i];
        if (r <= acc)
        {
            tgt = &targets[i];
            break;
        }
    }

    // --- sample recoil energy by rejection
    double Tmax = 2.0 * Enu_GeV * Enu_GeV /
                  (tgt->mass_GeV + 2.0 * Enu_GeV);

    
    //  std::ofstream dsigFile("dsig_vs_T.txt");

    //  int Nplot = 10000;
    //   double Enu = 25.0e-3;
    //    for(int i = 1; i <= Nplot; i++)
    //    {
    //        double T = Tmax * i / (Nplot);
    //        double val = dsig_dT(Enu, *tgt, T);
    //        dsigFile << T*1e6 << " " << val << "\n";  // T in keV
    //    }
   
    //    dsigFile.close();

      //double fmax = dsig_dT(Enu_GeV, *tgt, 0.0); // this is making FF=-NAN
      double fmax = dsig_dT(Enu_GeV, *tgt, 1.0e-40); // this is making FF=-NAN

    // double T,X;
    // vector<double> RecoilE_Random; 
    // vector<double> dsigdE_Random; 
    // do
    // {
    //     T = U(rng) * Tmax;
    //     RecoilE_Random.push_back(T);
    //     X =U(rng) * fmax;
    //     dsigdE_Random.push_back(X);
    // } while ( X> dsig_dT(Enu_GeV, *tgt, T));

   std::ofstream acceptFile("accepted_points.txt", std::ios::app);

    double T,y;
    while(true)
    {
        T = U(rng) * Tmax;
        y = U(rng) * fmax;
    
        if(y <= dsig_dT(Enu_GeV, *tgt, T))
        {
            acceptFile << T*1e6 << " " << y << "\n";  // T in keV
            break;
        }
    }
    
    acceptFile.close();
   
    return {tgt, T};
}


int main()
{
    // 1. Initialize ONCE (using a fixed number like 42 or a clock)
    std::mt19937 rng(82734);
    double Enu = 25.0e-3;
    std::ofstream outFile("recoil_data.txt");
    //  std::cout<<"The random number is "<<rng()<<std::endl;

    for (int i = 0; i < 100000; i++)
    {
        // The generator state evolves naturally with each call
        // std::cout<<"The random number"<<i<<" is "<<rng()<<std::endl;
        Recoil rec = sampleCENNS(Enu, rng);
        double T_keV = rec.T * 1e6;
        //   cout << "Target: " << rec.target->name << "\n";
        outFile << T_keV << "\n";
    }

    outFile.close();

    // Storing dsigma_dT to plot it analytically
    double M = targets[2].mass_GeV;  // example: W
    double Tmax = 2.0 * Enu * Enu / (M + 2.0 * Enu);

    std::ofstream dsigFile("dsig_vs_T.txt");

    int Nplot = 100000;

    for(int i = 1; i <= Nplot; i++)
    {
        double T = Tmax * i / (Nplot);
        double val = dsig_dT(Enu, targets[2], T);
        dsigFile << T*1e6 << " " << val << "\n";  // T in keV
    }

    dsigFile.close();

    return 0;
}
