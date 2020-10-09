#include <cstdint>

/**
 *  Contents of CPUID(1) eax register contents describing model version
*/
struct CpuVersion {
  uint32_t stepping       : 4;
  uint32_t model          : 4;
  uint32_t family         : 4;
  uint32_t type           : 2;
  uint32_t reserved1      : 2;
  uint32_t extendedModel  : 4;
  uint32_t extendedFamily : 8;
  uint32_t reserved2      : 4;
};

static_assert(sizeof(CpuVersion) == sizeof(uint32_t), "CpuVersion size mismatch!");

/**
 *  Intel CPU models as returned by CPUID
 *  The list is synchronised and updated with XNU source code (osfmk/i386/cpuid.h).
 *  Names are altered to avoid conflicts just in case.
 *  Last update: xnu-4903.221.2
 *  Some details could be found on http://instlatx64.atw.hu and https://en.wikichip.org/wiki/64-bit_architecture#x86
 *  Also: https://www.intel.com/content/dam/www/public/us/en/documents/sa00115-microcode-update-guidance.pdf
 */
enum CpuModel {
  CPU_MODEL_UNKNOWN        =  0x00,
  CPU_MODEL_PENRYN         =  0x17,
  CPU_MODEL_NEHALEM        =  0x1A,
  CPU_MODEL_FIELDS         =  0x1E, /* Lynnfield, Clarksfield */
  CPU_MODEL_DALES          =  0x1F, /* Havendale, Auburndale */
  CPU_MODEL_NEHALEM_EX     =  0x2E,
  CPU_MODEL_DALES_32NM     =  0x25, /* Clarkdale, Arrandale */
  CPU_MODEL_WESTMERE       =  0x2C, /* Gulftown, Westmere-EP/-WS */
  CPU_MODEL_WESTMERE_EX    =  0x2F,
  CPU_MODEL_SANDYBRIDGE    =  0x2A,
  CPU_MODEL_JAKETOWN       =  0x2D,
  CPU_MODEL_IVYBRIDGE      =  0x3A,
  CPU_MODEL_IVYBRIDGE_EP   =  0x3E,
  CPU_MODEL_CRYSTALWELL    =  0x46,
  CPU_MODEL_HASWELL        =  0x3C,
  CPU_MODEL_HASWELL_EP     =  0x3F,
  CPU_MODEL_HASWELL_ULT    =  0x45,
  CPU_MODEL_BROADWELL      =  0x3D,
  CPU_MODEL_BROADWELL_ULX  =  0x3D,
  CPU_MODEL_BROADWELL_ULT  =  0x3D,
  CPU_MODEL_BRYSTALWELL    =  0x47,
  CPU_MODEL_SKYLAKE        =  0x4E,
  CPU_MODEL_SKYLAKE_ULT    =  0x4E,
  CPU_MODEL_SKYLAKE_ULX    =  0x4E,
  CPU_MODEL_SKYLAKE_DT     =  0x5E,
  CPU_MODEL_SKYLAKE_W      =  0x55,
  CPU_MODEL_KABYLAKE       =  0x8E,
  CPU_MODEL_KABYLAKE_ULT   =  0x8E,
  CPU_MODEL_KABYLAKE_ULX   =  0x8E,
  CPU_MODEL_KABYLAKE_DT    =  0x9E,
  CPU_MODEL_CANNONLAKE     =  0x66,
  CPU_MODEL_ICELAKE_Y      =  0x7D,
  CPU_MODEL_ICELAKE_U      =  0x7E,
  CPU_MODEL_ICELAKE_SP     =  0x9F, /* Some variation of Ice Lake */
  CPU_MODEL_COMETLAKE_S    =  0xA5, /* desktop CometLake */
  CPU_MODEL_COMETLAKE_Y    =  0xA5, /* aka 10th generation Amber Lake Y */
  CPU_MODEL_COMETLAKE_U    =  0xA6,
};

/**
 *  Known CPU vendors
 */
enum class CpuVendor {
  Unknown,
  AMD,
  Intel
  /* Add more processors here if needed */
};

/**
 *  Intel CPU generations (starting from 0)
 */
enum class CpuGeneration {
  Unknown,
  Penryn,
  Nehalem,
  Westmere,
  SandyBridge,
  IvyBridge,
  Haswell,
  Broadwell,
  Skylake,
  KabyLake,
  CoffeeLake,
  CannonLake,
  IceLake,
  CometLake,
  MaxGeneration
};

/* INTEL:   "GenuineIntel" */
static constexpr uint32_t signature_INTEL_ebx = 0x756e6547;
static constexpr uint32_t signature_INTEL_edx = 0x49656e69;
static constexpr uint32_t signature_INTEL_ecx = 0x6c65746e;

/* AMD:     "AuthenticAMD" */
static constexpr uint32_t signature_AMD_ebx = 0x68747541;
static constexpr uint32_t signature_AMD_edx = 0x69746e65;
static constexpr uint32_t signature_AMD_ecx = 0x444d4163;
