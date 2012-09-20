/** @file cudaOutputImageInformation.h
*
*  @brief File for output image and intermediate information holding structure used for volume ray casting
*
*  @author John Stuart Haberl Baxter (Dr. Peter's Lab at Robarts Research Institute)
*  @note First documented on March 28, 2011
*
*  @note This is primarily an internal file used by the vtkCUDAOutputImageInformationHandler and CUDA_renderAlgo to store and communicate constants
*
*/

#ifndef __CUDA_containerOutputImageInformation_h
#define __CUDA_containerOutputImageInformation_h

// CUDA Volume Rendering includes
#include "vector_types.h"

/** @brief A stucture located on the CUDA hardware that holds all the information required about the output image.
*
*/
typedef struct
{
  uint2       resolution;        /**< The resolution of the texture/image that will be textured to the screen */
  uchar4*     deviceOutputImage; /**< The texture/image that will be textured to the screen on device memory */

  float*      rayStartX;         /**< The ray starting location buffer (x component) */
  float*      rayStartY;         /**< The ray starting location buffer (y component) */
  float*      rayStartZ;         /**< The ray starting location buffer (z component) */
  float*      rayIncX;           /**< The ray increment amount buffer (x component) */
  float*      rayIncY;           /**< The ray increment amount buffer (y component) */
  float*      rayIncZ;           /**< The ray increment amount buffer (z component) */
  float*      numSteps;          /**< The number of sample points on the ray */

} cudaOutputImageInformation;

#endif
