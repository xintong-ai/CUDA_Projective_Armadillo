project(CUDA_Projective_Armadillo)

cmake_minimum_required(VERSION 2.6)

#Set(CMAKE_PREFIX_PATH "C:/Program Files (x86)/Windows Kits/8.0/Lib/win8/um/x64")

find_package(OpenGL REQUIRED)
find_package(GLUT)
find_package(GLEW)
find_package(CUDA REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} 
	${CUDA_TOOLKIT_INCLUDE} ${CUDA_SDK_ROOT_DIR}/common/inc 
	required)

set(SHARED_LIBRARIES
		${OPENGL_LIBRARIES} 
		${GLUT_glut_LIBRARY}
		${GLEW_LIBRARY}
)

set( Srcs kernel.cu
	)
set( Hdrs 
OPENGL_DRIVER.h ARMADILLO.h GridMesh.h lib/CUDA_PROJECTIVE_TET_MESH.h
    )
#set( Rcs resources/vechist.qrc)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set(omp_deps gomp)

#list( APPEND CUDA_NVCC_FLAGS  -Xcompiler /openmp -DTHRUST_DEVICE_SYSTEM=THRUST_DEVICE_SYSTEM_OMP )
#list(APPEND CUDA_NVCC_FLAGS "-arch=sm_20;-std=c++11;-O2;-DVERBOSE")
#SET(CUDA_PROPAGATE_HOST_FLAGS OFF)
#qt5_add_resources(QtApp_RCC_SRCS ${Rcs})
cuda_add_executable(${PROJECT_NAME} ${Srcs} ${Hdrs} )
target_link_libraries(${PROJECT_NAME} ${SHARED_LIBRARIES})
