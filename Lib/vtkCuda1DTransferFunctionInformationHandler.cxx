/** @file vtkCUDA1DTransferFunctionInformationHandler.cxx
*
*  @brief Implementation of an internal class for vtkCUDAVolumeMapper which manages information regarding the volume and transfer function
*
*  @author John Stuart Haberl Baxter (Dr. Peter's Lab at Robarts Research Institute)
*  @note First documented on May 11, 2012
*
*/

#include "vtkCUDA1DTransferFunctionInformationHandler.h"
#include "vtkObjectFactory.h"
#include "vtkMatrix4x4.h"

//Volume and Property
#include "vtkPiecewiseFunction.h"
#include "vtkColorTransferFunction.h"
#include "vtkImageData.h"

#include "CUDA_vtkCUDA1DVolumeMapper_renderAlgo.h"

vtkStandardNewMacro(vtkCUDA1DTransferFunctionInformationHandler);

vtkCUDA1DTransferFunctionInformationHandler
::vtkCUDA1DTransferFunctionInformationHandler()
{
  this->colourFunction = NULL;
  this->opacityFunction = NULL;
  this->gradientopacityFunction = NULL;
  this->useGradientOpacity = false;

  this->FunctionSize = 512;
  this->lastModifiedTime = 0;

  this->TransInfo.alphaTransferArray1D = 0;
  this->TransInfo.galphaTransferArray1D = 0;
  this->TransInfo.colorRTransferArray1D = 0;
  this->TransInfo.colorGTransferArray1D = 0;
  this->TransInfo.colorBTransferArray1D = 0;

  this->InputData = NULL;
  this->Reinitialize();
}

vtkCUDA1DTransferFunctionInformationHandler
::~vtkCUDA1DTransferFunctionInformationHandler()
{
  this->Deinitialize();
  this->SetInputData(NULL, 0);
}

void vtkCUDA1DTransferFunctionInformationHandler
::Deinitialize(int vtkNotUsed(withData))
{
  this->ReserveGPU();
  CUDA_vtkCUDA1DVolumeMapper_renderAlgo_UnloadTextures( this->TransInfo, this->GetStream() );
}

void vtkCUDA1DTransferFunctionInformationHandler
::Reinitialize(int vtkNotUsed(withData))
{
  lastModifiedTime = 0;
  UpdateTransferFunction();
}

void vtkCUDA1DTransferFunctionInformationHandler
::SetInputData(vtkImageData* inputData, int vtkNotUsed(index))
{
  if( inputData == this->InputData )
    {
    return;
    }

  if (inputData == NULL)
    {
    this->InputData = NULL;
    }
  else
    {
    this->InputData = inputData;
    this->Modified();
    }
}

void vtkCUDA1DTransferFunctionInformationHandler
::SetColourTransferFunction(vtkColorTransferFunction* f)
{
  if( f != this->colourFunction )
    {
    this->colourFunction = f;
    this->lastModifiedTime = 0;
    this->Modified();
    }
}

void vtkCUDA1DTransferFunctionInformationHandler
::SetOpacityTransferFunction(vtkPiecewiseFunction* f)
{
  if( f != this->opacityFunction )
    {
    this->opacityFunction = f;
    this->lastModifiedTime = 0;
    this->Modified();
    }
}

void vtkCUDA1DTransferFunctionInformationHandler
::SetGradientOpacityTransferFunction(vtkPiecewiseFunction* f)
{
  if( f != this->gradientopacityFunction )
    {
    this->gradientopacityFunction = f;
    this->lastModifiedTime = 0;
    this->Modified();
    }
}

void vtkCUDA1DTransferFunctionInformationHandler::UpdateTransferFunction()
{
  //if we don't need to update the transfer function, don't
  if(!this->colourFunction || !this->opacityFunction ||
    (this->colourFunction->GetMTime() <= lastModifiedTime &&
    this->opacityFunction->GetMTime() <= lastModifiedTime) )
    {
    return;
    }
  lastModifiedTime = (this->colourFunction->GetMTime() > this->opacityFunction->GetMTime()) ?
    this->colourFunction->GetMTime() : this->opacityFunction->GetMTime();

  //get the ranges from the transfer function
  double minIntensity; 
  double maxIntensity;
  this->opacityFunction->GetRange( minIntensity, maxIntensity );
  minIntensity = (this->InputData->GetScalarRange()[0] > minIntensity ) ? this->InputData->GetScalarRange()[0] : minIntensity;
  maxIntensity = (this->InputData->GetScalarRange()[1] < maxIntensity ) ? this->InputData->GetScalarRange()[1] : maxIntensity;

  //get the gradient ranges from the transfer function
  double minGradient; 
  double maxGradient;
  this->gradientopacityFunction->GetRange( minGradient, maxGradient );

  //figure out the multipliers for applying the transfer function in GPU
  this->TransInfo.intensityLow = minIntensity;
  this->TransInfo.intensityMultiplier = 1.0 / ( maxIntensity - minIntensity );
  this->TransInfo.gradientLow = minGradient;
  this->TransInfo.gradientMultiplier = 1.0 / ( maxGradient - minGradient );

  //create local buffers to house the transfer function
  float* LocalColorRedTransferFunction = new float[this->FunctionSize];
  float* LocalColorGreenTransferFunction = new float[this->FunctionSize];
  float* LocalColorBlueTransferFunction = new float[this->FunctionSize];
  float* LocalColorWholeTransferFunction = new float[3*this->FunctionSize];
  float* LocalAlphaTransferFunction = new float[this->FunctionSize];
  float* LocalGAlphaTransferFunction = new float[this->FunctionSize];

  memset( (void*) LocalColorRedTransferFunction, 0.0f, sizeof(float) * this->FunctionSize);
  memset( (void*) LocalColorGreenTransferFunction, 0.0f, sizeof(float) * this->FunctionSize);
  memset( (void*) LocalColorBlueTransferFunction, 0.0f, sizeof(float) * this->FunctionSize);
  memset( (void*) LocalColorWholeTransferFunction, 0.0f, 3*sizeof(float) * this->FunctionSize);
  memset( (void*) LocalAlphaTransferFunction, 1.0f, sizeof(float) * this->FunctionSize);
  memset( (void*) LocalGAlphaTransferFunction, 1.0f, sizeof(float) * this->FunctionSize);

  //populate the table
  this->opacityFunction->GetTable( minIntensity, maxIntensity, this->FunctionSize,
    LocalAlphaTransferFunction );
  this->gradientopacityFunction->GetTable( minGradient, maxGradient, this->FunctionSize,
    LocalGAlphaTransferFunction );
  this->colourFunction->GetTable( minIntensity, maxIntensity, this->FunctionSize,
    LocalColorWholeTransferFunction );
  for( int i = 0; i < this->FunctionSize; i++ )
    {
    LocalColorRedTransferFunction[i] = LocalColorWholeTransferFunction[3*i];
    LocalColorGreenTransferFunction[i] = LocalColorWholeTransferFunction[3*i+1];
    LocalColorBlueTransferFunction[i] = LocalColorWholeTransferFunction[3*i+2];
    }

  //map the trasfer functions to textures for fast access
  this->TransInfo.functionSize = this->FunctionSize;

  this->ReserveGPU();
  CUDA_vtkCUDA1DVolumeMapper_renderAlgo_loadTextures(this->TransInfo,
    LocalColorRedTransferFunction,
    LocalColorGreenTransferFunction,
    LocalColorBlueTransferFunction,
    LocalAlphaTransferFunction,
    LocalGAlphaTransferFunction,
    this->GetStream() );

  //clean up the garbage
  delete LocalColorRedTransferFunction;
  delete LocalColorGreenTransferFunction;
  delete LocalColorBlueTransferFunction;
  delete LocalColorWholeTransferFunction;
  delete LocalAlphaTransferFunction;
  delete LocalGAlphaTransferFunction;
}

void vtkCUDA1DTransferFunctionInformationHandler::UseGradientOpacity(int u)
{
  this->useGradientOpacity = (u != 0);
}

void vtkCUDA1DTransferFunctionInformationHandler::Update()
{
  if(this->InputData)
    {
    this->InputData->Update();
    this->Modified();
    }
  if(this->colourFunction && this->opacityFunction)
    {
    this->UpdateTransferFunction();
    this->Modified();
    }
}
