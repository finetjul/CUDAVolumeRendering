/** @file CUDA_vtkCUDAVolumeMapper_renderAlgo.h
*
*  @brief Header file with definitions for different common CUDA functions for setting up and
*         running the ray casting process regardless of its structure
*
*  @author John Stuart Haberl Baxter (Dr. Peter's Lab at Robarts Research Institute)
*  @note First documented on May 6, 2012
*
*  @note This is primarily an internal file used by the vtkCUDAVolumeMapper and subclasses
*        to manage the ray casting process.
*
*/

#ifndef __CUDA_vtkCUDAVolumeMapper_renderAlgo_h
#define __CUDA_vtkCUDAVolumeMapper_renderAlgo_h

// CUDA Volume Rendering includes
#include "CUDA_containerOutputImageInformation.h"
#include "CUDA_containerRendererInformation.h"
#include "CUDA_containerVolumeInformation.h"

/** @brief Loads the ZBuffer into a 2D texture for checking during the rendering process
*
*  @param zBuffer A floating point buffer 
*  @param zBufferSizeX The size of the z buffer in the x direction
*  @param zBufferSizeY The size of the z buffer in the y direction
*
*  @pre The zBuffer consists only of numbers between 0.0f and 1.0f inclusive
*
*/
bool CUDA_vtkCUDAVolumeMapper_renderAlgo_loadZBuffer(const float* zBuffer, const int zBufferSizeX,
                                                     const int zBufferSizeY, cudaStream_t* stream);
bool CUDA_vtkCUDAVolumeMapper_renderAlgo_unloadZBuffer(cudaStream_t* stream);

/** @brief Loads an random image into a 2D CUDA array for de-artifacting
*
*  @param randomRayOffsets A 16x16 array (in 1 dimension, so 256 elements) of random numbers
*
*  @pre Each number in randomRayOffsets is between 0.0f and 1.0f inclusive
*
*/
bool CUDA_vtkCUDAVolumeMapper_renderAlgo_loadrandomRayOffsets(const float* randomRayOffsets,
                                                              cudaStream_t* stream);


/** @brief Unloads the random image from the 2D CUDA array set before
*
*/
bool CUDA_vtkCUDAVolumeMapper_renderAlgo_unloadrandomRayOffsets(cudaStream_t* stream);

#endif
