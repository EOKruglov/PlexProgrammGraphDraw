#pragma once
#include <stdlib.h>
#include <stack>

using namespace System::Drawing;
using namespace std;

class TChart;
class TPoint;
class TRoot;

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
	TRoot(bool _visible = true, bool _active = false) {
		visible = _visible;
		active = _active;
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

	void SetVisible(bool val)
	{
		visible = val;
	}

	void SetActive(bool val)
	{
		active = val;
	}
};


class TPoint : public TRoot {

protected:
	int x, y;

public:
	TPoint(int _x = 0, int _y = 0):TRoot() {
		x = _x;
		y = _y;
	}

	virtual void Show(Graphics ^gr) {

		if (visible)
			if (active)
				gr->DrawEllipse(Pens::Red, x - 2, y - 2, 5, 5);
			else
				gr->DrawEllipse(Pens::Black, x - 2, y - 2, 5, 5);

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

	int GetX()
	{
		return x;
	}

	int GetY()
	{
		return y;
	}
};


class TChart : public TRoot {

protected:
	TRoot *pBegin, *pEnd;
	stack <TCurrLine> st;
	
	

public:
	TChart *pRes;
	bool first;

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

	TRoot* GetBegin()
	{
		return pBegin;
	}

	TRoot* GetEnd()
	{
		return pEnd;
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
			pe = dynamic_cast<TPoint*>(Show(gr, tc->pEnd));

			if (pb && pe) {
				gr -> DrawLine(Pens::Black, pb->GetX(), pb->GetY(), pe->GetX(), pe->GetY());
				return tc -> pEnd;
			}
		}
		return nullptr;
	}

	TRoot* Hide(Graphics ^gr, TRoot *curr)
	{
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
			pe = dynamic_cast<TPoint*>(Show(gr, tc->pEnd));

			if (pb && pe) {
				gr->DrawLine(Pens::White, pb->GetX(), pb->GetY(), pe->GetX(), pe->GetY());
				return tc->pEnd;
			}
		}
		return nullptr;
	}

	void Show(Graphics^ gr)
	{
		TCurrLine curr;
		TPoint *tmp;

		curr.tc = this;
		curr.pb = curr.pe = NULL;

		while (!st.empty())
			st.pop();

		st.push(curr);
		while (!st.empty())
		{
			curr = st.top();
			st.pop();
			while (!curr.pb)
			{
				tmp = dynamic_cast<TPoint*>(curr.tc->GetBegin());
				if (tmp)
					curr.pb = tmp;
				else
				{
					st.push(curr);
					curr.tc =(TChart*) curr.tc->GetBegin();
				}
			}
			if (!curr.pe)
			{
				tmp = dynamic_cast<TPoint*>(curr.tc->GetEnd());
				if (tmp)
					curr.pe = tmp;
				else
				{
					st.push(curr);
					curr.tc = (TChart*)curr.tc->GetEnd();
					curr.pb = NULL;
					st.push(curr);
				}
			}
			if (curr.pb && curr.pe)
			{
				if(curr.tc->getvisible())
					if(curr.tc->getactive())
						gr->DrawLine(Pens::Red, curr.pb->GetX(), curr.pb->GetY(),
							curr.pe->GetX(), curr.pe->GetY());
					else
						gr->DrawLine(Pens::Black, curr.pb->GetX(), curr.pb->GetY(),
							curr.pe->GetX(), curr.pe->GetY());

				curr.pb->Show(gr);
				curr.pe->Show(gr);

				tmp = curr.pe;
				if (!st.empty())
				{
					curr = st.top();
					st.pop();
					if (!curr.pb)
						curr.pb = tmp;
					else
						curr.pe = tmp;
					st.push(curr);
				}
			}
		}
	}

	bool Find(int _x, int _y)
	{
		TCurrLine curr;
		TPoint *tmp;

		curr.tc = this;
		curr.pb = curr.pe = NULL;

		while (!st.empty())
			st.pop();

		st.push(curr);
		while (!st.empty())
		{
			curr = st.top();
			st.pop();
			while (!curr.pb)
			{
				tmp = dynamic_cast<TPoint*>(curr.tc->GetBegin());
				if (tmp)
					curr.pb = tmp;
				else
				{
					st.push(curr);
					curr.tc = (TChart*)curr.tc->GetBegin();
				}
			}
			if (!curr.pe)
			{
				tmp = dynamic_cast<TPoint*>(curr.tc->GetEnd());
				if (tmp)
					curr.pe = tmp;
				else
				{
					st.push(curr);
					curr.tc = (TChart*)curr.tc->GetEnd();
					curr.pb = NULL;
					st.push(curr);
				}
			}
			if (curr.pb&&curr.pe)
			{
				if (abs(curr.pb->GetX() - _x) < 10 && abs(curr.pb->GetY() - _y) < 10)
				{
					pRes = curr.tc;
					first = true;
					return true;
				}
				if (abs(curr.pe->GetX() - _x) < 10 && abs(curr.pe->GetY() - _y) < 10)
				{
					pRes = curr.tc;
					first = false;
					return true;
				}
				tmp = curr.pe;
				if (!st.empty())
				{
					curr = st.top();
					st.pop();
					if (!curr.pb)
						curr.pb = tmp;
					else
						curr.pe = tmp;
					st.push(curr);
				}
			}
		}
		return false;
	}

	void Hide(Graphics ^gr)
	{
		TCurrLine curr;
		TPoint *tmp;

		curr.tc = this;
		curr.pb = curr.pe = NULL;

		while (!st.empty())
			st.pop();

		st.push(curr);
		while (!st.empty())
		{
			curr = st.top();
			st.pop();
			while (!curr.pb)
			{
				tmp = dynamic_cast<TPoint*>(curr.tc->GetBegin());
				if (tmp)
					curr.pb = tmp;
				else
				{
					st.push(curr);
					curr.tc = (TChart*)curr.tc->GetBegin();
				}
			}
			if (!curr.pe)
			{
				tmp = dynamic_cast<TPoint*>(curr.tc->GetEnd());
				if (tmp)
					curr.pe = tmp;
				else
				{
					st.push(curr);
					curr.tc = (TChart*)curr.tc->GetEnd();
					curr.pb = NULL;
					st.push(curr);
				}
			}
			if (curr.pb && curr.pe)
			{
				gr->DrawLine(Pens::White, curr.pb->GetX(), curr.pb->GetY(),
					curr.pe->GetX(), curr.pe->GetY());
				gr->DrawEllipse(Pens::White, curr.pb->GetX() - 2, curr.pb->GetY() - 2, 5, 5);
				gr->DrawEllipse(Pens::White, curr.pe->GetX() - 2, curr.pe->GetY() - 2, 5, 5);

				tmp = curr.pe;
				if (!st.empty())
				{
					curr = st.top();
					st.pop();
					if (!curr.pb)
						curr.pb = tmp;
					else
						curr.pe = tmp;
					st.push(curr);
				}
			}
		}
	}
	void Move(Graphics ^gr, int dx, int dy)
	{
		Hide(gr);

		pBegin->Move(gr, dx, dy);
		pEnd->Move(gr, dx, dy);

		Show(gr);

	}



};