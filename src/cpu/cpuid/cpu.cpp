#include "cpu.h"
#include "print.h"

// which bit in eflags to test to determine if
// the cpuid instruction set is available
extern "C" constexpr int EFLAGS_BIT = 21;

// checks if the cpuid instruction set is enabled
extern "C" bool CheckCPUID();

// get the max allowed input to the cpuid instruction
extern "C" unsigned int GetBasicMax();
extern "C" unsigned int GetExtendedMax();

// get the vendor id
extern "C" void GetVendorID(char* id);

extern "C" uint32_t GetProcessorSignature();
extern "C" uint32_t GetMiscInfo();
extern "C" uint32_t GetFeatures();
extern "C" uint32_t GetNewFeatures();
extern "C" uint32_t CheckERMSB();

extern "C" int GetDescriptors(uint8_t* codes);

extern "C" uint32_t GetMidPSN();
extern "C" uint32_t GetLowPSN();

extern "C" char* GetBrandString();

CPUInfo::CPUInfo()
{
	supported = CheckCPUID();

	if ( !supported ) {
		printf("CPUID instructions not available!\n");
		return;
	}

	basic.max = GetBasicMax();
	extended.max = GetExtendedMax();

	GetVendorID(vendorID);
	vendorID[12] = '\0';

	cpuSignature = GetProcessorSignature();
	miscInfo = GetMiscInfo();
	features = GetFeatures();
	newFeatures = GetNewFeatures();

	// this returns a garbage value in VirtualBox
	// don't use it there
	numCacheDescriptors = GetDescriptors(cacheDescriptorCodes);

	midPSN = GetMidPSN();
	lowPSN = GetLowPSN();

	brandString = GetBrandString();

}

const char* CPUInfo::GetDescriptorName(uint8_t descriptorCode)
{

	// these are just the descriptors for my laptop
	// there are way more
	switch ( descriptorCode ) {

	case 0x00:
		return "";
	case 0x5A:
		return "Data TLB0: 2-MB or 4-MB pages, 4-way associative, 32 entries\n";
	case 0x03:
		return "Data TLB: 4-KB Pages, 4-way set associative, 64 entries\n";
	case 0x76:
		return "Instruction TLB: 2M/4M pages, fully associative, 8 entries\n";
	case 0xFF:
		return "CPUID Leaf 2 does not report cache descriptor information; use CPUID Leaf 4 to query cache parameters\n";
	case 0xB2:
		return "Instruction TLB: 4-KB pages, 4-way set associative, 64 entries\n";
	case 0xF0:
		return "64-byte Prefetching\n";
	case 0xCA:
		return "Shared 2nd-level TLB: 4 KB pages, 4-way set associative, 512 entries\n";
	}
}

const char* CPUInfo::VendorID()
{
	return vendorID;
}

const char* CPUInfo::BrandNameLong()
{
	return brandString;
}

bool CPUInfo::QueryFeature(Feat1 feature)
{
	return features & feature > 0;
}

bool CPUInfo::QueryFeature(Feat2 feature)
{
	return newFeatures & feature > 0;
}

bool CPUInfo::HasERMSB()
{
	return CheckERMSB() & (1<<9) > 0;
}

int CPUInfo::NumCacheDescriptors()
{
	return numCacheDescriptors;
}

const char* CPUInfo::GetCacheDescriptor(int idx)
{
	return GetDescriptorName(cacheDescriptorCodes[idx]);
}