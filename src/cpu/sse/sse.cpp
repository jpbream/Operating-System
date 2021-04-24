#include "sse.h"
#include "cpu.h"
#include "print.h"

extern "C" void InitSSE();

void SSE::Init()
{
	CPUInfo cpu;
	if ( !cpu.QueryFeature(CPUInfo::FEAT_AVX) || !cpu.QueryFeature(CPUInfo::FEAT_XSAVE) ) {
        printf("AVX %s\n", cpu.QueryFeature(CPUInfo::FEAT_AVX) ? "Supported" : "Not Supported");
		printf("XSAVE %s\n", cpu.QueryFeature(CPUInfo::FEAT_XSAVE) ? "Supported" : "Not Supported");
        while (true);
	}

	InitSSE();
}