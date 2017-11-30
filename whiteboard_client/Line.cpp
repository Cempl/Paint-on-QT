#include "Shape.h"

// QT
#include <QPolygon>
#include <QDebug>


namespace Paint
{

	class Line : public Shape
	{
		public:

			explicit Line(
						const QPoint &topLeft,
						int penWidth,
						const QColor& penColor);

		protected:

			void DoDraw(QPainter &painter) override;

			QRect DoRect() const override;

			void DoUpdate(const QPoint &toPoint) override;

			PObjectType GetType() const override { return PObjectType::TLine; };

			void DoSavePoints(const QPoint& currPoints) override { mBuffer.push_back(currPoints); };

			void DoClearPoints() override { mBuffer.clear(); };
			 
		private:

			QPolygon mPoly;

			QList<QPoint> mBuffer;
	};

	Line::Line(
		const QPoint &topLeft,
		int penWidth,
		const QColor &penColor)
	:
		Shape(penWidth, penColor)
	{
		update(topLeft);
	}

	void Line::DoDraw(QPainter &painter)
	{
		if (!mBuffer.isEmpty())
		{
			mPoly.clear();
			mPoly <<  mBuffer[0];
			mPoly <<  mBuffer[mBuffer.size() - 1];
		}

		painter.drawPolyline(mPoly);
	}

	QRect Line::DoRect() const
	{
		return mPoly.boundingRect();
	}

	void Line::DoUpdate(const QPoint &toPoint)
	{
		mPoly << toPoint;
	}

	std::unique_ptr<Shape> CreateLine(
								const QPoint &topLeft,
								int penWidth,
								const QColor& penColor)
	{
		return std::unique_ptr<Shape>(new Line(topLeft, penWidth, penColor));
	}
}
