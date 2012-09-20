/** @file vtkCUDAVolumeMapper.h
*
*  @brief Header file defining a volume mapper (ray caster) using CUDA kernels for parallel ray calculation
*
*  @author John Stuart Haberl Baxter (Dr. Peter's Lab at Robarts Research Institute)
*  @note First documented on May 12, 2012
*
*/

#ifndef __vtkCUDA1DVolumeMapper_h
#define __vtkCUDA1DVolumeMapper_h

#include "vtkCUDAVolumeMapper.h"
class vtkCUDA1DTransferFunctionInformationHandler;

// VTK includes
class vtkMutexLock;

/** @brief vtkCUDA1DVolumeMapper is a volume mapper, taking a set of 3D image data objects, volume and renderer as input and creates a 2D ray casted projection of the scene which is then displayed to screen
*
*/
class CUDA_LIB_EXPORT vtkCUDA1DVolumeMapper
  : public vtkCUDAVolumeMapper
{
public:

  vtkTypeMacro (vtkCUDA1DVolumeMapper,vtkCUDAVolumeMapper);

  /** @brief VTK compatible constructor method
  *
  */
  static vtkCUDA1DVolumeMapper *New();

  virtual void SetInputInternal( vtkImageData * image, int frame);
  virtual void ClearInputInternal();
  virtual void ChangeFrameInternal(unsigned int frame);
  virtual void InternalRender (  vtkRenderer* ren, vtkVolume* vol,
    const cudaRendererInformation& rendererInfo,
    const cudaVolumeInformation& volumeInfo,
    const cudaOutputImageInformation& outputInfo );

protected:
  /** @brief Constructor which initializes the number of frames, rendering type and other constants to safe initial values, and creates the required information handlers
  *
  */
  vtkCUDA1DVolumeMapper();

  /** @brief Destructor which deallocates the various information handlers and matrices
  *
  */
  ~vtkCUDA1DVolumeMapper();
  virtual void Reinitialize(int withData = 0);
  virtual void Deinitialize(int withData = 0);

  vtkCUDA1DTransferFunctionInformationHandler* transferFunctionInfoHandler;

  static vtkMutexLock* tfLock;

private:
  vtkCUDA1DVolumeMapper operator=(const vtkCUDA1DVolumeMapper&); /**< not implemented */
  vtkCUDA1DVolumeMapper(const vtkCUDA1DVolumeMapper&); /**< not implemented */

};

#endif
