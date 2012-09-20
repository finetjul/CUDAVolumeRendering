/** @file cudaRendererInformation.h
*
*  @brief File for renderer information holding structure used for volume ray casting
*
*  @author John Stuart Haberl Baxter (Dr. Peter's Lab at Robarts Research Institute)
*  @note First documented on March 27, 2011
*
*  @note This is primarily an internal file used by the vtkCUDARendererInformationHandler and CUDA_renderAlgo to store and communicate constants
*
*/

#ifndef __CUDA_containerRendererInformation_h
#define __CUDA_containerRendererInformation_h

// CUDA Volume Rendering includes
#include "vector_types.h"

/** @brief A stucture located on the CUDA hardware that holds all the information required about the renderer.
*
*/
typedef struct __align__(16)
{
  uint2 actualResolution;        /**< The resolution of the rendering screen */

  float ViewToVoxelsMatrix[16];  /**< 4x4 matrix mapping the view space (0 to 1 in each direction, with 0 and 1 in x and y being the borders of the screen, and 0 and 1 in z being the clipping planes) to the volume space */

  int NumberOfClippingPlanes;    /**< Number of additional user defined clipping planes to a maximum of 6 */
  float ClippingPlanes[24];      /**< Parameters defining each of the additional user defined clipping planes */

  //Gradient shading constants
  float gradShadeScale;      /**< Multiplicative constant for flat-like shading of the volume */
  float gradShadeShift;      /**< Additive constant for the flat-like shading of the volume */

} cudaRendererInformation;

#endif
