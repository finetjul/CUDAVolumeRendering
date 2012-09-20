/** @file vtkCUDAVolumeMapper.h
*
*  @brief Header file defining a volume mapper (ray caster) using CUDA kernels for parallel ray calculation
*
*  @author John Stuart Haberl Baxter (Dr. Peter's Lab at Robarts Research Institute)
*  @note First documented on March 29, 2011
*
*/

#ifndef __vtkCUDAVolumeMapper_h
#define __vtkCUDAVolumeMapper_h

// CUDA Volume Rendering includes
#include "vtkCUDAObject.h"
#include "CUDA_containerOutputImageInformation.h"
#include "CUDA_containerRendererInformation.h"
#include "CUDA_containerVolumeInformation.h"
#include "CUDA_container1DTransferFunctionInformation.h"
class vtkCUDAOutputImageInformationHandler;
class vtkCUDARendererInformationHandler;
class vtkCUDAVolumeInformationHandler;

// VTK includes
#include <vtkVolumeMapper.h>
class vtkMatrix4x4;
class vtkRenderer;
class vtkRenderWindow;
class vtkTransform;
class vtkVolume;
class vtkVolumeProperty;

// STD includes
#include <map>

/** @brief vtkCUDAVolumeMapper is an abstract CUDA volume mapper
*   Taking a set of 3D image data objects, volume and renderer as input and
*   creates a 2D ray casted projection of the scene which is then displayed to screen
*/
class CUDA_LIB_EXPORT vtkCUDAVolumeMapper
  : public vtkVolumeMapper
  , public vtkCUDAObject
{
public:
  vtkTypeMacro (vtkCUDAVolumeMapper,vtkVolumeMapper);
  void PrintSelf( ostream& os, vtkIndent indent );

  /** @brief Sets the 3D image data for the first frame in the 4D sequence
  *
  *  @param image The 3D image data.

  *  @pre All dataset being rendered are the same size, anatomy, patient and modality
  */
  virtual void SetInput( vtkImageData * image);

  /** @brief Sets the 3D image data for a particular frame in the 4D sequence
  *
  *  @param image The 3D image data.
  *  @param frame The desired frame number when this data is rendered
  *
  *  @pre All dataset being rendered are the same size, anatomy, patient and modality
  */
  void SetInput( vtkImageData * image, int frame);
  virtual void SetInputInternal( vtkImageData * image, int frame) = 0;

  /** @brief Uses the provided renderer and volume to render the image data at the current frame
  *
  *  @note This is an internal method used primarily by the rendering pipeline
  */
  void Render(vtkRenderer* renderer, vtkVolume* volume);

  /** @brief Perform specific rendering process
  *
  *  @note This is an internal method used primarily by the raycasting hierarchy structure
  */
  virtual void InternalRender (  vtkRenderer* ren, vtkVolume* vol,
    const cudaRendererInformation& rendererInfo,
    const cudaVolumeInformation& volumeInfo,
    const cudaOutputImageInformation& outputInfo ) = 0;

  /** @brief Sets how the image is displayed which is passed to the output image information handler
  *
  *  @param scaleFactor The factor by which the screen is undersampled in each direction (must be equal or greater than 1.0f, where 1.0f means full sampling)
  */
  void SetRenderOutputScaleFactor(float scaleFactor);

  /** @brief Set the strength of the photorealistic shading model which is given to the renderer information handler
  *
  *  @param darkness Floating point between 0.0f and 1.0f inclusive, where 0.0f means no shading, and 1.0f means maximal shading
  */
  void SetGradientShadingConstants(float darkness);

  /** @brief Based on hardware and properties, we may or may not be able to render using CUDA volume mapper.
  *   This indicates if 3D mapper is supported by the hardware, and if the other
  *   extensions necessary to support the specific properties are available.
  */
  virtual int IsRenderSupported(vtkRenderWindow *vtkNotUsed(window), vtkVolumeProperty *vtkNotUsed(property));

protected:
  /** @brief Constructor which initializes the number of frames, rendering type and other constants to safe initial values, and creates the required information handlers
  *
  */
  vtkCUDAVolumeMapper();

  /** @brief Destructor which deallocates the various information handlers and matrices
  *
  */
  virtual ~vtkCUDAVolumeMapper();

  /** @brief Changes the next frame to be rendered to the provided frame
  *
  *  @param frame The next frame to be rendered
  *
  *  @pre frame is a non-negative integer less than the total number of frames
  */
  void ChangeFrame(unsigned int frame);
  virtual void ChangeFrameInternal(unsigned int frame) = 0;

  /** @brief Clears all the frames in the 4D sequence
  *
  */
  void ClearInput();
  virtual void ClearInputInternal() = 0;

  virtual void Reinitialize(int withData = 0);
  virtual void Deinitialize(int withData = 0);

  vtkCUDARendererInformationHandler* RendererInfoHandler;   /**< The handler for any renderer/camera/geometry/clipping information */
  vtkCUDAVolumeInformationHandler* VolumeInfoHandler;       /**< The handler for any volume/transfer function information */
  vtkCUDAOutputImageInformationHandler* OutputInfoHandler;  /**< The handler for any output image housing/display information */

  //modified time variables used to minimize setup
  unsigned long  renModified;                /**< The last time the renderer object was modified */
  unsigned long  volModified;                /**< The last time the volume object was modified */

  /** @brief Using the mapper's volume and renderer objects, check for updates and reconstruct the appropriate matrices based on them, sending them off to the renderer information handler afterwards
  *
  *  @pre The mapper's volume and renderer objects are not null.
  */
  void ComputeMatrices();
  vtkMatrix4x4  *ViewToVoxelsMatrix;          /**< Matrix used as temporary storage for the view to voxels transformation */
  vtkMatrix4x4  *WorldToVoxelsMatrix;         /**< Matrix used as temporary storage for the voxels to view transformation */

  vtkTransform  *PerspectiveTransform;        /**< Temporary storage of the perspective transform used by the current camera */
  vtkTransform  *VoxelsTransform;             /**< Temporary storage of the user defined volume transform used to modify postion, orientation, etc... */
  vtkTransform  *VoxelsToViewTransform;       /**< Temporary storage of the voxels to view transformation used to speed the process of switching/recalculating matrices*/
  vtkTransform  *NextVoxelsToViewTransform;   /**< Temporary storage of the next voxels to view transformation used to speed the process of switching/recalculating matrices */

  bool erroredOut;                            /**< Boolean to describe whether it is safe to render */
  std::map<int, vtkImageData*> inputImages;

private:

};

#endif
