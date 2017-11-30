#include "shape.h"


namespace Paint 
{
	Shape::Shape(
		int penWidth, 
		const QColor& penColor) 
	:
		mPenWidth(penWidth), 
		mPenColor(penColor)
	{
	}

	Shape::~Shape()
	{
	}

	void Shape::draw(QPainter &painter)
	{
		const QPen prevPen = painter.pen();
		const QPen currPen(mPenColor, mPenWidth, Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin);

		painter.setPen(currPen);

		DoDraw(painter);

		painter.setPen(prevPen);
	}

	QRect Shape::rect() const
	{
		const int rad = mPenWidth / 2 + 2;

		return DoRect().adjusted(-rad, -rad, +rad, +rad);
	}

	void Shape::update(const QPoint &toPoint)
	{
		DoUpdate(toPoint);
	}
}
