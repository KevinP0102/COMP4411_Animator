#include "BezierCurveEvaluator.h"
#include <assert.h>

void BezierCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	ptvEvaluatedCurvePts.clear();

	std::vector<Point> ctrlPts = ptvCtrlPts;
	int iCtrlPtCount = ctrlPts.size();

	if (iCtrlPtCount < 4) {
		for (const auto& pt : ctrlPts) {
			ptvEvaluatedCurvePts.push_back(pt);
		}
		if (!bWrap) {
			ptvEvaluatedCurvePts.push_back(Point(0, ptvCtrlPts[0].y));
			ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvCtrlPts[ptvCtrlPts.size() - 1].y));
		}
		else {
			float x = 0.0;
			float y1;

			if ((ptvCtrlPts[0].x + fAniLength) - ptvCtrlPts[iCtrlPtCount - 1].x > 0.0f) {
				y1 = (ptvCtrlPts[0].y * (fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x) +
					ptvCtrlPts[iCtrlPtCount - 1].y * ptvCtrlPts[0].x) /
					(ptvCtrlPts[0].x + fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x);
			}
			else
				y1 = ptvCtrlPts[0].y;

			ptvEvaluatedCurvePts.push_back(Point(x, y1));

			x = fAniLength;
			ptvEvaluatedCurvePts.push_back(Point(x, y1));
		}
		return;
	}

	if (bWrap) {
		ctrlPts.push_back(Point(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y));
	}
	else {
		ptvEvaluatedCurvePts.push_back(Point(0.0f, ptvCtrlPts[0].y));
	}

	iCtrlPtCount = ctrlPts.size();
	for (int i = 0; i <= iCtrlPtCount - 4; i += 3) {
		Point p0 = ctrlPts[i];
		Point p1 = ctrlPts[i + 1];
		Point p2 = ctrlPts[i + 2];
		Point p3 = ctrlPts[i + 3];
		for (int j = 0; j < s_iSegCount; j++) {
			float t = (float)j / s_iSegCount;
			float t2 = t * t;
			float t3 = t2 * t;
			float x = (1 - 3 * t + 3 * t2 - t3) * p0.x + (3 * t - 6 * t2 + 3 * t3) * p1.x + (3 * t2 - 3 * t3) * p2.x + t3 * p3.x;
			float y = (1 - 3 * t + 3 * t2 - t3) * p0.y + (3 * t - 6 * t2 + 3 * t3) * p1.y + (3 * t2 - 3 * t3) * p2.y + t3 * p3.y;
			
			if (x > fAniLength) {
				x -= fAniLength;
			}
			
			ptvEvaluatedCurvePts.push_back(Point(x, y));
		}

		if (i + 3 > iCtrlPtCount - 4) {
			ptvEvaluatedCurvePts.push_back(p3);
		}
	}

	if ((iCtrlPtCount - 1) % 3 != 0) {
		for (int i = iCtrlPtCount - (iCtrlPtCount - 1) % 3; i < iCtrlPtCount; i++) {
			ptvEvaluatedCurvePts.push_back(ctrlPts[i]);

			if (bWrap && i == iCtrlPtCount - 1) {
				float y2 = (ctrlPts[iCtrlPtCount - 1].y * (fAniLength - ctrlPts[iCtrlPtCount - 2].x) + ctrlPts[iCtrlPtCount - 2].y * ctrlPts[0].x) /
					(ctrlPts[iCtrlPtCount - 1].x - ctrlPts[iCtrlPtCount - 2].x);
				ptvEvaluatedCurvePts.push_back(Point(0.0f, y2));
			}
		}
	}

	if (!bWrap) {
		ptvEvaluatedCurvePts.push_back(Point(ptvCtrlPts[ptvCtrlPts.size() - 1].x, ptvCtrlPts[ptvCtrlPts.size() - 1].y));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvCtrlPts[ptvCtrlPts.size() - 1].y));
	}
}