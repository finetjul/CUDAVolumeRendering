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

#ifndef __vtkMRMLCUDAVolumeRenderingDisplayableManager_h
#define __vtkMRMLCUDAVolumeRenderingDisplayableManager_h

// CUDA Volume Rendering includes
#include "vtkSlicerCUDAVolumeRenderingModuleLogicExport.h"
class vtkCUDAVolumeMapper;
class vtkMRMLCUDARayCastVolumeRenderingDisplayNode;

// Slicer includes
#include <vtkMRMLVolumeRenderingDisplayableManager.h>

/// \ingroup Slicer_QtModules_VolumeRendering
class VTK_SLICER_CUDAVOLUMERENDERING_MODULE_LOGIC_EXPORT vtkMRMLCUDAVolumeRenderingDisplayableManager
  : public vtkMRMLVolumeRenderingDisplayableManager
{
public:
  static vtkMRMLCUDAVolumeRenderingDisplayableManager *New();
  vtkTypeRevisionMacro(vtkMRMLCUDAVolumeRenderingDisplayableManager, vtkMRMLVolumeRenderingDisplayableManager);
  virtual void PrintSelf(ostream& os, vtkIndent indent);

  virtual void Reset();
  virtual void SetGUICallbackCommand(vtkCommand* callback);
  virtual vtkVolumeMapper* GetVolumeMapper(vtkMRMLVolumeRenderingDisplayNode* vspNode);
  using vtkMRMLVolumeRenderingDisplayableManager::UpdateMapper;
  virtual bool UpdateMapper(vtkMRMLVolumeRenderingDisplayNode* vspNode);
  virtual void UpdateCUDARaycastMapper(vtkCUDAVolumeMapper* mapper,
                                       vtkMRMLCUDARayCastVolumeRenderingDisplayNode* vspNode);
  virtual int IsMapperSupported(vtkVolumeMapper* volumeMapper, vtkMRMLVolumeRenderingDisplayNode* vspNode);

protected:
  vtkMRMLCUDAVolumeRenderingDisplayableManager();
  ~vtkMRMLCUDAVolumeRenderingDisplayableManager();
  vtkMRMLCUDAVolumeRenderingDisplayableManager(const vtkMRMLCUDAVolumeRenderingDisplayableManager&);
  void operator=(const vtkMRMLCUDAVolumeRenderingDisplayableManager&);

  virtual int GetMaxMemory(vtkVolumeMapper* mapper, vtkMRMLVolumeRenderingDisplayNode* vspNode);

  vtkCUDAVolumeMapper *CUDARaycastMapper;
};

#endif
