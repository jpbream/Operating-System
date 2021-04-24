#include "sse.h"
#include "cpu.h"
#include "print.h"

extern "C" void InitSSE();

void SSE::Init()
{
	CPUInfo cpu;
	if ( !cpu.QueryFeature(CPUInfo::FEAT_AVX) || !cpu.QueryFeature(CPUInfo::FEAT_XSAVE) || !cpu.QueryFeature(CPUInfo::FEAT_OSXSAVE) ) {
        printf("AVX Not Supported\n");
        return;
	}

	InitSSE();
}