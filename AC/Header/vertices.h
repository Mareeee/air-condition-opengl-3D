#pragma once
#include <map>
#include <string>
#include <vector>

extern float verticesButton[];
extern float verticesПречага[];
extern float verticesCrosshair[];

std::map<std::string, float*> getVerticesMap();
std::map<std::string, size_t> getVerticesSizes();
std::vector<float> generateCylinder(float radius, float height, int segments);
std::vector<float> generateSphere(float radius, int segments);