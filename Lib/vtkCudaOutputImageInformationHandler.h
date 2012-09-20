/** @file vtkCUDAOutputImageInformationHandler.h
*
*  @brief Header file defining an internal class for vtkCUDAVolumeMapper which manages information regarding the image being outputted
*
*  @author John Stuart Haberl Baxter (Dr. Peter's Lab at Robarts Research Institute)
*  @note First documented on March 28, 2011
*
*/

#ifndef __vtkCUDAOutputImageInformationHandler_h
#define __vtkCUDAOutputImageInformationHandler_h

// CUDA Volume Rendering includes
#include "CUDA_containerOutputImageInformation.h"
#include "vtkCUDAObject.h"

// VTK includes
#include <vtkObject.h>
class vtkRayCastImageDisplayHelper;
class vtkRenderer;
class vtkVolume;

/** @brief vtkCUDAOutputImageInformationHandler handles all output image, buffering, texturing and OpenGL related information on behalf of the CUDA volume mapper to facilitate the rendering and display process
*
*/
class CUDA_LIB_EXPORT vtkCUDAOutputImageInformationHandler
  : public vtkObject
  , public vtkCUDAObject
{
public:

  vtkTypeMacro (vtkCUDAOutputImageInformationHandler,vtkObject);

  /** @brief VTK compatible constructor method
  *
  */
  static vtkCUDAOutputImageInformationHandler* New();

  /** @brief Get the renderer that the handler is currently collecting information from
  *
  */
  vtkRenderer* GetRenderer();

  /** @brief Set the renderer that the handler will collect information from
  *
  *  @param renderer A vtkRenderer associated with the mapper in use
  */
  void SetRenderer(vtkRenderer* renderer);

  /** @brief Sets how the image is displayed
  *
  *  @param scaleFactor The factor by which the screen is undersampled in each direction (must be equal or greater than 1.0f, where 1.0f means full sampling)
  */
  void SetRenderOutputScaleFactor(float scaleFactor);

  /** @brief Gets the CUDA compatible container for the output image buffer location needed during rendering, and the additional information needed after rendering for displaying
  *
  */
  const cudaOutputImageInformation& GetOutputImageInfo() { return (this->OutputImageInfo); }

  /** @brief Prepares the buffers/textures/images before rendering
  *
  */
  void Prepare();

  /** @brief Displays the buffers/textures/images to the render window after the ray casting process
  *
  */
  void Display(vtkVolume* volume, vtkRenderer* renderer);

  /** @brief Updates the various available rendering parameters, reconstructing the buffers/textures/images if the render type or output image resolution has changed
  *
  */
  void Update();

protected:

  /** @brief Constructor which initializes all the displyy parameters to safe values, and create a display helper and a CUDA memory texture to help with the display process
  *
  */
  vtkCUDAOutputImageInformationHandler();

  /** @brief Destructor that deallocates the displayer helper and memory texture
  *
  */
  ~vtkCUDAOutputImageInformationHandler();

  void Deinitialize(int withData = 0);
  void Reinitialize(int withData = 0);

private:
  vtkCUDAOutputImageInformationHandler& operator=(const vtkCUDAOutputImageInformationHandler&); /**< not implemented */
  vtkCUDAOutputImageInformationHandler(const vtkCUDAOutputImageInformationHandler&); /**< not implemented */

private:
  cudaOutputImageInformation    OutputImageInfo;      /**< CUDA compatible container for the output image display information */
  vtkRayCastImageDisplayHelper*  Displayer;          /**< A VTK class solely for helping ray casters render a 2D RGBA image to the appropriate section of the render window */
  vtkRenderer*          Renderer;          /**< The vtkRenderer which information is currently being extracted from */

  int                oldRenderType;        /**< The render type used previous to the current one, used to clean up information when switching display type */
  uint2              oldResolution;        /**< The previous window size (used to determine whether or not to recreate buffers) */

  uchar4* hostOutputImage;                  /**< The image that will be textured to the screen stored on host memory */
  uchar4* deviceOutputImage;                  /**< The image that will be textured to the screen stored on device memory */

  float              RenderOutputScaleFactor;  /**< The approximate factor by which the screen is resized in order to speed up the rendering process*/

};

#endif