#define PI 3.1415926535897932384626433

double Get_Tungsten_HC_SimpleT3(double curr_temp) // In Kelvin
{
  return 6.25e18*5.11e-5*curr_temp*curr_temp*curr_temp*1e-9/184; // In eV/(g.K)
}

double Get_Tungsten_HC(double curr_temp) // In Kelvin
{
  return 6.25e18*(1.008*curr_temp+0.0346*curr_temp*curr_temp*curr_temp)*1e-3;
}

double Get_Tungsten_Resistance_Simplified(double curr_temp) // In Kelvin
{
  const double Tc=41e-3, T_width=1e-3; // In Kelvin
  const double R_belowTc=0.001, R_aboveTc=0.015; // In Ohm
  if(curr_temp<=Tc)
    return R_belowTc;
  else if(curr_temp>(Tc+T_width))
    return R_aboveTc+(0.02-R_aboveTc)*(curr_temp-Tc-T_width)/0.6e-3;
  else
    return R_belowTc+(R_aboveTc-R_belowTc)*(curr_temp-Tc)/T_width;
}
