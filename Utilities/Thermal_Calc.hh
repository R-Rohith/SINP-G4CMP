double Get_Tungsten_HC_SimpleT3(double curr_temp) // In Kelvin
{
  return 6.25e18*5.11e-5*curr_temp*curr_temp*curr_temp*1e-9/184; // In eV/(g.K)
}

double Get_Tungsten_HC_FromData(double curr_temp) // In Kelvin
{
  
}
double Get_Tungsten_Resistance_Simplified(double curr_temp) // In Kelvin
{
  const double Tc=41e-3, T_width=1e-3; // In Kelvin
  const double R_belowTc=8e-16, R_aboveTc=12e-15; // In Ohm
  if(curr_temp<Tc)
    return R_belowTc;
  else if(curr_temp>(Tc+T_width))
    return (20e-15-12e-15)/15e-3*(Tc+T_width-curr_temp);
  else
    return (R_aboveTc-R_belowTc)*(Tc-curr_temp)/T_width;
}
double Get_JouleHeating(double resistance/* In Ohm */, double delta_t/* In s */, double value/* In Volts or Amps */, bool voltage=true) // If supplying current pass false as the fourth arguement.
{
  if(voltage)
  {
    return value*value*delta_t/resistance;
  }
  else
  {
    return value*value*resistance*delta_t;
  }
}
