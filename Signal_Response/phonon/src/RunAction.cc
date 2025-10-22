#include "RunAction.hh"
#include "G4Run.hh"
#include "G4Threading.hh"   // G4Threading::IsMasterThread()

std::ofstream RunAction::s_out;
std::mutex    RunAction::s_mtx;

RunAction::RunAction() = default;
RunAction::~RunAction() = default;

void RunAction::BeginOfRunAction(const G4Run*) {
  // Only the master thread opens/closes the shared file
  if (G4Threading::IsMasterThread()) {
    if (!s_out.is_open()) {
      s_out.open("dm_root.txt");     // ASCII .txt with CSV content
      s_out << "event_id,ER_eV,theta_rad,v_kms,x_cm,y_cm,z_cm\n";
      s_out.flush();
    }
  }
}

void RunAction::EndOfRunAction(const G4Run*) {
  if (G4Threading::IsMasterThread()) {
    if (s_out.is_open()) {
      s_out.flush();
      s_out.close();
    }
  }
}

void RunAction::LogRow(int eid, double ER_eV, double theta,
                       double v_kms, double x_cm, double y_cm, double z_cm)
{
  // Workers (and master) can call this concurrently; we serialize writes
  if (!s_out.is_open()) return;   // file opens in BeginOfRunAction on master
  std::lock_guard<std::mutex> lock(s_mtx);
  s_out << eid << ',' << ER_eV << ',' << theta << ','
        << v_kms << ',' << x_cm << ',' << y_cm << ',' << z_cm << '\n';
}
