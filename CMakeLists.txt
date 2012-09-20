cmake_minimum_required(VERSION 2.8.7)

#-----------------------------------------------------------------------------
set(EXTENSION_NAME CUDAVolumeRendering) # Do not use 'project()'
set(EXTENSION_TITLE ${EXTENSION_NAME})

set(MODULE_NAME ${EXTENSION_NAME})
set(MODULE_TITLE ${EXTENSION_TITLE})
string(TOUPPER ${MODULE_NAME} MODULE_NAME_UPPER)

#-----------------------------------------------------------------------------
if(NOT Slicer_SOURCE_DIR)
  set(EXTENSION_HOMEPAGE "http://www.slicer.org/slicerWiki/index.php/Documentation/Snapshot/Developers/Modules/CUDAVolumeRendering")
  set(EXTENSION_CATEGORY "Volume Rendering")
  set(EXTENSION_STATUS "Beta")
  set(EXTENSION_CONTRIBUTORS "Julien Finet(Kitware)")
  set(EXTENSION_DESCRIPTION "CUDA Raycast volume rendering plugin")
  set(EXTENSION_ICONURL "")
  set(EXTENSION_SCREENSHOTURLS "")
  set(EXTENSION_DEPENDS VolumeRendering) # Specified as a space separated list or 'NA' if any
endif()

#-----------------------------------------------------------------------------
if(NOT Slicer_SOURCE_DIR)
  find_package(Slicer REQUIRED)
  include(${Slicer_USE_FILE})
endif()

#-----------------------------------------------------------------------------
FIND_PACKAGE(CUDA REQUIRED)
if (CUDA_FOUND)
  include(FindCUDA)
endif()

include_directories("C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v4.2/include")
#-----------------------------------------------------------------------------
add_subdirectory(LIB)
add_subdirectory(MRML)
add_subdirectory(Logic)

#-----------------------------------------------------------------------------
set(MODULE_EXPORT_DIRECTIVE "Q_SLICER_QTMODULES_${MODULE_NAME_UPPER}_EXPORT")

# Current_{source,binary} and Slicer_{Libs,Base} already included
set(MODULE_INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}/Logic
  ${CMAKE_CURRENT_BINARY_DIR}/Logic
  )

set(MODULE_SRCS
  qSlicerCUDAVolumeRenderingModule.cxx
  qSlicerCUDAVolumeRenderingModule.h
  #qSlicerCUDAVolumeRenderingModuleWidget.cxx
  #qSlicerCUDAVolumeRenderingModuleWidget.h
  )

set(MODULE_MOC_SRCS
  qSlicerCUDAVolumeRenderingModule.h
  #qSlicerCUDAVolumeRenderingModuleWidget.h
  )

set(MODULE_UI_SRCS
  #Resources/UI/qSlicerCUDAVolumeRenderingModule.ui
  )

set(MODULE_TARGET_LIBRARIES
  vtkSlicerCUDAVolumeRenderingModuleLogic
  )

set(MODULE_RESOURCES
  Resources/qSlicerCUDAVolumeRenderingModule.qrc
  )

#-----------------------------------------------------------------------------
slicerMacroBuildQtModule(
  NAME ${MODULE_NAME}
  TITLE ${MODULE_TITLE}
  EXPORT_DIRECTIVE ${MODULE_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${MODULE_INCLUDE_DIRECTORIES}
  SRCS ${MODULE_SRCS}
  MOC_SRCS ${MODULE_MOC_SRCS}
  UI_SRCS ${MODULE_UI_SRCS}
  TARGET_LIBRARIES ${MODULE_TARGET_LIBRARIES}
  RESOURCES ${MODULE_RESOURCES}
  )

#-----------------------------------------------------------------------------
if(BUILD_TESTING)
  add_subdirectory(Testing)
endif()

#-----------------------------------------------------------------------------
if(NOT Slicer_SOURCE_DIR)
  include(${Slicer_EXTENSION_CPACK})
endif()
