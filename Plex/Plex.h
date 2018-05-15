#pragma once
#include <stdlib.h>
#include <stack>

using namespace System::Drawing;
using namespace std;

struct TCurrLine {
	TChart *tc;
	TPoint *pb;
	TPoint *pe;
};

class TRoot {
	
protected:
	bool visible;
	bool active;

public:
	TRoot() {
		visible = false;
		active = false;
	}

	virtual void Show(Graphics ^gr) = 0;
	virtual void Hide(Graphics ^gr) = 0;
	virtual void Move(Graphics ^gr, int dx, int dy) = 0;

	bool getvisible() {
		return visible;
	}

	bool getactive() {
		return active;
	}
};


class TPoint : public TRoot {

protected:
	int x, y;

public:
	TPoint() {
		x = y = 0;
	}

	virtual void Show(Graphics ^gr) {

		gr->DrawEllipse(Pens::Black, x - 2, y - 2, x + 2, y + 2);
		visible = true;

	}

	virtual void Hide(Graphics ^gr) {

		gr->DrawEllipse(Pens::White, x - 2, y - 2, x + 2, y + 2);
		visible = false;

	}

	virtual void Move(Graphics ^gr, int dx, int dy) {

		Hide(gr);
		x += dx;
		y += dy;
		Show(gr);

	}
};


class TChart : public TRoot {

protected:
	TRoot *pBegin, *pEnd;
	stack <TCurrLine> st;
	

public:
	TChart(TRoot *pB = NULL, TRoot *pE = NULL) {
		pBegin = pB;
		pEnd = pE;
	}


	void setBegin(TRoot *pB) {

		TPoint *tp = dynamic_cast<TPoint*>(pB);
		TChart *tc = dynamic_cast<TChart*>(pB);
		
		if (tp != NULL || tc != NULL)
			pBegin = pB;

	}

	void setEnd(TRoot *pE) {

		TPoint *tp = dynamic_cast<TPoint*>(pE);
		TChart *tc = dynamic_cast<TChart*>(pE);

		if (tp != NULL || tc != NULL)
			pEnd = pE;

	}


	TRoot *Show(Graphics ^gr, TRoot *curr) {

		TPoint *tp, *pb, *pe;
		TChart *tc;

		tp = dynamic_cast<TPoint*>(curr);
		tc = dynamic_cast<TChart*>(curr);

		if (tp) {

			tp->Show(gr);
			return tp;
		}

		if (tc) {

			pb = dynamic_cast<TPoint*>(Show(gr, tc->pBegin));
			pe = dynamic_cast<TPoint*>(Show(gr, tc->pBegin));

			if (pb && pe) {
				gr -> DrawLine(Pens::Black, pb, pe);
				return pe;
			}
		}

	}

};