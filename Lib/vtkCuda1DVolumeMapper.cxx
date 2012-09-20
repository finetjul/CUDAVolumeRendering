/** @file vtkCUDAVolumeMapper.cxx
*
*  @brief Implementation of a volume mapper (ray caster) using CUDA kernels for parallel ray calculation
*
*  @author John Stuart Haberl Baxter (Dr. Peter's Lab at Robarts Research Institute)
*  @note First documented on May 12, 2012
*
*/

// Type
#include "vtkCUDA1DVolumeMapper.h"
#include "vtkCUDAVolumeInformationHandler.h"
#include "vtkCUDA1DTransferFunctionInformationHandler.h"

// CUDA Volume Rendering includes
#include "CUDA_vtkCUDA1DVolumeMapper_renderAlgo.h"

// Volume
#include <vtkVolume.h>
#include <vtkImageData.h>

// Rendering
#include <vtkCamera.h>
#include <vtkColorTransferFunction.h>
#include <vtkMutexLock.h>
#include <vtkObjectFactory.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderer.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>

vtkStandardNewMacro(vtkCUDA1DVolumeMapper);

vtkMutexLock* vtkCUDA1DVolumeMapper::tfLock = 0;
vtkCUDA1DVolumeMapper::vtkCUDA1DVolumeMapper()
  {
  if( !vtkCUDA1DVolumeMapper::tfLock ) vtkCUDA1DVolumeMapper::tfLock = vtkMutexLock::New();
  else tfLock->Register( this );
  this->transferFunctionInfoHandler = vtkCUDA1DTransferFunctionInformationHandler::New();
  this->Reinitialize();
  }

void vtkCUDA1DVolumeMapper::Deinitialize(int withData)
  {
  this->vtkCUDAVolumeMapper::Deinitialize(withData);
  this->ReserveGPU();
  CUDA_vtkCUDA1DVolumeMapper_renderAlgo_clearImageArray(this->GetStream());
  }

void vtkCUDA1DVolumeMapper::Reinitialize(int withData)
  {
  this->vtkCUDAVolumeMapper::Reinitialize(withData);
  this->transferFunctionInfoHandler->ReplicateObject(this, withData);
  this->ReserveGPU();
  CUDA_vtkCUDA1DVolumeMapper_renderAlgo_initImageArray(this->GetStream());
  CUDA_vtkCUDA1DVolumeMapper_renderAlgo_changeFrame(0, this->GetStream());
  }

vtkCUDA1DVolumeMapper::~vtkCUDA1DVolumeMapper()
  {
  this->Deinitialize();
  int tfLockReferenceCount = tfLock->GetReferenceCount() - 1;
  tfLock->UnRegister( this );
  if (tfLockReferenceCount == 0)
    {
    tfLock = 0;
    }
  this->transferFunctionInfoHandler->UnRegister( this );
  }

void vtkCUDA1DVolumeMapper::SetInputInternal(vtkImageData * input, int index)
  {

  //convert data to float
  const cudaVolumeInformation& VolumeInfo = this->VolumeInfoHandler->GetVolumeInfo();
  float* buffer = new float[VolumeInfo.VolumeSize.x*VolumeInfo.VolumeSize.y*VolumeInfo.VolumeSize.z];
  if(input->GetScalarType() == VTK_CHAR)
    {
    char* tempPtr = (char*) input->GetScalarPointer();
    for(int i = 0; i < VolumeInfo.VolumeSize.x*VolumeInfo.VolumeSize.y*VolumeInfo.VolumeSize.z; i++)
      buffer[i] = (float)(tempPtr[i]);
    }
  else if(input->GetScalarType() == VTK_UNSIGNED_CHAR)
    {
    unsigned char* tempPtr = (unsigned char*) input->GetScalarPointer();
    for(int i = 0; i < VolumeInfo.VolumeSize.x*VolumeInfo.VolumeSize.y*VolumeInfo.VolumeSize.z; i++)
      buffer[i] = (float)(tempPtr[i]);
    }
  else if(input->GetScalarType() == VTK_SIGNED_CHAR)
    {
    signed char* tempPtr = (signed char*) input->GetScalarPointer();
    for(int i = 0; i < VolumeInfo.VolumeSize.x*VolumeInfo.VolumeSize.y*VolumeInfo.VolumeSize.z; i++)
      buffer[i] = (float)(tempPtr[i]);
    }
  else if(input->GetScalarType() == VTK_INT)
    {
    int* tempPtr = (int*) input->GetScalarPointer();
    for(int i = 0; i < VolumeInfo.VolumeSize.x*VolumeInfo.VolumeSize.y*VolumeInfo.VolumeSize.z; i++)
      buffer[i] = (float)(tempPtr[i]);
    }
  else if(input->GetScalarType() == VTK_UNSIGNED_INT)
    {
    unsigned int* tempPtr = (unsigned int*) input->GetScalarPointer();
    for(int i = 0; i < VolumeInfo.VolumeSize.x*VolumeInfo.VolumeSize.y*VolumeInfo.VolumeSize.z; i++)
      buffer[i] = (float)(tempPtr[i]);
    }
  else if(input->GetScalarType() == VTK_SHORT)
    {
    short* tempPtr = (short*) input->GetScalarPointer();
    for(int i = 0; i < VolumeInfo.VolumeSize.x*VolumeInfo.VolumeSize.y*VolumeInfo.VolumeSize.z; i++)
      buffer[i] = (float)(tempPtr[i]);
    }
  else if(input->GetScalarType() == VTK_UNSIGNED_SHORT)
    {
    unsigned short* tempPtr = (unsigned short*) input->GetScalarPointer();
    for(int i = 0; i < VolumeInfo.VolumeSize.x*VolumeInfo.VolumeSize.y*VolumeInfo.VolumeSize.z; i++)
      buffer[i] = (float)(tempPtr[i]);
    }
  else if(input->GetScalarType() == VTK_LONG)
    {
    long* tempPtr = (long*) input->GetScalarPointer();
    for(int i = 0; i < VolumeInfo.VolumeSize.x*VolumeInfo.VolumeSize.y*VolumeInfo.VolumeSize.z; i++)
      buffer[i] = (float)(tempPtr[i]);
    }
  else if(input->GetScalarType() == VTK_UNSIGNED_LONG)
    {
    unsigned long* tempPtr = (unsigned long*) input->GetScalarPointer();
    for(int i = 0; i < VolumeInfo.VolumeSize.x*VolumeInfo.VolumeSize.y*VolumeInfo.VolumeSize.z; i++)
      buffer[i] = (float)(tempPtr[i]);
    }
  else if(input->GetScalarType() == VTK_FLOAT)
    {
    delete buffer;
    buffer = (float*) input->GetScalarPointer();
    }
  else if(input->GetScalarType() == VTK_DOUBLE)
    {
    double* tempPtr = (double*) input->GetScalarPointer();
    for(int i = 0; i < VolumeInfo.VolumeSize.x*VolumeInfo.VolumeSize.y*VolumeInfo.VolumeSize.z; i++)
      buffer[i] = (float)(tempPtr[i]);
    }
  else
    {
    vtkErrorMacro(<<"Input cannot be of that type.");
    return;
    }

  //load data onto the GPU and clean up the CPU
  if(!this->erroredOut)
    {
    this->ReserveGPU();
    this->erroredOut = !CUDA_vtkCUDA1DVolumeMapper_renderAlgo_loadImageInfo( buffer, VolumeInfoHandler->GetVolumeInfo(), this->GetStream());
    }
  if(input->GetScalarType() != VTK_FLOAT) delete buffer;

  //inform transfer function handler of the data
  this->transferFunctionInfoHandler->SetInputData(input,index);
  }

void vtkCUDA1DVolumeMapper::ChangeFrameInternal(unsigned int frame){
  if(!this->erroredOut)
    {
    this->ReserveGPU();
    this->erroredOut = !CUDA_vtkCUDA1DVolumeMapper_renderAlgo_changeFrame(frame, this->GetStream());
    }
  }

void vtkCUDA1DVolumeMapper::InternalRender (  vtkRenderer* ren, vtkVolume* vol,
                                            const cudaRendererInformation& rendererInfo,
                                            const cudaVolumeInformation& volumeInfo,
                                            const cudaOutputImageInformation& outputInfo ){
                                              //handle the transfer function changes
                                              this->transferFunctionInfoHandler->SetColourTransferFunction( vol->GetProperty()->GetRGBTransferFunction() );
                                              this->transferFunctionInfoHandler->SetOpacityTransferFunction( vol->GetProperty()->GetScalarOpacity() );
                                              this->transferFunctionInfoHandler->SetGradientOpacityTransferFunction( vol->GetProperty()->GetGradientOpacity() );
                                              this->transferFunctionInfoHandler->UseGradientOpacity( !vol->GetProperty()->GetDisableGradientOpacity() );
                                              this->transferFunctionInfoHandler->Update();

                                              //perform the render
                                              this->tfLock->Lock();
                                              this->ReserveGPU();
                                              this->erroredOut = !CUDA_vtkCUDA1DVolumeMapper_renderAlgo_doRender(outputInfo, rendererInfo, volumeInfo,
                                                this->transferFunctionInfoHandler->GetTransferFunctionInfo(), this->GetStream());
                                              this->tfLock->Unlock();

  }

void vtkCUDA1DVolumeMapper::ClearInputInternal()
  {
  this->ReserveGPU();

  CUDA_vtkCUDA1DVolumeMapper_renderAlgo_clearImageArray(this->GetStream());
  }