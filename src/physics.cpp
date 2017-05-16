#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <glm\glm.hpp>
#include <iostream>

using namespace glm;
using namespace std;

namespace ClothMesh {
	extern const int numCols;
	extern const int numRows;
	extern const int numVerts;
	extern void updateClothMesh(float* array_data);
}

vec3 *waveVertexPosition;
vec3 *waveInitialVertexPosition;

#pragma region Variables

float separationX = 1, separationY = 1;
float gravity = -9.8;
vec3 spherePosition(0.0f, 1.0f, 0.0f);
float sphereRadius = 3.0f;
float stretchD = 50, shearD = 50, bendD = 50, stretchK = 1000, shearK, bendK;
float elasticity = 0.5;

//Wave parameters
float A = 0.5;	//Amplitude
float omega = 2;
vec3 waveVector;
float time = 0;
#pragma endregion

bool show_test_window = false;

void GUI() {
	{	//FrameRate
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		//TODO
	}

	// ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if(show_test_window) {
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}

vec3 * CreateClothMeshArray(int rowVerts, int columnVerts, float vertexSeparationX, float vertexSeparationY, vec3 position) {
	int currPosX = 0, currPosY = 0;
	vec3 *result = new vec3[columnVerts * rowVerts];

	for (int i = 0; i < rowVerts * columnVerts; ++i) {

		if (currPosX < columnVerts) {
			result[i] = vec3(position.x + vertexSeparationY * currPosY, position.y, position.z + vertexSeparationY * currPosX);
		}

		currPosX++;

		if (currPosY < rowVerts && currPosX >= columnVerts) {
			currPosX = 0;
			currPosY++;
		}
	}
	return result;
}

void PhysicsInit() {
	//TODO
	vec3 meshPosition(-8.5, 5, -6.5);
	waveInitialVertexPosition = waveVertexPosition = CreateClothMeshArray(ClothMesh::numRows, ClothMesh::numCols, separationX, separationY, meshPosition);
}
void PhysicsUpdate(float dt) {
	//TODO
	time += dt;
	for (int i = 0; i < ClothMesh::numVerts; ++i) {
		waveVector = vec3(1.0, 1.0, 1.0);
		//waveVertexPosition[i].z = waveInitialVertexPosition[i].z - normalize(waveVector).z * A * sin(waveVector.z * omega * time);
		//waveVertexPosition[i].x = waveInitialVertexPosition[i].x - normalize(waveVector).x * A * sin(waveVector.z * omega * time);
		waveVertexPosition[i].y = A * cos(waveVector.y * waveInitialVertexPosition[i].x - omega * time) + 5;
	}

	ClothMesh::updateClothMesh(&waveVertexPosition[0].x);
}
void PhysicsCleanup() {
	//TODO
}