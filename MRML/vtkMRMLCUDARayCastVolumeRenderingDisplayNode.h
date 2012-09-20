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

#ifndef __vtkMRMLCUDARayCastVolumeRenderingDisplayNode_h
#define __vtkMRMLCUDARayCastVolumeRenderingDisplayNode_h

// CUDA Volume Rendering includes
#include "vtkSlicerCUDAVolumeRenderingModuleMRMLExport.h"

// Volume Rendering includes
#include "vtkMRMLVolumeRenderingDisplayNode.h"

/// \ingroup Slicer_QtModules_VolumeRendering
/// \name vtkMRMLGPURayCastGPURayCastVolumeRenderingDisplayNode
/// \brief MRML node for storing information for GPU Raycast Volume Rendering
class VTK_SLICER_CUDAVOLUMERENDERING_MODULE_MRML_EXPORT vtkMRMLCUDARayCastVolumeRenderingDisplayNode
  : public vtkMRMLVolumeRenderingDisplayNode
{
public:
  static vtkMRMLCUDARayCastVolumeRenderingDisplayNode *New();
  vtkTypeMacro(vtkMRMLCUDARayCastVolumeRenderingDisplayNode,vtkMRMLVolumeRenderingDisplayNode);
  virtual void PrintSelf(ostream& os, vtkIndent indent);

  virtual vtkMRMLNode* CreateNodeInstance();

  // Description:
  // Get node XML tag name (like Volume, Model)
  virtual const char* GetNodeTagName() {return "CUDARayCastVolumeRendering";};

protected:
  vtkMRMLCUDARayCastVolumeRenderingDisplayNode();
  ~vtkMRMLCUDARayCastVolumeRenderingDisplayNode();
  vtkMRMLCUDARayCastVolumeRenderingDisplayNode(const vtkMRMLCUDARayCastVolumeRenderingDisplayNode&);
  void operator=(const vtkMRMLCUDARayCastVolumeRenderingDisplayNode&);

};

#endif

