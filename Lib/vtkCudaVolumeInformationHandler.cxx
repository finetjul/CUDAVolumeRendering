/** @file vtkCUDAVolumeInformationHandler.cxx
*
*  @brief An internal class for vtkCUDAVolumeMapper which manages information regarding the volume and transfer function
*
*  @author John Stuart Haberl Baxter (Dr. Peter's Lab at Robarts Research Institute)
*  @note First documented on March 28, 2011
*
*/

#include "vtkCUDAVolumeInformationHandler.h"

// VTK includes
#include <vtkImageData.h>
#include <vtkMatrix4x4.h>
#include <vtkObjectFactory.h>
#include <vtkPlaneCollection.h>
#include <vtkRenderer.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>

vtkStandardNewMacro(vtkCUDAVolumeInformationHandler);

vtkCUDAVolumeInformationHandler::vtkCUDAVolumeInformationHandler()
  {
  this->lastModifiedTime = 0;
  this->Volume = NULL;
  this->InputData = NULL;
  }

vtkCUDAVolumeInformationHandler::~vtkCUDAVolumeInformationHandler()
  {
  this->SetVolume(NULL);
  this->SetInputData(NULL, 0);
  }

void vtkCUDAVolumeInformationHandler::Deinitialize(int withData)
  {
  //TODO
  }

void vtkCUDAVolumeInformationHandler::Reinitialize(int withData)
  {
  //TODO
  }

vtkVolume* vtkCUDAVolumeInformationHandler::GetVolume()
  {
  return this->Volume;
  }

void vtkCUDAVolumeInformationHandler::SetVolume(vtkVolume* volume)
  {
  if( this->Volume == volume ) return;
  this->Volume = volume;
  if (Volume != NULL)
    this->Update();
  }

void vtkCUDAVolumeInformationHandler::SetInputData(vtkImageData* inputData, int index)
  {
  if (inputData == NULL)
    {
    this->InputData = NULL;
    }
  else if (inputData != this->InputData)
    {
    this->InputData = inputData;
    this->UpdateImageData(index);
    this->Modified();
    }
  }

void vtkCUDAVolumeInformationHandler::UpdateImageData(int index)
  {
  this->InputData->Update();

  int* dims = this->InputData->GetDimensions();
  double* spacing = this->InputData->GetSpacing();

  this->VolumeInfo.VolumeSize.x = dims[0];
  this->VolumeInfo.VolumeSize.y = dims[1];
  this->VolumeInfo.VolumeSize.z = dims[2];

  this->VolumeInfo.SpacingReciprocal.x = 1.0f / spacing[0];
  this->VolumeInfo.SpacingReciprocal.y = 1.0f / spacing[1];
  this->VolumeInfo.SpacingReciprocal.z = 1.0f / spacing[2];
  this->VolumeInfo.Spacing.x = spacing[0];
  this->VolumeInfo.Spacing.y = spacing[1];
  this->VolumeInfo.Spacing.z = spacing[2];
  this->VolumeInfo.MinSpacing = spacing[0];
  this->VolumeInfo.MinSpacing = (this->VolumeInfo.MinSpacing > spacing[1]) ? spacing[1] : this->VolumeInfo.MinSpacing;
  this->VolumeInfo.MinSpacing = (this->VolumeInfo.MinSpacing > spacing[2]) ? spacing[2] : this->VolumeInfo.MinSpacing;

  //calculate the bounds
  this->VolumeInfo.Bounds[0] = 0.0f;
  this->VolumeInfo.Bounds[1] = (float) dims[0] - 1.0f;
  this->VolumeInfo.Bounds[2] = 0.0f;
  this->VolumeInfo.Bounds[3] = (float) dims[1] - 1.0f;
  this->VolumeInfo.Bounds[4] = 0.0f;
  this->VolumeInfo.Bounds[5] = (float) dims[2] - 1.0f;

  }

void vtkCUDAVolumeInformationHandler::Update()
  {

  //get shading params
  this->VolumeInfo.Ambient = 1.0f;
  this->VolumeInfo.Diffuse = 0.0f;
  this->VolumeInfo.Specular.x = 0.0f;
  this->VolumeInfo.Specular.y = 1.0f;
  if( this->Volume && this->Volume->GetProperty() ){
    int shadingType = this->Volume->GetProperty()->GetShade();
    if(shadingType)
      {
      this->VolumeInfo.Ambient = this->Volume->GetProperty()->GetAmbient();
      this->VolumeInfo.Diffuse = this->Volume->GetProperty()->GetDiffuse();
      this->VolumeInfo.Specular.x = this->Volume->GetProperty()->GetSpecular();
      this->VolumeInfo.Specular.y = this->Volume->GetProperty()->GetSpecularPower();
      }
    }

  if(this->InputData)
    {
    this->InputData->Update();
    this->Modified();
    }
  }

void vtkCUDAVolumeInformationHandler::ClearInput()
  {
  this->Modified();
  this->Volume = NULL;
  this->InputData = NULL;
  }
