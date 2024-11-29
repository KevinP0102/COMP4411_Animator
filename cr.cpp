#include "CRCurveEvaluator.h"
#include <assert.h>

//catmull-rom curve evaluator
void CRCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
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

			float x = 0.5 * ((2 * ctrlPts[i + 1].x) + (-ctrlPts[i].x + ctrlPts[i + 2].x) * t +
				(2 * ctrlPts[i].x - 5 * ctrlPts[i + 1].x + 4 * ctrlPts[i + 2].x - ctrlPts[i + 3].x) * t2 +
				(-ctrlPts[i].x + 3 * ctrlPts[i + 1].x - 3 * ctrlPts[i + 2].x + ctrlPts[i + 3].x) * t3);

			float y = 0.5 * ((2 * ctrlPts[i + 1].y) + (-ctrlPts[i].y + ctrlPts[i + 2].y) * t +
				(2 * ctrlPts[i].y - 5 * ctrlPts[i + 1].y + 4 * ctrlPts[i + 2].y - ctrlPts[i + 3].y) * t2 +
				(-ctrlPts[i].y + 3 * ctrlPts[i + 1].y - 3 * ctrlPts[i + 2].y + ctrlPts[i + 3].y) * t3);

			ptvEvaluatedCurvePts.push_back(Point(x, y));
		}
	}

}