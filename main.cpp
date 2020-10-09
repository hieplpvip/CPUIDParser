#include <cstdio>
#include "definitions.hpp"

CpuVendor cpuVendor {CpuVendor::Unknown};
CpuGeneration cpuGeneration {CpuGeneration::Unknown};
const char* cpuGenerationText {"Unknown"};
uint32_t cpuFamily {};
uint32_t cpuModel {};
uint32_t cpuStepping {};
uint32_t cpuMaxLevel {};
uint32_t cpuMaxLevelExt {0x80000000};

bool getCpuid(uint32_t no, uint32_t count, uint32_t *a, uint32_t *b=nullptr, uint32_t *c=nullptr, uint32_t *d=nullptr) {
  uint32_t eax = 0, ebx = 0, ecx = 0, edx = 0;

  bool supported = (no & 0x80000000) ? cpuMaxLevelExt >= no : cpuMaxLevel >= no;

  // At least pass zeroes on failure
  if (supported) {
    asm volatile ("xchgq %%rbx, %q1\n"
                  "cpuid\n"
                  "xchgq %%rbx, %q1"
                  : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
                  : "0" (no), "2" (count));
  }

  if (a) *a = eax;
  if (b) *b = ebx;
  if (c) *c = ecx;
  if (d) *d = edx;

  return supported;
}

void parseCPUID() {
  // Start with detecting CPU vendor
  uint32_t b = 0, c = 0, d = 0;
  getCpuid(0, 0, &cpuMaxLevel, &b, &c, &d);
  if (b == signature_INTEL_ebx && c == signature_INTEL_ecx && d == signature_INTEL_edx)
    cpuVendor = CpuVendor::Intel;
  else if (b == signature_AMD_ebx && c == signature_AMD_ecx && d == signature_AMD_edx)
    cpuVendor = CpuVendor::AMD;

  getCpuid(0x80000000, 0, &cpuMaxLevelExt);

  // Only do extended model checking on supported Intel
  if (cpuVendor != CpuVendor::Intel || cpuMaxLevel < 1)
    return;

  // Detect CPU family and model
  union {
    CpuVersion fmt;
    uint32_t raw;
  } ver {};

  getCpuid(1, 0, &ver.raw);
  cpuFamily = ver.fmt.family;
  if (cpuFamily == 15) cpuFamily += ver.fmt.extendedFamily;

  cpuModel = ver.fmt.model;
  if (cpuFamily == 15 || cpuFamily == 6)
    cpuModel |= ver.fmt.extendedModel << 4;
  cpuStepping = ver.fmt.stepping;

  // Last but not least detect CPU generation
  // Keep this mostly in sync to cpuid_set_cpufamily from osfmk/i386/cpuid.c
  if (ver.fmt.family == 6) {
    switch (cpuModel) {
      case CPU_MODEL_PENRYN:
        cpuGeneration = CpuGeneration::Penryn;
        cpuGenerationText = "Penryn";
        break;
      case CPU_MODEL_NEHALEM:
      case CPU_MODEL_FIELDS:
      case CPU_MODEL_DALES:
      case CPU_MODEL_NEHALEM_EX:
        cpuGeneration = CpuGeneration::Nehalem;
        cpuGenerationText = "Nehalem";
        break;
      case CPU_MODEL_DALES_32NM:
      case CPU_MODEL_WESTMERE:
      case CPU_MODEL_WESTMERE_EX:
        cpuGeneration = CpuGeneration::Westmere;
        cpuGenerationText = "Westmere";
        break;
      case CPU_MODEL_SANDYBRIDGE:
      case CPU_MODEL_JAKETOWN:
        cpuGeneration = CpuGeneration::SandyBridge;
        cpuGenerationText = "Sandy Bridge";
        break;
      case CPU_MODEL_IVYBRIDGE:
      case CPU_MODEL_IVYBRIDGE_EP:
        cpuGeneration = CpuGeneration::IvyBridge;
        cpuGenerationText = "Ivy Bridge";
        break;
      case CPU_MODEL_HASWELL:
      case CPU_MODEL_HASWELL_EP:
      case CPU_MODEL_HASWELL_ULT:
      case CPU_MODEL_CRYSTALWELL:
        cpuGeneration = CpuGeneration::Haswell;
        cpuGenerationText = "Haswell";
        break;
      case CPU_MODEL_BROADWELL:
      case CPU_MODEL_BRYSTALWELL:
        cpuGeneration = CpuGeneration::Broadwell;
        cpuGenerationText = "Broadwell";
        break;
      case CPU_MODEL_SKYLAKE:
      case CPU_MODEL_SKYLAKE_DT:
      case CPU_MODEL_SKYLAKE_W:
        cpuGeneration = CpuGeneration::Skylake;
        cpuGenerationText = "Skylake";
        break;
      case CPU_MODEL_KABYLAKE:
      case CPU_MODEL_KABYLAKE_DT:
        // Kaby has 0x9 stepping, and Coffee use 0xA / 0xB stepping.
        if (ver.fmt.stepping == 9) {
          cpuGeneration = CpuGeneration::KabyLake;
          cpuGenerationText = "Kaby Lake";
        } else {
          cpuGeneration = CpuGeneration::CoffeeLake;
          cpuGenerationText = "Coffee Lake";
        }
        break;
      case CPU_MODEL_CANNONLAKE:
        cpuGeneration = CpuGeneration::CannonLake;
        cpuGenerationText = "Cannon Lake";
        break;
      case CPU_MODEL_ICELAKE_Y:
      case CPU_MODEL_ICELAKE_U:
      case CPU_MODEL_ICELAKE_SP:
        cpuGeneration = CpuGeneration::IceLake;
        cpuGenerationText = "Ice Lake";
        break;
      case CPU_MODEL_COMETLAKE_Y:
      case CPU_MODEL_COMETLAKE_U:
        cpuGeneration = CpuGeneration::CometLake;
        cpuGenerationText = "Comet Lake";
        break;
      default:
        cpuGeneration = CpuGeneration::Unknown;
        break;
    }
  }
}

void printInfo() {
  printf("CPU vendor: %s\n", (cpuVendor == CpuVendor::Intel) ? "Intel" : "AMD");
  printf("CPU max level: 0x%x\n", cpuMaxLevel);
  printf("CPU max level (ext): 0x%x\n", cpuMaxLevelExt);

  // Only do extended model checking on supported Intel
  if (cpuVendor != CpuVendor::Intel || cpuMaxLevel < 1)
    return;

  printf("CPU family: %d\n", cpuFamily);
  printf("CPU model: %d\n", cpuModel);
  printf("CPU stepping: 0x%x\n", cpuStepping);
  printf("CPU generation: %d (%s)\n", cpuGeneration, cpuGenerationText);
}

int main() {
  parseCPUID();
  printInfo();
  return 0;
}
