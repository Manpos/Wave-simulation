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
float gravity = -4.9;

float stretchD = 50, shearD = 50, bendD = 50, stretchK = 1000, shearK, bendK;
float elasticity = 0.5;

//Wave parameters
float A = 0.5;	//Amplitude
float omega = 2;
vec3 waveVector;
float time = 0;

//Sphere parameters
vec3 spherePosition = vec3(0.0f, 10.0f, 0.0f);
float sphereRadius = 0.5;
vec3 sphereOnWavePosition = vec3(0.0f);
vec3 initialSpherePosition;
float sphereMass = 1;
float sphereArea = 3.14 * (sphereRadius * sphereRadius);
vec3 sphereVelocity;

//Forces parameters
float p = 2.5; //Density of the fluid
float vSub; //Volume "submergit" of the sphere
float dragCoeff = 0.75; //Give a value between 0.1 and 1.1

void Restart();
float timeCounter;
#pragma endregion

bool show_test_window = false;

void GUI() {
	{	//FrameRate
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::SliderFloat("Sphere Mass", &sphereMass, 1, 10);
		ImGui::SliderFloat("Drag coefficient", &dragCoeff, 0.2, 1.1);
		ImGui::InputFloat("Fluid density", &p);
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

float BuoyancyForce() {
	float diameter = sphereRadius * 2;

	float subHeight = clamp(sphereOnWavePosition.y - (spherePosition.y - sphereRadius), 0.f, diameter);

	//vSub = (3.14 / 3) * (subHeight * subHeight) * (1.5 * (sphereRadius * 2) - subHeight);
	vSub = diameter * diameter * subHeight;
	
	return (p * (4.9) * vSub);
}

vec3 DragForce() {
	vec3 velocityDiff = sphereVelocity;
	return (-0.5f * p * (length(velocityDiff)) * sphereArea * dragCoeff) * sphereVelocity;
}

void UpdateSphere(float dt) {
	vec3 totalForce;
	sphereOnWavePosition = spherePosition;
	sphereOnWavePosition.y = A * cos(dot(waveVector, initialSpherePosition) - omega * time) + 5;	

	if ((spherePosition.y - sphereRadius) <= sphereOnWavePosition.y) {
		totalForce.y = (gravity * sphereMass) + BuoyancyForce();
		totalForce += DragForce();
	}
	else
		totalForce.y = gravity;
	
	sphereVelocity += totalForce * dt;

	// Euler movement
	spherePosition.y += sphereVelocity.y * dt;
}

void PhysicsInit() {
	//TODO
	vec3 meshPosition(-8.5, 5, -6.5);
	waveInitialVertexPosition = CreateClothMeshArray(ClothMesh::numRows, ClothMesh::numCols, separationX, separationY, meshPosition);
	waveVertexPosition = CreateClothMeshArray(ClothMesh::numRows, ClothMesh::numCols, separationX, separationY, meshPosition);

	initialSpherePosition = vec3(spherePosition.x, 5, spherePosition.z);

}

void PhysicsUpdate(float dt) {
	//TODO
	time += dt;
	for (int i = 0; i < ClothMesh::numVerts; ++i) {
		waveVector = vec3(1.0, 1.0, 0.0);
		waveVertexPosition[i] = waveInitialVertexPosition[i] - normalize(waveVector) * A * sin(dot(waveVector, waveInitialVertexPosition[i]) - omega * time);
		waveVertexPosition[i].y = A * cos(dot(waveVector, waveInitialVertexPosition[i]) - omega * time) + 5;
	}

	UpdateSphere(dt);
	ClothMesh::updateClothMesh(&waveVertexPosition[0].x);

	if (ImGui::Button("Restart") || timeCounter >= 20) {
		Restart();
	}
	timeCounter += dt;
}
void PhysicsCleanup() {
	//TODO
}

void Restart() {
	sphereVelocity = vec3(0.0);
	spherePosition.x = -5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (5 - -5)));
	spherePosition.y = 6 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - 6)));
	spherePosition.z = -5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (5 - -5)));
	
	vec3 meshPosition(-8.5, 5, -6.5);
	waveInitialVertexPosition = CreateClothMeshArray(ClothMesh::numRows, ClothMesh::numCols, separationX, separationY, meshPosition);
	waveVertexPosition = CreateClothMeshArray(ClothMesh::numRows, ClothMesh::numCols, separationX, separationY, meshPosition);

	initialSpherePosition = vec3(spherePosition.x, 5, spherePosition.z);
	timeCounter = 0;
}