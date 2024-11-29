#include "BsplineCurveEvaluator.h"
#include <assert.h>

void BsplineCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	ptvEvaluatedCurvePts.clear();
	std::vector<Point> ctrlPts = ptvCtrlPts;
	if (bWrap) {
		ctrlPts.push_back(Point(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y));
		ctrlPts.push_back(Point(ptvCtrlPts[ptvCtrlPts.size() - 1].x - fAniLength, ptvCtrlPts[1].y));
	}
	else {
		ptvEvaluatedCurvePts.push_back(Point(0, ptvCtrlPts[0].y));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvCtrlPts[ptvCtrlPts.size() - 1].y));
	}
	int n = ctrlPts.size() - 1;
	for (int i = 0; i < n - 2; i++) {
		for (int j = 0; j < s_iSegCount; j++) {
			float t = (float)j / s_iSegCount;
			float t2 = t * t;
			float t3 = t2 * t;
			float x = (float)(-1.0 / 6 * t3 + 1.0 / 2 * t2 - 1.0 / 2 * t + 1.0 / 6) * ctrlPts[i].x
				+ (float)(1.0 / 2 * t3 - t2 + 2.0 / 3 - 1.0 / 2) * ctrlPts[i + 1].x
				+ (float)(-1.0 / 2 * t3 + 1.0 / 2 * t2 + 1.0 / 2 * t + 1.0 / 6) * ctrlPts[i + 2].x
				+ (float)(1.0 / 6 * t3) * ctrlPts[i + 3].x;
			float y = (float)(-1.0 / 6 * t3 + 1.0 / 2 * t2 - 1.0 / 2 * t + 1.0 / 6) * ctrlPts[i].y
				+ (float)(1.0 / 2 * t3 - t2 + 2.0 / 3 - 1.0 / 2) * ctrlPts[i + 1].y
				+ (float)(-1.0 / 2 * t3 + 1.0 / 2 * t2 + 1.0 / 2 * t + 1.0 / 6) * ctrlPts[i + 2].y
				+ (float)(1.0 / 6 * t3) * ctrlPts[i + 3].y;
			ptvEvaluatedCurvePts.push_back(Point(x, y));
		}
	}
}