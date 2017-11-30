#include "Shape.h"

// QT
#include <QPolygon>


namespace Paint 
{
	class Curve : public Shape
	{
		public:

			explicit Curve(
						const QPoint &topLeft,
						int penWidth,
						const QColor& penColor);

		protected:

			void DoDraw(QPainter &painter) override;

			QRect DoRect() const override;

			void DoUpdate(const QPoint &toPoint) override;

			PObjectType GetType() const override { return PObjectType::TCurve; };

		private:

			QPolygon mPoly;
	};

	Curve::Curve(
		const QPoint &topLeft,
		int penWidth,
		const QColor &penColor) 
	:
		Shape(penWidth, penColor)
	{
		update(topLeft);
	}

	void Curve::DoDraw(QPainter &painter)
	{
		painter.drawPolyline(mPoly);
	}

	QRect Curve::DoRect() const
	{
		return mPoly.boundingRect();
	}

	void Curve::DoUpdate(const QPoint &toPoint)
	{
		mPoly << toPoint;
	}

	std::unique_ptr<Shape> CreateCurve(
								const QPoint &topLeft,
								int penWidth,
								const QColor& penColor)
	{
		return std::unique_ptr<Shape>(new Curve(topLeft, penWidth, penColor));
	}
}
