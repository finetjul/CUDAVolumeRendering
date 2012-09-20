/** @file vtkCUDAObject.h
*
*  @brief Header file defining an abstract class which uses CUDA
*
*  @author John Stuart Haberl Baxter (Dr. Peter's Lab at Robarts Research Institute)
*  @note First documented on June 12, 2012
*
*  @note Interacts primarily with the vtkCUDADeviceManager
*/

#ifndef __vtkCUDAObject_h
#define __vtkCUDAObject_h

// CUDA Volume Rendering includes
#include "cuda.h"
#include "CUDAVolumeRenderingLibExport.h"
#include "vector_types.h"
class vtkCUDADeviceManager;

class CUDA_LIB_EXPORT vtkCUDAObject
{
public:

  void SetDevice( int d, int withData = 0 );
  int GetDevice(){ return this->DeviceNumber; };

  void ReserveGPU( );
  void CallSyncThreads( );
  cudaStream_t* vtkCUDAObject::GetStream( );

  void ReplicateObject( vtkCUDAObject* object, int withData = 0  );

protected:
  vtkCUDAObject();
  ~vtkCUDAObject();

  virtual void Reinitialize(int withData = 0) = 0;
  virtual void Deinitialize(int withData = 0) = 0;

private:

  int DeviceNumber;
  cudaStream_t* DeviceStream;

  vtkCUDADeviceManager* DeviceManager;

  int withDataStatus;

};
#endif /* __VTKCUDAOBJECT_H__ */
