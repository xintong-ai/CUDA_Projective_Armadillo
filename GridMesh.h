///////////////////////////////////////////////////////////////////////////////////////////
//  Class GridMesh
///////////////////////////////////////////////////////////////////////////////////////////
#ifndef GRID_MESH_H
#define GRID_MESH_H
#include "../lib/CUDA_PROJECTIVE_TET_MESH.h"
#include <vector_types.h>
#include <vector_functions.h>

inline int IdxConv(int idx, int nStep[3], int3 vc)
{
	return idx + vc.x * nStep[1] * nStep[2] + vc.y * nStep[2] + vc.z;
}

inline int IdxConv(int i, int j, int k, int nStep[3], int3 vc)
{
	return (i + vc.x) * nStep[1] * nStep[2] + (j + vc.y) * nStep[2] + k + vc.z;
}

template <class TYPE>
inline void Copy3(TYPE from[3], TYPE to[3])
{
	to[0] = from[0];
	to[1] = from[1];
	to[2] = from[2];
}

template <class TYPE>
class GridMesh: public CUDA_PROJECTIVE_TET_MESH<TYPE> 
{
public:
	void Build_Boundary_Triangles2()
	{
		t_number = tet_number * 4;
		for (int i = 0; i < tet_number; i++) {
			T[i * 4 * 3 + 0 * 3 + 0] = Tet[i * 4 + 0];
			T[i * 4 * 3 + 0 * 3 + 1] = Tet[i * 4 + 1];
			T[i * 4 * 3 + 0 * 3 + 2] = Tet[i * 4 + 2];

			T[i * 4 * 3 + 1 * 3 + 0] = Tet[i * 4 + 0];
			T[i * 4 * 3 + 1 * 3 + 1] = Tet[i * 4 + 1];
			T[i * 4 * 3 + 1 * 3 + 2] = Tet[i * 4 + 3];

			T[i * 4 * 3 + 2 * 3 + 0] = Tet[i * 4 + 0];
			T[i * 4 * 3 + 2 * 3 + 1] = Tet[i * 4 + 2];
			T[i * 4 * 3 + 2 * 3 + 2] = Tet[i * 4 + 3];

			T[i * 4 * 3 + 3 * 3 + 0] = Tet[i * 4 + 1];
			T[i * 4 * 3 + 3 * 3 + 1] = Tet[i * 4 + 2];
			T[i * 4 * 3 + 3 * 3 + 2] = Tet[i * 4 + 3];
		}
	}

	void BuildMesh(TYPE dmin[3], TYPE dmax[3], TYPE step)
	{
		int nStep[3];
		for (int i = 0; i < 3; i++){
			nStep[i] = ceil((dmax[i] - dmin[i]) / step) + 1;
		}
		number = nStep[0] * nStep[1] * nStep[2];
		int idx = 0;
		for (int i = 0; i < nStep[0]; i++){
			for (int j = 0; j < nStep[1]; j++){
				for (int k = 0; k < nStep[2]; k++){
					idx = i * nStep[1] * nStep[2] + j * nStep[2] + k;
					X[3 * idx + 0] = dmin[0] + i * step;
					X[3 * idx + 1] = dmin[1] + j * step;
					X[3 * idx + 2] = dmin[2] + k * step;
				}
			}
		}

		int3 vc[8];
		vc[0] = make_int3(0, 0, 0);
		vc[1] = make_int3(1, 0, 0);
		vc[2] = make_int3(0, 1, 0);
		vc[3] = make_int3(1, 1, 0);
		vc[4] = make_int3(0, 0, 1);
		vc[5] = make_int3(1, 0, 1);
		vc[6] = make_int3(0, 1, 1);
		vc[7] = make_int3(1, 1, 1);

		int idx2 = 0;
		tet_number = (nStep[0] - 1) * (nStep[1] - 1) * (nStep[2] - 1) * 5;
		for (int i = 0; i < (nStep[0] - 1); i++){
			for (int j = 0; j < (nStep[1] - 1); j++){
				for (int k = 0; k < (nStep[2] - 1); k++){
					idx = i * (nStep[1] - 1) * (nStep[2] - 1) + j * (nStep[2] - 1) + k;
					idx2 = i * nStep[1] * nStep[2] + j * nStep[2] + k;
					if ((i + j + k) % 2 == 0) {
						Tet[idx * 5 * 4 + 4 * 0 + 0] = IdxConv(idx2, nStep, vc[0]);
						Tet[idx * 5 * 4 + 4 * 0 + 1] = IdxConv(idx2, nStep, vc[1]);
						Tet[idx * 5 * 4 + 4 * 0 + 2] = IdxConv(idx2, nStep, vc[2]);
						Tet[idx * 5 * 4 + 4 * 0 + 3] = IdxConv(idx2, nStep, vc[4]);

						Tet[idx * 5 * 4 + 4 * 1 + 0] = IdxConv(idx2, nStep, vc[3]);
						Tet[idx * 5 * 4 + 4 * 1 + 1] = IdxConv(idx2, nStep, vc[1]);
						Tet[idx * 5 * 4 + 4 * 1 + 2] = IdxConv(idx2, nStep, vc[2]);
						Tet[idx * 5 * 4 + 4 * 1 + 3] = IdxConv(idx2, nStep, vc[7]);

						Tet[idx * 5 * 4 + 4 * 2 + 0] = IdxConv(idx2, nStep, vc[4]);
						Tet[idx * 5 * 4 + 4 * 2 + 1] = IdxConv(idx2, nStep, vc[5]);
						Tet[idx * 5 * 4 + 4 * 2 + 2] = IdxConv(idx2, nStep, vc[1]);
						Tet[idx * 5 * 4 + 4 * 2 + 3] = IdxConv(idx2, nStep, vc[7]);

						Tet[idx * 5 * 4 + 4 * 3 + 0] = IdxConv(idx2, nStep, vc[2]);
						Tet[idx * 5 * 4 + 4 * 3 + 1] = IdxConv(idx2, nStep, vc[4]);
						Tet[idx * 5 * 4 + 4 * 3 + 2] = IdxConv(idx2, nStep, vc[6]);
						Tet[idx * 5 * 4 + 4 * 3 + 3] = IdxConv(idx2, nStep, vc[7]);

						Tet[idx * 5 * 4 + 4 * 4 + 0] = IdxConv(idx2, nStep, vc[1]);
						Tet[idx * 5 * 4 + 4 * 4 + 1] = IdxConv(idx2, nStep, vc[2]);
						Tet[idx * 5 * 4 + 4 * 4 + 2] = IdxConv(idx2, nStep, vc[4]);
						Tet[idx * 5 * 4 + 4 * 4 + 3] = IdxConv(idx2, nStep, vc[7]);
					}
					else{
						Tet[idx * 5 * 4 + 4 * 0 + 0] = IdxConv(idx2, nStep, vc[0]);
						Tet[idx * 5 * 4 + 4 * 0 + 1] = IdxConv(idx2, nStep, vc[1]);
						Tet[idx * 5 * 4 + 4 * 0 + 2] = IdxConv(idx2, nStep, vc[3]);
						Tet[idx * 5 * 4 + 4 * 0 + 3] = IdxConv(idx2, nStep, vc[5]);

						Tet[idx * 5 * 4 + 4 * 1 + 0] = IdxConv(idx2, nStep, vc[0]);
						Tet[idx * 5 * 4 + 4 * 1 + 1] = IdxConv(idx2, nStep, vc[2]);
						Tet[idx * 5 * 4 + 4 * 1 + 2] = IdxConv(idx2, nStep, vc[3]);
						Tet[idx * 5 * 4 + 4 * 1 + 3] = IdxConv(idx2, nStep, vc[6]);

						Tet[idx * 5 * 4 + 4 * 2 + 0] = IdxConv(idx2, nStep, vc[0]);
						Tet[idx * 5 * 4 + 4 * 2 + 1] = IdxConv(idx2, nStep, vc[4]);
						Tet[idx * 5 * 4 + 4 * 2 + 2] = IdxConv(idx2, nStep, vc[5]);
						Tet[idx * 5 * 4 + 4 * 2 + 3] = IdxConv(idx2, nStep, vc[6]);

						Tet[idx * 5 * 4 + 4 * 3 + 0] = IdxConv(idx2, nStep, vc[3]);
						Tet[idx * 5 * 4 + 4 * 3 + 1] = IdxConv(idx2, nStep, vc[5]);
						Tet[idx * 5 * 4 + 4 * 3 + 2] = IdxConv(idx2, nStep, vc[6]);
						Tet[idx * 5 * 4 + 4 * 3 + 3] = IdxConv(idx2, nStep, vc[7]);

						Tet[idx * 5 * 4 + 4 * 4 + 0] = IdxConv(idx2, nStep, vc[0]);
						Tet[idx * 5 * 4 + 4 * 4 + 1] = IdxConv(idx2, nStep, vc[3]);
						Tet[idx * 5 * 4 + 4 * 4 + 2] = IdxConv(idx2, nStep, vc[5]);
						Tet[idx * 5 * 4 + 4 * 4 + 3] = IdxConv(idx2, nStep, vc[6]);

					}
				}
			}
		}
		Build_Boundary_Triangles2();
	}


	GridMesh()
	{
		//Read_Original_File("sorted_armadillo");
		//Read_Original_File("armadillo_10k.1");
		float dataMin[3] = { -0.50, -0.50, -0.50 };
		float dataMax[3] = { 0.50, 0.50, 0.50 };
		BuildMesh(dataMin, dataMax, 0.1);
		//Scale(0.008);
		Centralize();
		printf("N: %d, %d\n", number, tet_number);

		//Set fixed nodes
		//Rotate_X(-0.2);
		//for(int v=0; v<number; v++)
		//	//if(X[v*3+1]>-0.04 && X[v*3+1]<0)		
		//	//	if(fabsf(X[v*3+1]+0.01)<1*(X[v*3+2]-0.1))
		//	if(fabsf(X[v*3+1]+0.01)<2*(X[v*3+2]-0.2))
		//		fixed[v]=10000000;
		//Rotate_X(1.2);

		for(int v=0; v<number; v++)
			if (	X[v * 3 + 0] < -0.49 || X[v * 3 + 0] > 0.49
				||	X[v * 3 + 1] < -0.49 || X[v * 3 + 1] > 0.49
				||	X[v * 3 + 2] < -0.49 || X[v * 3 + 2] > 0.49)
				fixed[v]=10000000;

		elasticity = 1800;// 18000000; //5000000
		control_mag	= 500;		//500
		damping		= 1;
	}

};


#endif
