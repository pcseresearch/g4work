#ifndef SBSMaterials_H
#define SBSMaterials_H 1
#include "globals.hh"

class G4Material;
class G4NistManager;
class SBSMaterials
{
public:
  
  SBSMaterials();
  ~SBSMaterials();

  G4Material*        defaultMaterial;
  G4Material*        Vacuum;
  G4Material*        SciMaterial;
  G4Material*        AbsorberMaterial; 
  /*
  G4Material*        MagnetMaterial;
  G4Material*        SpulenMaterial;
  G4Material*        HallenMaterial;
  G4Material*        TargetMaterial;
  G4Material*        StrahlrohrMaterial;
  G4Material*        DumpMaterial;
  G4Material*        WeltMaterial;
  G4Material*        Blei;
  G4Material*        ChipMaterial;
  G4Material*        MWPCFolienMaterial;
  G4Material*        MWPCGasMaterial;
  G4Material*        KollimatorMaterial;
  */

private:

  //Check with KAOS to see its use
  //  G4int              verbose;

};

#endif
