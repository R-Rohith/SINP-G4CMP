#pragma once
#include "G4UserRunAction.hh"
#include <fstream>
#include <mutex>

class RunAction : public G4UserRunAction {
public:
  RunAction();
  ~RunAction() override;

  void BeginOfRunAction(const G4Run* run) override;
  void EndOfRunAction(const G4Run* run) override;

  // Thread-safe logging API for any thread (primary generator calls this)
  static void LogRow(int event_id, double ER_eV, double theta_rad,
                     double v_kms, double x_cm, double y_cm, double z_cm);

private:
  // Shared across threads
  static std::ofstream s_out;
  static std::mutex    s_mtx;
};
