#include "BsplineCurveEvaluator.h"
#include "mat.h"
#include "vec.h"

void BsplineCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	ptvEvaluatedCurvePts.clear();

	const Mat4d convert = Mat4d(
		1, 4, 1, 0,
		0, 4, 2, 0,
		0, 2, 4, 0,
		0, 1, 4, 1) 
		/ 6.0;
	
	const Mat4d ker(
		-1, 3, -3, 1,
		3, -6, 3, 0,
		-3, 3, 0, 0,
		1, 0, 0, 0);

	std::vector<Point> ctrlPts = ptvCtrlPts;

	if (bWrap)
	{
		ctrlPts.push_back(Point(ctrlPts[0].x + fAniLength, ctrlPts[0].y));
		ctrlPts.push_back(Point(ctrlPts[1].x + fAniLength, ctrlPts[1].y));
		ctrlPts.push_back(Point(ctrlPts[2].x + fAniLength, ctrlPts[2].y));
	}
	else
	{
		ctrlPts.push_back(ptvCtrlPts[ptvCtrlPts.size() - 1]);
		ctrlPts.insert(ctrlPts.begin(), ptvCtrlPts[0]);
	}



	for (int i = 0; i < ((int)ctrlPts.size() - 3); ++i)
	{
		Vec4d ctrl_x(ctrlPts[i].x, 
					 ctrlPts[i + 1].x,
					 ctrlPts[i + 2].x, 
					 ctrlPts[i + 3].x);

		Vec4d ctrl_y(ctrlPts[i].y, 
					 ctrlPts[i + 1].y,
					 ctrlPts[i + 2].y, 
					 ctrlPts[i + 3].y);

		Vec4d convert_x = convert * ctrl_x;
		Vec4d convert_y = convert * ctrl_y;

		for (int j = 0; j < s_iSegCount; ++j)
		{
			double t = j / (double)s_iSegCount;

			Vec4d p(t * t * t, t * t, t, 1);

			float x = (float)(p * ker * convert_x);
			float y = (float)(p * ker * convert_y);

			if (x > fAniLength)
			{
				x -= fAniLength;
			}

			Point pt(x, y);
			ptvEvaluatedCurvePts.push_back(pt);
		}
	}

	if (!bWrap)
	{
		ptvEvaluatedCurvePts.push_back(Point(0, ptvCtrlPts[0].y));
		ptvEvaluatedCurvePts.push_back(ptvCtrlPts[0]);
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvCtrlPts[ptvCtrlPts.size() - 1].y));
		ptvEvaluatedCurvePts.push_back(ptvCtrlPts[ptvCtrlPts.size() - 1]);
	}
}