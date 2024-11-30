#ifndef BSPLINECURVEEVALUATOR_H
#define BSPLINECURVEEVALUATOR_H

#include "CurveEvaluator.h"

//using namespace std;

class BsplineCurveEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;
};

#endif 