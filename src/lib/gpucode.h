/*
 * Header file for gpu code.
 */

#ifndef __GPUCODE_H
#define __GPUCODE_H
//everything is guarded by SMINA_GPU
// CUDA runtime
#include <cuda_runtime.h>
#include <vector>

struct GPUSplineInfo
{
	unsigned n; //number of components
	float **splines; //pointer to spline data in device memory, size is number of components
	float fraction; //how spline is binned
	float cutoff; //where to stop

	GPUSplineInfo(): n(0), splines(NULL), fraction(0), cutoff(0) {}
};

/* float3 reads/writes can't be coalesced into a single load/store. But
   float4 ops can. Rather than padding 3-dimensional data to exploit this,
   pack it in with a relevant piece of 1-dimensional data. NB: without
   __align__, the compiler can't do this coalescing. */
struct __align__(sizeof(float4)) atom_params{
    float3 coords;
    float charge;
};

struct __align__(sizeof(float4)) force_energy_tup{
    float3 minus_force;
    float energy;

    __host__ __device__ force_energy_tup(void);
    __host__ __device__ force_energy_tup(float3 f, float e) : minus_force(f), energy(e){};
};

struct GPUNonCacheInfo
{
	unsigned nlig_atoms, nrec_atoms;
	float cutoff_sq;

	//device pointers for grid data
	float3 gridends; //max range of grid
	float3 gridbegins; //min range of grid
    
	//device pointers for ligand data
    atom_params *lig_atoms;
    force_energy_tup *lig_penalties;
    force_energy_tup *result;
	unsigned *types; //n

	//device pointers for receptor data
    atom_params *rec_atoms;
	unsigned *rectypes;
    
	//triangular matrix of spline data, indexed by type, device pointer
	unsigned ntypes; //number of atom types; also, dimension of triangular splineInfo
	GPUSplineInfo *splineInfo;
};

void evaluate_splines_host(const GPUSplineInfo& spInfo, float r, float *device_vals, float *device_derivs);
float single_point_calc(const GPUNonCacheInfo *dinfo, force_energy_tup *out, float slope, unsigned nlig_atoms, unsigned nrec_atoms, float v);


#endif
