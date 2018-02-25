#pragma once

template<typename _T>
class CPoint{
public:
	_T x, y;

	CPoint():x(0), y(0){}
	CPoint(_T x, _T y):x(x), y(y){}
	CPoint(const CPoint &a):x(a.x), y(a.y){}

	CPoint& operator=(const CPoint &a){
		x = a.x;
		y = a.y;
		return *this;
	}

	const CPoint operator+(const CPoint &a){
		return CPoint(x + a.x, y + a.y);
	}

	CPoint &operator+=(const CPoint &a){
		x += a.x, y += a.y;
		return *this;
	}

	const CPoint operator-(const CPoint &a){
		return CPoint(x - a.x, y - a.y);
	}

	CPoint &operator-=(const CPoint &a){
		x -= a.x, y -= a.y;
		return *this;
	}

	bool operator==(const CPoint &a){
		return x == a.x&&y == a.y;
	}

	bool operator!=(const CPoint &a){
		return !( x == a.x&&y == a.y );
	}
};
