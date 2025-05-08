#pragma once

//Simple perlin noise generaotr, para-phrased from:https://github.com/christopher-little/PerlinNoiseCpp/tree/master
class PerlinEngine
{
public:
	PerlinEngine();
	~PerlinEngine();

	float Noise(float, float);

private:
	int* table;
	float* gradX;
	float* gradY;

};
