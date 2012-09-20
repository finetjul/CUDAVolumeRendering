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

// SlicerQt includes
#include "qSlicerCUDAVolumeRenderingModuleWidget.h"
#include "ui_qSlicerCUDAVolumeRenderingModule.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerCUDAVolumeRenderingModuleWidgetPrivate: public Ui_qSlicerCUDAVolumeRenderingModule
{
public:
  qSlicerCUDAVolumeRenderingModuleWidgetPrivate();
};

//-----------------------------------------------------------------------------
// qSlicerCUDAVolumeRenderingModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerCUDAVolumeRenderingModuleWidgetPrivate::qSlicerCUDAVolumeRenderingModuleWidgetPrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerCUDAVolumeRenderingModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerCUDAVolumeRenderingModuleWidget::qSlicerCUDAVolumeRenderingModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerCUDAVolumeRenderingModuleWidgetPrivate )
{
}

//-----------------------------------------------------------------------------
qSlicerCUDAVolumeRenderingModuleWidget::~qSlicerCUDAVolumeRenderingModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerCUDAVolumeRenderingModuleWidget::setup()
{
  Q_D(qSlicerCUDAVolumeRenderingModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();
}

