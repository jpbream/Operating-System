#ifndef _CPU_H_
#define _CPU_H_

#include <stdint.h>

class Intel_CPU {

private:

    struct CPUID_Limit {
        unsigned int min;
        unsigned int max;
    };

    // input limits when querying basic
    // processor information
    CPUID_Limit basic = { 0 };

    // input limits when querying extended
    // processor information
    CPUID_Limit extended = { 0x80000000 };

    // if bit EFLAGS_BIT in eflags
    // can be modified, cpuid is enabled
	bool supported;

    // INPUT EAX = 0
	char vendorID[13]; //ebx, edx, ecx from low to high

    // INPUT EAX = 1
    uint32_t cpuSignature; // eax, also high bits of PSN
    uint32_t miscInfo; // ebx
    uint32_t newFeatures; // ecx
    uint32_t features; // edx

    // INPUT EAX = 2
    uint32_t descriptor1; // eax
    uint32_t descriptor2; // ebx
    uint32_t descriptor3; // ecx
    uint32_t descriptor4; // edx

    uint8_t cacheDescriptorCodes[64];
    int numCacheDescriptors;
    
    // INPUT EAX = 3
    uint32_t midPSN; // edx
    uint32_t lowPSN; // ecx

    // PSN should be displayed as
    // XXXX-XXXX-XXXX-XXXX-XXXX-XXXX

    // INPUT EAX = 0X80000001
    // more features

    // INPUT EAX = 0X80000002 - 4
    // edx, ecx, ebx, eax, low to high
    char* brandString;

    const char* GetDescriptorName(uint8_t descriptorCode);

public:
	Intel_CPU();

    // features sent to the edx register
	enum Feat1 {

        // fpu is on chip
        FEAT_FPU = 1 << 0,

        // virtual mode is supported
        FEAT_VME = 1 << 1,

        // breakpoints supported
        FEAT_DE = 1 << 2,

        // 4 Mb pages supported
        FEAT_PSE = 1 << 3,

        // time stamp counter supported
        FEAT_TSC = 1 << 4,

        // model specific registers supported
        FEAT_MSR = 1 << 5,

        // physical address more than 32 bits
        FEAT_PAE = 1 << 6,

        // machine check exceptions
        FEAT_MCE = 1 << 7,

        // compare and exchange 8 bytes instruction
        FEAT_CX8 = 1 << 8,

        // on chip APIC hardware
        FEAT_APIC = 1 << 9,

        // fast system calls
        FEAT_SEP = 1 << 11,

        // memory type range registers
        FEAT_MTRR = 1 << 12,

        // page global enable
        FEAT_PGE = 1 << 13,

        // machine check architecture
        FEAT_MCA = 1 << 14,

        // cmov supported
        FEAT_CMOV = 1 << 15,

        // page attribute table
        FEAT_PAT = 1 << 16,

        // 36 bit page extension
        FEAT_PSE36 = 1 << 17,

        // processor serial number supported
        FEAT_PSN = 1 << 18,

        // clflush supported
        FEAT_CLFSH = 1 << 19,

        // debug store enabled
        FEAT_DS = 1 << 21,

        // temperature modulation supported
        FEAT_ACPI = 1 << 22,

        // mmx supported
        FEAT_MMX = 1 << 23,

        // fast floating point save and restore
        FEAT_FXSR = 1 << 24,

        // sse enabled
        FEAT_SSE = 1 << 25,

        // sse2 enabled
        FEAT_SSE2 = 1 << 26,

        // self snoop
        FEAT_SS = 1 << 27,

        // hyper threading
        FEAT_HTT = 1 << 28,

        // thermal monitor supported
        FEAT_TM = 1 << 29

	};

    // features sent to the ecx register
    // available on newer cpu's
	enum Feat2 {

        FEAT_SSE3 = 1 << 0,
        FEAT_PCLMUL = 1 << 1,
        FEAT_DTES64 = 1 << 2,
        FEAT_MONITOR = 1 << 3,
        FEAT_DS_CPL = 1 << 4,
        FEAT_VMX = 1 << 5,
        FEAT_SMX = 1 << 6,
        FEAT_EST = 1 << 7,
        FEAT_TM2 = 1 << 8,
        FEAT_SSSE3 = 1 << 9,
        FEAT_CID = 1 << 10,
        FEAT_FMA = 1 << 12,
        FEAT_CX16 = 1 << 13,
        FEAT_ETPRD = 1 << 14,
        FEAT_PDCM = 1 << 15,
        FEAT_PCIDE = 1 << 17,
        FEAT_DCA = 1 << 18,
        FEAT_SSE4_1 = 1 << 19,
        FEAT_SSE4_2 = 1 << 20,
        FEAT_x2APIC = 1 << 21,
        FEAT_MOVBE = 1 << 22,
        FEAT_POPCNT = 1 << 23,
        FEAT_AES = 1 << 25,
        FEAT_XSAVE = 1 << 26,
        FEAT_OSXSAVE = 1 << 27,
        FEAT_AVX = 1 << 28

	};

	const char* VendorID();
    //const char* Name();
    //const char* BrandNameShort();
    const char* BrandNameLong();
	bool QueryFeature(Feat1 feature);
	bool QueryFeature(Feat2 feature);

    int NumCacheDescriptors();
    const char* GetCacheDescriptor(int idx);

};

#endif