/** @file vtkCUDADeviceManager.h
*
*  @brief Header file defining a singleton class to manage cards and stream interleaving
*
*  @author John Stuart Haberl Baxter (Dr. Peter's Lab at Robarts Research Institute)
*  @note First documented on June 12, 2012
*
*/

#ifndef __vtkCUDADeviceManager_h
#define __vtkCUDADeviceManager_h

// CUDA Volume Rendering includes
#include "CUDAVolumeRenderingLibExport.h"
#include "vector_types.h"
class vtkCUDAObject;

// VTK includes
#include "vtkObject.h"
class vtkMutexLock;

// STD includes
#include <map>

class CUDA_LIB_EXPORT vtkCUDADeviceManager
  : public vtkObject
{
public:
  static vtkCUDADeviceManager* Singleton();

  vtkTypeMacro (vtkCUDADeviceManager,vtkObject);

  int GetNumberOfDevices();
  bool GetDevice(vtkCUDAObject* caller, int device);
  bool ReturnDevice(vtkCUDAObject* caller, int device);
  bool GetStream(vtkCUDAObject* caller, cudaStream_t** stream, int device);
  bool ReturnStream(vtkCUDAObject* caller, cudaStream_t* stream, int device);

  bool SynchronizeStream( cudaStream_t* stream );
  bool ReserveGPU( cudaStream_t* stream );

  int QueryDeviceForObject( vtkCUDAObject* object );
  int QueryDeviceForStream( cudaStream_t* stream );

protected:

private:
  vtkCUDADeviceManager();
  ~vtkCUDADeviceManager();
  vtkCUDADeviceManager operator=(const vtkCUDADeviceManager&); /**< not implemented */
  vtkCUDADeviceManager(const vtkCUDADeviceManager&); /**< not implemented */

  void DestroyEmptyStream( cudaStream_t* stream );

  std::map<cudaStream_t*,int> StreamToDeviceMap;
  std::multimap<vtkCUDAObject*,int> ObjectToDeviceMap;
  std::multimap<cudaStream_t*, vtkCUDAObject*> StreamToObjectMap;

  static vtkCUDADeviceManager* singletonManager;

  vtkMutexLock* regularLock;

};

#endif
