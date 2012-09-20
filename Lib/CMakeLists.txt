set(KIT CUDAVolumeRenderingLib)

project(${KIT})

set(KIT_LIBS
  ${KIT_LIBS}
  vtkVolumeRendering
  vtkRendering
  vtkFiltering
  vtkImaging
  vtkWidgets
  vtkIO
  vtkCommon
  ${CUDA_CUDA_LIBRARY}
  ${CUDA_CUDART_LIBRARY}
  )

set(KIT_EXTRA_LIBS)

set(KIT_SRCS
  #main.cxx
  vtkCUDAObject.h vtkCUDAObject.cxx
  vtkCUDADeviceManager.h vtkCUDADeviceManager.cxx
  vtkCUDAVolumeMapper.h vtkCUDAVolumeMapper.cxx
  vtkCUDARendererInformationHandler.h vtkCUDARendererInformationHandler.cxx
  vtkCUDAVolumeInformationHandler.h vtkCUDAVolumeInformationHandler.cxx
  vtkCUDAOutputImageInformationHandler.h vtkCUDAOutputImageInformationHandler.cxx
  CUDA_containerRendererInformation.h
  CUDA_containerVolumeInformation.h
  CUDA_containerOutputImageInformation.h
  CUDA_vtkCUDAVolumeMapper_renderAlgo.h CUDA_vtkCUDAVolumeMapper_renderAlgo.cu
  vtkCUDA1DVolumeMapper.h vtkCUDA1DVolumeMapper.cxx
  vtkCUDA1DTransferFunctionInformationHandler.h vtkCUDA1DTransferFunctionInformationHandler.cxx
  CUDA_container1DTransferFunctionInformation.h
  CUDA_vtkCUDA1DVolumeMapper_renderAlgo.h CUDA_vtkCUDA1DVolumeMapper_renderAlgo.cuh
  )

set(Kit_RegularMapper_SRCS
  #main_regularCaster.cxx
  )

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${vtkCUDATesting_INCLUDE_DIRS}
  )

set(MY_EXPORT_HEADER_PREFIX ${KIT})
set(MY_LIBNAME ${KIT})
set(MY_LIBRARY_EXPORT_DIRECTIVE CUDA_LIB_EXPORT)

configure_file(
  ${Slicer_EXPORT_HEADER_TEMPLATE}
  ${CMAKE_CURRENT_BINARY_DIR}/${MY_EXPORT_HEADER_PREFIX}Export.h
  )
cuda_add_library( ${KIT} ${KIT_SRCS} )
target_link_libraries( ${KIT} ${KIT_LIBS} )

install(TARGETS ${KIT}
  RUNTIME DESTINATION ${Slicer_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
  LIBRARY DESTINATION ${Slicer_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
  ARCHIVE DESTINATION ${Slicer_INSTALL_LIB_DIR} COMPONENT Development
  )
