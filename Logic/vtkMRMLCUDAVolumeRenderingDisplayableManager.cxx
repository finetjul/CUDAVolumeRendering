/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Julien Finet, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// CUDA Volume Rendering includes
#include "vtkCUDA1DVolumeMapper.h"
#include "vtkMRMLCUDAVolumeRenderingDisplayableManager.h"
#include "vtkMRMLCUDARayCastVolumeRenderingDisplayNode.h"

// MRML includes
#include "vtkMRMLScalarVolumeDisplayNode.h"
#include "vtkMRMLScalarVolumeNode.h"
#include "vtkMRMLVolumePropertyNode.h"

// VTK includes
#include <vtkCallbackCommand.h>
#include <vtkImageData.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

// STD includes
#include <cassert>

//---------------------------------------------------------------------------
vtkStandardNewMacro (vtkMRMLCUDAVolumeRenderingDisplayableManager);
vtkCxxRevisionMacro (vtkMRMLCUDAVolumeRenderingDisplayableManager, "$Revision: 1.0 $");

//---------------------------------------------------------------------------
vtkMRMLCUDAVolumeRenderingDisplayableManager::vtkMRMLCUDAVolumeRenderingDisplayableManager()
{
  this->CUDARaycastMapper = NULL;
  // Initialize the raycasters in Reset
  this->Reset();
}

//---------------------------------------------------------------------------
vtkMRMLCUDAVolumeRenderingDisplayableManager::~vtkMRMLCUDAVolumeRenderingDisplayableManager()
{
  this->RemoveDisplayNodes();

  //delete instances
  vtkSetMRMLNodeMacro(this->CUDARaycastMapper, NULL);
}

//---------------------------------------------------------------------------
void vtkMRMLCUDAVolumeRenderingDisplayableManager::PrintSelf(std::ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//---------------------------------------------------------------------------
void vtkMRMLCUDAVolumeRenderingDisplayableManager::SetGUICallbackCommand(vtkCommand* callback)
{
  //cpu ray casting
  this->CUDARaycastMapper->AddObserver(vtkCommand::VolumeMapperComputeGradientsProgressEvent, callback);
  this->CUDARaycastMapper->AddObserver(vtkCommand::ProgressEvent,callback);
}

//---------------------------------------------------------------------------
void vtkMRMLCUDAVolumeRenderingDisplayableManager::Reset()
{
  vtkNew<vtkIntArray> mapperEvents;
  vtkNew<vtkIntArray> mapperEventsWithProgress;

  // CUDA raycast mapper
  vtkNew<vtkCUDA1DVolumeMapper> newRaycastMapper;
  vtkSetAndObserveMRMLNodeEventsMacro(this->CUDARaycastMapper,
                                      newRaycastMapper.GetPointer(),
                                      mapperEventsWithProgress.GetPointer());
}

//---------------------------------------------------------------------------
int vtkMRMLCUDAVolumeRenderingDisplayableManager
::GetMaxMemory(vtkVolumeMapper* volumeMapper,
               vtkMRMLVolumeRenderingDisplayNode* vspNode)
{
  return this->Superclass::GetMaxMemory(volumeMapper, vspNode);
}

//---------------------------------------------------------------------------
void vtkMRMLCUDAVolumeRenderingDisplayableManager
::UpdateCUDARaycastMapper(
  vtkCUDAVolumeMapper* mapper,
  vtkMRMLCUDARayCastVolumeRenderingDisplayNode* vspNode)
{
  this->UpdateMapper(mapper, vspNode);
}

//---------------------------------------------------------------------------
int vtkMRMLCUDAVolumeRenderingDisplayableManager
::IsMapperSupported(vtkVolumeMapper* volumeMapper, vtkMRMLVolumeRenderingDisplayNode* vspNode)
{
  if (volumeMapper == 0)
    {
    return 0;
    }
  vtkRenderWindow* window = this->GetRenderer()->GetRenderWindow();

  volumeMapper->SetInput(vtkMRMLScalarVolumeNode::SafeDownCast(
                           vspNode->GetVolumeNode())->GetImageData() );

  int supported = 0;
  if (volumeMapper->IsA("vtkCUDAVolumeMapper"))
    {
    supported = vtkCUDAVolumeMapper::SafeDownCast(volumeMapper)->IsRenderSupported(
      window, vspNode->GetVolumePropertyNode()->GetVolumeProperty());
    }
  if (supported == 0)
    {
    vtkErrorMacro("CUDA volume mapper is not supported.");
    }
  return supported;
}

//---------------------------------------------------------------------------
vtkVolumeMapper* vtkMRMLCUDAVolumeRenderingDisplayableManager
::GetVolumeMapper(vtkMRMLVolumeRenderingDisplayNode* vspNode)
{
  if (!vspNode)
    {
    return 0;
    }
  if (vspNode->IsA("vtkMRMLCUDARayCastVolumeRenderingDisplayNode"))
    {
    return this->CUDARaycastMapper;
    }
  return 0;
}

//---------------------------------------------------------------------------
bool vtkMRMLCUDAVolumeRenderingDisplayableManager::UpdateMapper(
  vtkMRMLVolumeRenderingDisplayNode* vspNode)
{
  vtkVolumeMapper* volumeMapper = this->GetVolumeMapper(vspNode);
  if (vspNode->IsA("vtkMRMLCUDARayCastVolumeRenderingDisplayNode"))
    {
    this->UpdateCUDARaycastMapper(vtkCUDAVolumeMapper::SafeDownCast(volumeMapper),
                                  vtkMRMLCUDARayCastVolumeRenderingDisplayNode::SafeDownCast(vspNode));
    }
  else
    {
    return false;
    }
  return true;
}
