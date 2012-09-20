/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// .NAME vtkSlicerCUDAVolumeRenderingLogic - slicer logic class for volumes manipulation
// .SECTION Description
// This class manages the logic associated with reading, saving,
// and changing propertied of the volumes


#ifndef __vtkSlicerCUDAVolumeRenderingLogic_h
#define __vtkSlicerCUDAVolumeRenderingLogic_h

// CUDA Volume Rendering includes
#include "vtkSlicerCUDAVolumeRenderingModuleLogicExport.h"

// Slicer includes
#include "vtkSlicerModuleLogic.h"

// STD includes
#include <cstdlib>

/// \ingroup Slicer_QtModules_ExtensionTemplate
class VTK_SLICER_CUDAVOLUMERENDERING_MODULE_LOGIC_EXPORT vtkSlicerCUDAVolumeRenderingLogic
  : public vtkSlicerModuleLogic
{
public:

  static vtkSlicerCUDAVolumeRenderingLogic *New();
  vtkTypeMacro(vtkSlicerCUDAVolumeRenderingLogic, vtkSlicerModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent);

protected:
  vtkSlicerCUDAVolumeRenderingLogic();
  virtual ~vtkSlicerCUDAVolumeRenderingLogic();

  /// Register MRML Node classes to Scene. Gets called automatically when the MRMLScene is attached to this logic class.
  virtual void RegisterNodes();

private:
  vtkSlicerCUDAVolumeRenderingLogic(const vtkSlicerCUDAVolumeRenderingLogic&); // Not implemented
  void operator=(const vtkSlicerCUDAVolumeRenderingLogic&);               // Not implemented
};

#endif
