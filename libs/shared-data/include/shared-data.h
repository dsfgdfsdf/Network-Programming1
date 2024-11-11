#pragma once

struct QuadraticEquation
{
	double a;
	double b;
	double c;
};

enum SquareRootResult { NO_ROOT, ONE_ROOT, TWO_ROOT };

struct SquareRootData
{
	enum SquareRootResult result;

	double x1;

	double x2;
};

enum NumberOperationType { AVG, MIN, MAX };

struct NumberOperationData

{
	enum NumberOperationType type;

	int count;

	double data [1024];

};

struct NumberOperationResult {
	double avg;
	double max;
	double min;
};