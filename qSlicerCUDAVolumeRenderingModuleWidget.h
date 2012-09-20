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

#ifndef __qSlicerCUDAVolumeRenderingModuleWidget_h
#define __qSlicerCUDAVolumeRenderingModuleWidget_h

// SlicerQt includes
#include "qSlicerAbstractModuleWidget.h"

#include "qSlicerCUDAVolumeRenderingModuleExport.h"

class qSlicerCUDAVolumeRenderingModuleWidgetPrivate;
class vtkMRMLNode;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_CUDAVOLUMERENDERING_EXPORT qSlicerCUDAVolumeRenderingModuleWidget :
  public qSlicerAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qSlicerAbstractModuleWidget Superclass;
  qSlicerCUDAVolumeRenderingModuleWidget(QWidget *parent=0);
  virtual ~qSlicerCUDAVolumeRenderingModuleWidget();

public slots:


protected:
  QScopedPointer<qSlicerCUDAVolumeRenderingModuleWidgetPrivate> d_ptr;
  
  virtual void setup();

private:
  Q_DECLARE_PRIVATE(qSlicerCUDAVolumeRenderingModuleWidget);
  Q_DISABLE_COPY(qSlicerCUDAVolumeRenderingModuleWidget);
};

#endif
