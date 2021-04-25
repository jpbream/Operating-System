#include "sse.h"
#include "cpu.h"
#include "print.h"

extern "C" void InitSSE();

void SSE::Init()
{
	// virtualbox and qemu seem to lie about their support for these
	#if !defined VIRTUALBOX && !defined QEMU
	CPUInfo cpu;
	if ( !cpu.QueryFeature(CPUInfo::FEAT_AVX) || !cpu.QueryFeature(CPUInfo::FEAT_XSAVE) ) {
        printf("AVX %s\n", cpu.QueryFeature(CPUInfo::FEAT_AVX) ? "Supported" : "Not Supported");
		printf("XSAVE %s\n", cpu.QueryFeature(CPUInfo::FEAT_XSAVE) ? "Supported" : "Not Supported");
        while (true);
	}
	#endif

	InitSSE();
}