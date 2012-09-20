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

// Qt includes
#include <QDebug>
#include <QtPlugin>

// Slicer includes
#include <qSlicerCoreApplication.h>
#include <qSlicerModuleManager.h>

// ExtensionTemplate Logic includes
#include "vtkMRMLCUDARayCastVolumeRenderingDisplayNode.h"
#include "vtkSlicerCUDAVolumeRenderingLogic.h"

// ExtensionTemplate includes
#include "qSlicerCUDAVolumeRenderingModule.h"
//#include "qSlicerCUDAVolumeRenderingModuleWidget.h"

// Volume Rendering includes
#include "vtkSlicerVolumeRenderingLogic.h"

// MRML includes
#include <vtkMRMLThreeDViewDisplayableManagerFactory.h>

// VTK includes
#include <vtkNew.h>

//-----------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(qSlicerCUDAVolumeRenderingModule, qSlicerCUDAVolumeRenderingModule);

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerCUDAVolumeRenderingModulePrivate
{
public:
  qSlicerCUDAVolumeRenderingModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerCUDAVolumeRenderingModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerCUDAVolumeRenderingModulePrivate::qSlicerCUDAVolumeRenderingModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerCUDAVolumeRenderingModule methods

//-----------------------------------------------------------------------------
qSlicerCUDAVolumeRenderingModule::qSlicerCUDAVolumeRenderingModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerCUDAVolumeRenderingModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerCUDAVolumeRenderingModule::~qSlicerCUDAVolumeRenderingModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerCUDAVolumeRenderingModule::helpText()const
{
  return "This CUDAVolumeRendering module adds a CUDA volume rendering method";
}

//-----------------------------------------------------------------------------
QString qSlicerCUDAVolumeRenderingModule::acknowledgementText()const
{
  return "This work was was partially funded by NIH grant 3P41RR013218-12S1";
}

//-----------------------------------------------------------------------------
QStringList qSlicerCUDAVolumeRenderingModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Julien Finet (Kitware)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerCUDAVolumeRenderingModule::icon()const
{
  return QIcon(":/Icons/CUDAVolumeRendering.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerCUDAVolumeRenderingModule::categories() const
{
  return QStringList() << "Volume Rendering";
}

//-----------------------------------------------------------------------------
QStringList qSlicerCUDAVolumeRenderingModule::dependencies() const
{
  return QStringList() << "VolumeRendering";
}

//-----------------------------------------------------------------------------
bool qSlicerCUDAVolumeRenderingModule::isHidden()const
{
  return true;
}

//-----------------------------------------------------------------------------
void qSlicerCUDAVolumeRenderingModule::setup()
{
  vtkMRMLThreeDViewDisplayableManagerFactory::GetInstance()->
    RegisterDisplayableManager("vtkMRMLCUDAVolumeRenderingDisplayableManager");

  this->Superclass::setup();

  qSlicerAbstractCoreModule* volumeRenderingModule =
    qSlicerCoreApplication::application()->moduleManager()->module("VolumeRendering");
  if (volumeRenderingModule)
    {
    vtkNew<vtkMRMLCUDARayCastVolumeRenderingDisplayNode> displayNode;
    vtkSlicerVolumeRenderingLogic* volumeRenderingLogic =
      vtkSlicerVolumeRenderingLogic::SafeDownCast(volumeRenderingModule->logic());
    volumeRenderingLogic->RegisterRenderingMethod(
      "CUDA Volume Rendering", displayNode->GetClassName());
    }
  else
    {
    qWarning() << "Volume Rendering module is not found";
    }
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation * qSlicerCUDAVolumeRenderingModule::createWidgetRepresentation()
{
  return 0; //return new qSlicerCUDAVolumeRenderingModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerCUDAVolumeRenderingModule::createLogic()
{
  return vtkSlicerCUDAVolumeRenderingLogic::New();
}
