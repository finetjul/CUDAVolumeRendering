/** @file vtkCUDAOutputImageInformationHandler.cxx
*
*  @brief An internal class for vtkCUDAVolumeMapper which manages information regarding the image being outputted
*
*  @author John Stuart Haberl Baxter (Dr. Peter's Lab at Robarts Research Institute)
*  @note First documented on March 28, 2011
*
*/

#include "vtkCUDAOutputImageInformationHandler.h"

#include "vector_functions.h"
#include "vtkgl.h"
#include "cuda_runtime_api.h"

// vtk base
#include <vtkObjectFactory.h>
#include <vtkRayCastImageDisplayHelper.h>
#include <vtkRenderer.h>

vtkStandardNewMacro(vtkCUDAOutputImageInformationHandler);

vtkCUDAOutputImageInformationHandler::vtkCUDAOutputImageInformationHandler()
  {
  this->Renderer = 0;
  this->Displayer = vtkRayCastImageDisplayHelper::New();
  this->RenderOutputScaleFactor = 1.0f;
  this->OutputImageInfo.resolution.x = this->OutputImageInfo.resolution.y = 0;
  this->oldResolution.x = this->oldResolution.y = 0;
  this->OutputImageInfo.rayIncX = this->OutputImageInfo.rayStartX = 0;
  this->OutputImageInfo.rayIncY = this->OutputImageInfo.rayStartY = 0;
  this->OutputImageInfo.rayIncZ = this->OutputImageInfo.rayStartZ = 0;
  this->hostOutputImage = 0;
  this->deviceOutputImage = 0;
  this->oldRenderType = 1;
  this->Reinitialize();
  }


vtkCUDAOutputImageInformationHandler::~vtkCUDAOutputImageInformationHandler()
  {
  this->Deinitialize();
  if(this->Displayer) this->Displayer->UnRegister(this);
  }

void vtkCUDAOutputImageInformationHandler::Deinitialize(int withData)
  {  
  if(this->OutputImageInfo.numSteps) cudaFree(this->OutputImageInfo.numSteps);
  if(this->OutputImageInfo.rayIncX) cudaFree(this->OutputImageInfo.rayIncX);
  if(this->OutputImageInfo.rayIncY) cudaFree(this->OutputImageInfo.rayIncY);
  if(this->OutputImageInfo.rayIncZ) cudaFree(this->OutputImageInfo.rayIncZ);
  if(this->OutputImageInfo.rayStartX) cudaFree(this->OutputImageInfo.rayStartX);
  if(this->OutputImageInfo.rayStartY) cudaFree(this->OutputImageInfo.rayStartY);
  if(this->OutputImageInfo.rayStartZ) cudaFree(this->OutputImageInfo.rayStartZ);
  if(this->hostOutputImage) delete this->hostOutputImage;
  if(this->deviceOutputImage) cudaFree(this->deviceOutputImage);
  this->OutputImageInfo.resolution.x = this->OutputImageInfo.resolution.y = 0;
  this->oldResolution.x = this->oldResolution.y = 0;
  this->OutputImageInfo.rayIncX = this->OutputImageInfo.rayStartX = 0;
  this->OutputImageInfo.rayIncY = this->OutputImageInfo.rayStartY = 0;
  this->OutputImageInfo.rayIncZ = this->OutputImageInfo.rayStartZ = 0;
  this->hostOutputImage = 0;
  this->deviceOutputImage = 0;
  }

void vtkCUDAOutputImageInformationHandler::Reinitialize(int withData)
  {
  }

void vtkCUDAOutputImageInformationHandler::SetRenderOutputScaleFactor(float scaleFactor)
  {
  this->RenderOutputScaleFactor = (scaleFactor > 1.0) ? scaleFactor : 1.0;
  this->Update();
  }

vtkRenderer* vtkCUDAOutputImageInformationHandler::GetRenderer()
  {
  return this->Renderer;
  }

void vtkCUDAOutputImageInformationHandler::SetRenderer(vtkRenderer* renderer)
  {
  this->Renderer = renderer;
  this->Update();
  }

void vtkCUDAOutputImageInformationHandler::Prepare()
  {
  this->OutputImageInfo.deviceOutputImage = this->deviceOutputImage;
  }

void vtkCUDAOutputImageInformationHandler::Display(vtkVolume* volume, vtkRenderer* renderer)
  {

  this->ReserveGPU();
  cudaStreamSynchronize(*(this->GetStream()));

  //if desired, render using the fulling compatible displayer tool
  cudaMemcpyAsync( this->hostOutputImage, this->deviceOutputImage, 4*sizeof(unsigned char)*this->OutputImageInfo.resolution.x*this->OutputImageInfo.resolution.y, cudaMemcpyDeviceToHost, *(this->GetStream()));
  int imageMemorySize[2];
  imageMemorySize[0] = this->OutputImageInfo.resolution.x;
  imageMemorySize[1] = this->OutputImageInfo.resolution.y;
  int imageOrigin[2] = {0,0};
  this->Displayer->RenderTexture(volume,renderer,imageMemorySize,imageMemorySize,imageMemorySize,imageOrigin,0.001,(unsigned char*) this->hostOutputImage);

  this->ReserveGPU();
  cudaStreamSynchronize(*(this->GetStream()));

  }

void vtkCUDAOutputImageInformationHandler::Update()
  {

  if (this->Renderer == 0) return;

  // Image size update.
  int *size = this->Renderer->GetSize();
  this->OutputImageInfo.resolution.x = size[0] / this->RenderOutputScaleFactor;
  this->OutputImageInfo.resolution.y = size[1] / this->RenderOutputScaleFactor;

  //make it such that every thread fits within the solid for optimal access coalescing
  this->OutputImageInfo.resolution.x += (this->OutputImageInfo.resolution.x % 16) ? 16-(this->OutputImageInfo.resolution.x % 16) : 0;
  this->OutputImageInfo.resolution.y += (this->OutputImageInfo.resolution.y % 16) ?16-(this->OutputImageInfo.resolution.y % 16): 0;
  if(this->OutputImageInfo.resolution.y < 256) this->OutputImageInfo.resolution.y = 256;
  if(this->OutputImageInfo.resolution.x < 256) this->OutputImageInfo.resolution.x = 256;

  //if our image size hasn't changed, we don't have to reallocate any buffers, so we can just leave
  if(this->OutputImageInfo.resolution.x == this->oldResolution.x && this->OutputImageInfo.resolution.y == this->oldResolution.y)
    return;

  //reset the values for the old resolution to the current (for the next update)
  this->oldResolution = this->OutputImageInfo.resolution;

  //allocate the buffers used for intermediate output results in rendering
  this->ReserveGPU();
  if(this->OutputImageInfo.numSteps) cudaFree(this->OutputImageInfo.numSteps);
  cudaMalloc( (void**) &this->OutputImageInfo.numSteps, sizeof(float)*this->OutputImageInfo.resolution.x * this->OutputImageInfo.resolution.y);
  if(this->OutputImageInfo.rayIncX) cudaFree(this->OutputImageInfo.rayIncX);
  cudaMalloc( (void**) &this->OutputImageInfo.rayIncX, sizeof(float)*this->OutputImageInfo.resolution.x * this->OutputImageInfo.resolution.y);
  if(this->OutputImageInfo.rayIncY) cudaFree(this->OutputImageInfo.rayIncY);
  cudaMalloc( (void**) &this->OutputImageInfo.rayIncY, sizeof(float)*this->OutputImageInfo.resolution.x * this->OutputImageInfo.resolution.y);
  if(this->OutputImageInfo.rayIncZ) cudaFree(this->OutputImageInfo.rayIncZ);
  cudaMalloc( (void**) &this->OutputImageInfo.rayIncZ, sizeof(float)*this->OutputImageInfo.resolution.x * this->OutputImageInfo.resolution.y);
  if(this->OutputImageInfo.rayStartX) cudaFree(this->OutputImageInfo.rayStartX);
  cudaMalloc( (void**) &this->OutputImageInfo.rayStartX, sizeof(float)*this->OutputImageInfo.resolution.x * this->OutputImageInfo.resolution.y);
  if(this->OutputImageInfo.rayStartY) cudaFree(this->OutputImageInfo.rayStartY);
  cudaMalloc( (void**) &this->OutputImageInfo.rayStartY, sizeof(float)*this->OutputImageInfo.resolution.x * this->OutputImageInfo.resolution.y);
  if(this->OutputImageInfo.rayStartZ) cudaFree(this->OutputImageInfo.rayStartZ);
  cudaMalloc( (void**) &this->OutputImageInfo.rayStartZ, sizeof(float)*this->OutputImageInfo.resolution.x * this->OutputImageInfo.resolution.y);

  //allocate the buffers
  this->ReserveGPU();
  if(this->deviceOutputImage) cudaFree(this->deviceOutputImage);
  cudaMalloc( (void**) &this->deviceOutputImage, 4*sizeof(unsigned char)*this->OutputImageInfo.resolution.x * this->OutputImageInfo.resolution.y);
  if(this->hostOutputImage) delete this->hostOutputImage;
  this->hostOutputImage = new uchar4[this->OutputImageInfo.resolution.x * this->OutputImageInfo.resolution.y];

  }