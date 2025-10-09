/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

#include "PhononSensitivity.hh"
#include "G4CMPElectrodeHit.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4PhononLong.hh"
#include "G4PhononTransFast.hh"
#include "G4PhononTransSlow.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "PhononConfigManager.hh"
#include <fstream>

#include"G4CMPUtils.hh"

PhononSensitivity::PhononSensitivity(G4String name) :
  G4CMPElectrodeSensitivity(name), fileName("") {
 // SetOutputFile(PhononConfigManager::GetHitOutput());
 // SetOutputFile(G4CMP::DebuggingFileThread(PhononConfigManager::GetHitOutput()));

   PARQUET_ASSIGN_OR_THROW(outfile,arrow::io::FileOutputStream::Open(G4CMP::DebuggingFileThread("phonon_hits.parquet")));
   parquet::WriterProperties::Builder builder;
   builder.max_row_group_length(100);
   builder.created_by("My Application");
   builder.version(ParquetVersion::PARQUET_2_6);
   builder.data_page_version(ParquetDataPageVersion::V2);
   builder.compression(Compression::ZSTD);
// SNAPPY-->2x
// ZSTD-->3x
// BROTLI-->2.6x
// GZIP-->2.6x
// LZ4-->2x

   /// Define schema.
   parquet::schema::NodeVector fields;
   fields.push_back(parquet::schema::PrimitiveNode::Make("Run ID",parquet::Repetition::REQUIRED,parquet::Type::INT32, parquet::ConvertedType::INT_32));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Event ID",parquet::Repetition::REQUIRED,parquet::Type::INT32,parquet::ConvertedType::INT_32));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Track ID",parquet::Repetition::REQUIRED,parquet::Type::INT32,parquet::ConvertedType::INT_32));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Particle Name",parquet::Repetition::REQUIRED,parquet::Type::BYTE_ARRAY,parquet::ConvertedType::UTF8)); 
   fields.push_back(parquet::schema::PrimitiveNode::Make("Start Energy",parquet::Repetition::REQUIRED,parquet::Type::DOUBLE));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Starting Position x-axis",parquet::Repetition::REQUIRED,parquet::Type::DOUBLE));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Starting Position y-axis",parquet::Repetition::REQUIRED,parquet::Type::DOUBLE));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Starting Position z-axis",parquet::Repetition::REQUIRED,parquet::Type::DOUBLE));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Starting Time",parquet::Repetition::REQUIRED,parquet::Type::DOUBLE));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Deposited energy",parquet::Repetition::REQUIRED,parquet::Type::DOUBLE));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Track wieght",parquet::Repetition::REQUIRED,parquet::Type::DOUBLE));//INT32,parquet::ConvertedType::INT_32));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Final Position x-axis",parquet::Repetition::REQUIRED,parquet::Type::DOUBLE));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Final Position y-axis",parquet::Repetition::REQUIRED,parquet::Type::DOUBLE));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Final Position z-axis",parquet::Repetition::REQUIRED,parquet::Type::DOUBLE));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Final Time",parquet::Repetition::REQUIRED,parquet::Type::DOUBLE));


   parquet::schema::NodePtr schema = parquet::schema::GroupNode::Make(
    "schema", parquet::Repetition::REQUIRED, fields);
    auto group_schema = std::static_pointer_cast<parquet::schema::GroupNode>(schema);
   os = std::make_unique<parquet::StreamWriter>(parquet::ParquetFileWriter::Open(outfile, group_schema, builder.build()));



}

/* Move is disabled for now because old versions of GCC can't move ofstream
PhononSensitivity::PhononSensitivity(PhononSensitivity&& in) :
  G4CMPElectrodeSensitivity(std::move(in)),
  output(std::move(in.output)),
  fileName(std::move(in.fileName)) {
}

PhononSensitivity& PhononSensitivity::operator=(PhononSensitivity&& in) {
  // Move all base mebers
  G4CMPElectrodeSensitivity::operator=(std::move(in));

  // Our members
  output.close();
  output = std::move(in.output);
  fileName = in.fileName;

  return *this;
}
*/

PhononSensitivity::~PhononSensitivity() {

}

void PhononSensitivity::EndOfEvent(G4HCofThisEvent* HCE) {
  G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(hitsCollection);
  auto* hitCol = static_cast<G4CMPElectrodeHitsCollection*>(HCE->GetHC(HCID));
  std::vector<G4CMPElectrodeHit*>* hitVec = hitCol->GetVector();

  G4RunManager* runMan = G4RunManager::GetRunManager();

   /*
   PARQUET_ASSIGN_OR_THROW(outfile,arrow::io::FileOutputStream::Open(G4CMP::DebuggingFileThread("phonon_hits.parquet")));
   parquet::WriterProperties::Builder builder;
  //  std::shared_ptr<parquet::schema::GroupNode> schema;
   //schema=parquet::schema::GroupNode::Make("schema", parquet::Repetition::REQUIRED, {parquet::schema::PrimitiveNode::Make("Run ID",parquet::Repetition::REQUIRED,parquet::Type::INT32,parquet::LogicalType::None())});   // Set up builder with required compression type etc.
   builder.max_row_group_length(64 * 1024);
   builder.created_by("My Application");
   builder.version(ParquetVersion::PARQUET_2_6);
   builder.data_page_version(ParquetDataPageVersion::V1);
   builder.compression(Compression::SNAPPY);
   /// Define schema.
   parquet::schema::NodeVector fields;
   fields.push_back(parquet::schema::PrimitiveNode::Make("Run ID",parquet::Repetition::REQUIRED,parquet::Type::INT32, parquet::ConvertedType::INT_32));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Event ID",parquet::Repetition::REQUIRED,parquet::Type::INT32,parquet::ConvertedType::INT_32));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Track ID",parquet::Repetition::REQUIRED,parquet::Type::INT32,parquet::ConvertedType::INT_32));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Particle Name",parquet::Repetition::REQUIRED,parquet::Type::BYTE_ARRAY,parquet::ConvertedType::UTF8)); 
   fields.push_back(parquet::schema::PrimitiveNode::Make("Start Energy",parquet::Repetition::REQUIRED,parquet::Type::DOUBLE));/*
   fields.push_back(parquet::schema::PrimitiveNode::Make("Starting Position x-axis",parquet::Repetition::REQUIRED,parquet::Type::DOUBLE,parquet::LogicalType::));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Starting Position y-axis",parquet::Repetition::REQUIRED,parquet::Type::DOUBLE,parquet::LogicalType::));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Starting Position z-axis",parquet::Repetition::REQUIRED,parquet::Type::DOUBLE,parquet::LogicalType::));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Starting Time",parquet::Repetition::REQUIRED,parquet::Type::DOUBLE,parquet::LogicalType::));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Deposited energy",parquet::Repetition::REQUIRED,parquet::Type::DOUBLE,parquet::LogicalType::));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Track wieght",parquet::Repetition::REQUIRED,parquet::Type::INT32,parquet::LogicalType::INT_32));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Final Position x-axis",parquet::Repetition::REQUIRED,parquet::Type::DOUBLE,parquet::LogicalType::));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Final Position y-axis",parquet::Repetition::REQUIRED,parquet::Type::DOUBLE,parquet::LogicalType::));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Final Position z-axis",parquet::Repetition::REQUIRED,parquet::Type::DOUBLE,parquet::LogicalType::));
   fields.push_back(parquet::schema::PrimitiveNode::Make("Final Time",parquet::Repetition::REQUIRED,parquet::Type::DOUBLE,parquet::LogicalType::));



   parquet::schema::NodePtr schema = parquet::schema::GroupNode::Make(
    "schema", parquet::Repetition::REQUIRED, fields);
    auto group_schema = std::static_pointer_cast<parquet::schema::GroupNode>(schema);
   std::cout<<G4CMP::DebuggingFileThread("phonon_hits.parquet\n");
   std::cout.flush();
   parquet::StreamWriter os{parquet::ParquetFileWriter::Open(outfile, group_schema, builder.build())};
   //os = std::make_unique<parquet::StreamWriter>(parquet::ParquetFileWriter::Open(outfile, schema, builder.build()));
*/   

//  if (output.good()) {
	double energy=0;
    for (G4CMPElectrodeHit* hit : *hitVec) {
      *os << runMan->GetCurrentRun()->GetRunID()
             << runMan->GetCurrentEvent()->GetEventID() 
             << hit->GetTrackID() 
             << hit->GetParticleName()
             << hit->GetStartEnergy()/eV
             << hit->GetStartPosition().getX()/m 
             << hit->GetStartPosition().getY()/m 
             << hit->GetStartPosition().getZ()/m 
             << hit->GetStartTime()/ns 
             << hit->GetEnergyDeposit()/eV 
             << hit->GetWeight()
             << hit->GetFinalPosition().getX()/m 
             << hit->GetFinalPosition().getY()/m 
             << hit->GetFinalPosition().getZ()/m 
             << hit->GetFinalTime()/ns << parquet::EndRow; 
	  //  energy+=hit->GetEnergyDeposit()/eV;
    }
    //os.flush();
    // std::cout << typeid(runMan->GetCurrentRun()->GetRunID()) 
    //          << typeid(runMan->GetCurrentEvent()->GetEventID() )
    //          << typeid(hit->GetTrackID() ).name()
    //          << typeid(hit->GetParticleName() )
    //          << typeid(hit->GetStartEnergy()/eV )
    //          << typeid(hit->GetStartPosition().getX()/m )
    //          << typeid(hit->GetStartPosition().getY()/m )
    //          << typeid(hit->GetStartPosition().getZ()/m )
    //          << typeid(hit->GetStartTime()/ns )
    //          << typeid(hit->GetEnergyDeposit()/eV )
    //          << typeid(hit->GetWeight())
    //          << typeid(hit->GetFinalPosition().getX()/m )
    //          << typeid(hit->GetFinalPosition().getY()/m )
    //          << typeid(hit->GetFinalPosition().getZ()/m )
    //          << typeid(hit->GetFinalTime()/ns) << parquet::EndRow; 

    //output<<runMan->GetCurrentEvent()->GetEventID()<<'\t'<<energy<<'\n';
    //output.flush();
  //  hitsCollection->clear();
  //}
}

void PhononSensitivity::SetOutputFile(const G4String &fn) {
  if (fileName != fn) {
    if (output.is_open()) output.close();
    fileName = fn;
    output.open(fileName, std::ios_base::out);
    if (!output.good()) {
      G4ExceptionDescription msg;
      msg << "Error opening output file " << fileName;
      G4Exception("PhononSensitivity::SetOutputFile", "PhonSense003",
                  FatalException, msg);
      output.close();
    } else {/*
      output << "Run ID,Event ID,Track ID,Particle Name,Start Energy [eV],"
             << "Start X [m],Start Y [m],Start Z [m],Start Time [ns],"
             << "Energy Deposited [eV],Track Weight,End X [m],End Y [m],End Z [m],"
             << "Final Time [ns]\n";*/
    }    
  }
}

G4bool PhononSensitivity::IsHit(const G4Step* step,
                                const G4TouchableHistory*) const {
  /* Phonons tracks are sometimes killed at the boundary in order to spawn new
   * phonon tracks. These tracks that are killed deposit no energy and should
   * not be picked up as hits.
   */
  const G4Track* track = step->GetTrack();
  const G4StepPoint* postStepPoint = step->GetPostStepPoint();
  const G4ParticleDefinition* particle = track->GetDefinition();
  const G4TouchableHandle touch1 = postStepPoint->GetTouchableHandle();
  const G4VPhysicalVolume* volume = touch1->GetVolume();
  if(volume==nullptr)return false;

  G4bool correctParticle = particle == G4PhononLong::Definition() ||
                           particle == G4PhononTransFast::Definition() ||
                           particle == G4PhononTransSlow::Definition();

  G4bool correctStatus = step->GetTrack()->GetTrackStatus() == fStopAndKill &&
                         postStepPoint->GetStepStatus() == fGeomBoundary &&
                         step->GetNonIonizingEnergyDeposit() > 0.;

  return correctParticle && correctStatus;
}
