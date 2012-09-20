project(vtkSlicer${MODULE_NAME}ModuleLogic)

set(KIT ${PROJECT_NAME})
set(${KIT}_EXPORT_DIRECTIVE "VTK_SLICER_${MODULE_NAME_UPPER}_MODULE_LOGIC_EXPORT")

set(${KIT}_INCLUDE_DIRECTORIES
  ${CUDAVolumeRenderingLib_BINARY_DIR}
  ${CUDAVolumeRenderingLib_SOURCE_DIR}
  ${vtkSlicer${MODULE_NAME}ModuleMRML_SOURCE_DIR}}
  ${vtkSlicer${MODULE_NAME}ModuleMRML_BINARY_DIR}}
  )

set(displayable_manager_SRCS
  vtkMRMLCUDAVolumeRenderingDisplayableManager.cxx
  vtkMRMLCUDAVolumeRenderingDisplayableManager.h
  )

set(VTK_USE_INSTANTIATOR_NEW 1)
VTK_MAKE_INSTANTIATOR3("${MODULE_NAME}Instantiator"
  displayable_manager_instantiator_SRCS
  "${displayable_manager_SRCS}"
  "${${KIT}_EXPORT_DIRECTIVE}"
  ${CMAKE_CURRENT_BINARY_DIR}
  "${KIT}Export.h"
  )

set(${KIT}_SRCS
  ${displayable_manager_SRCS}
  ${displayable_manager_instantiator_SRCS}
  vtkSlicerCUDAVolumeRenderingLogic.cxx
  vtkSlicerCUDAVolumeRenderingLogic.h
  )

set(${KIT}_TARGET_LIBRARIES
  ${ITK_LIBRARIES}
  CUDAVolumeRenderingLib
  vtkSlicer${MODULE_NAME}ModuleMRML
  vtkSlicerVolumeRenderingModuleMRMLDisplayableManager
  vtkSlicerVolumeRenderingModuleMRMLDisplayableManagerPythonD
  )

#-----------------------------------------------------------------------------
SlicerMacroBuildModuleLogic(
  NAME ${KIT}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${${KIT}_INCLUDE_DIRECTORIES}
  SRCS ${${KIT}_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  )
