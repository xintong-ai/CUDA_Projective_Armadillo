///////////////////////////////////////////////////////////////////////////////////////////
//  Copyright (C) 2002 - 2015, Huamin Wang
//  All rights reserved.
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//     1. Redistributions of source code must retain the above copyright
//        notice, this list of conditions and the following disclaimer.
//     2. Redistributions in binary form must reproduce the above copyright
//        notice, this list of conditions and the following disclaimer in the
//        documentation and/or other materials provided with the distribution.
//     3. The names of its contributors may not be used to endorse or promote
//        products derived from this software without specific prior written
//        permission.
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//	NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///////////////////////////////////////////////////////////////////////////////////////////
//  Class ARMADILLO
///////////////////////////////////////////////////////////////////////////////////////////
#ifndef GRID_MESH_H
#define GRID_MESH_H
#include "../lib/CUDA_PROJECTIVE_TET_MESH.h"
#include <vector_types.h>
#include <vector_functions.h>

template <class TYPE>
class GridMesh: public CUDA_PROJECTIVE_TET_MESH<TYPE> 
{
public:
	void BuildMesh(TYPE dmin[3], TYPE dmax[3], TYPE step)
	{
		//char filename[1024];
		//int temp_value;
		//int bound;

		//sprintf_s(filename, sizeof(filename), "%s.node", name);
		//FILE *fp;
		//fopen_s(&fp, filename, "r+");
		//if (fp == NULL)	{ printf("ERROR: file %s not open.\n", filename); return; }
		//fscanf_s(fp, "%d %d %d %d\n", &number, &temp_value, &temp_value, &bound);
		//if (bound == 0)
		//	for (int i = 0; i<number; i++)
		//	{
		//	float temp_x0, temp_x1, temp_x2;
		//	fscanf_s(fp, "%d %f %f %f\n", &temp_value, &temp_x0, &temp_x1, &temp_x2);
		//	X[i * 3 + 0] = temp_x0;
		//	X[i * 3 + 1] = temp_x1;
		//	X[i * 3 + 2] = temp_x2;
		//	}
		//else
		//	for (int i = 0; i<number; i++)
		//	{
		//	float temp_x0, temp_x1, temp_x2;
		//	fscanf_s(fp, "%d %f %f %f %d\n", &temp_value, &temp_x0, &temp_x1, &temp_x2, &temp_value);
		//	X[i * 3 + 0] = temp_x0;
		//	X[i * 3 + 1] = temp_x1;
		//	X[i * 3 + 2] = temp_x2;
		//	}

		//fclose(fp);

		//sprintf_s(filename, sizeof(filename), "%s.ele", name);
		//fopen_s(&fp, filename, "r+");
		//if (fp == NULL)	{ printf("ERROR: file %s not open.\n", filename); return; }
		//fscanf_s(fp, "%d %d %d\n", &tet_number, &temp_value, &bound);

		//if (bound == 0)
		//	for (int i = 0; i<tet_number; i++)
		//		fscanf_s(fp, "%d %d %d %d %d\n", &temp_value, &Tet[i * 4 + 0], &Tet[i * 4 + 1], &Tet[i * 4 + 2], &Tet[i * 4 + 3]);
		//else if (bound == 1)
		//	for (int i = 0; i<tet_number; i++)
		//		fscanf_s(fp, "%d %d %d %d %d %d\n", &temp_value, &Tet[i * 4 + 0], &Tet[i * 4 + 1], &Tet[i * 4 + 2], &Tet[i * 4 + 3], &temp_value);
		//fclose(fp);
		int nStep[3];
		for (int i = 0; i < 3; i++){
			nStep[i] = ceil((dmax[i] - dmin[i]) / step);
		}
		number = nStep[0] * nStep[1] * nStep[2];
		int idx = 0;
		for (int i = 0; i < nStep[0]; i++){
			for (int j = 0; j < nStep[1]; j++){
				for (int k = 0; k < nStep[2]; k++){
					idx = i * nStep[1] * nStep[2] + j * nStep[2] + k;
					X[3 * idx + 0] = i * step;
					X[3 * idx + 1] = j * step;
					X[3 * idx + 2] = k * step;
				}
			}
		}

		tet_number = (nStep[0] - 1) * (nStep[1] - 1) * (nStep[2] - 1) * 5;
		for (int i = 0; i < (nStep[0] - 1); i++){
			for (int j = 0; j < (nStep[1] - 1); j++){
				for (int k = 0; k < (nStep[2] - 1); k++){
					idx = i * (nStep[1] - 1) * (nStep[2] - 1) + j * (nStep[2] - 1) + k;
					Tet[idx * 5 * 4 + 0] = i * nStep[1] * nStep[2] + j * nStep[2] + k;
				}
			}
		}
		//for (int i = 0; i<tet_number; i++)
		//{
		//	Tet[i * 4 + 0] -= 1;
		//	Tet[i * 4 + 1] -= 1;
		//	Tet[i * 4 + 2] -= 1;
		//	Tet[i * 4 + 3] -= 1;
		//}
		Build_Boundary_Triangles();
	}


	GridMesh()
	{
		//Read_Original_File("sorted_armadillo");
		Read_Original_File("armadillo_10k.1");
		Scale(0.008);
		Centralize();
		printf("N: %d, %d\n", number, tet_number);

		//Set fixed nodes
		Rotate_X(-0.2);
		for(int v=0; v<number; v++)
			//if(X[v*3+1]>-0.04 && X[v*3+1]<0)		
			//	if(fabsf(X[v*3+1]+0.01)<1*(X[v*3+2]-0.1))
			if(fabsf(X[v*3+1]+0.01)<2*(X[v*3+2]-0.2))
				fixed[v]=10000000;
		Rotate_X(1.2);

		elasticity	= 18000000; //5000000
		control_mag	= 500;		//500
		damping		= 1;
	}

};


#endif
