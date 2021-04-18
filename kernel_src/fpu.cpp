#include "fpu.h"
#include "cpu.h"
#include "print.h"

extern "C" bool DetectFPU();
extern "C" void InitFPU();

void FPU::Init()
{
	CPUInfo cpu;
	if ( !cpu.QueryFeature(CPUInfo::FEAT_FPU) ) {
		if ( !DetectFPU() ) {
			printf("No FPU Detected\n");
			while ( true );
		}
	}

	InitFPU();
}