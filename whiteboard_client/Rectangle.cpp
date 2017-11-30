#include "Shape.h"

namespace Paint 
{
	class Rectangle : public Shape
	{
		public:

			explicit Rectangle(
						const QPoint &topLeft,
						int penWidth,
						const QColor& penColor);

		protected:

			void DoDraw(QPainter &painter) override;

			QRect DoRect() const override;

			void DoUpdate(const QPoint &toPoint) override;

			PObjectType GetType() const override { return PObjectType::TRectangle; };


		private:

			QRect mRect;
	};

	Rectangle::Rectangle(
		const QPoint &topLeft,
		int penWidth,
		const QColor &penColor) 
	:
		Shape(penWidth, penColor), 
		mRect(topLeft, topLeft)
	{
	}

	void Rectangle::DoDraw(QPainter &painter)
	{
		if (!mRect.isNull())
			painter.drawRect(mRect.normalized());
	}

	QRect Rectangle::DoRect() const
	{
		return mRect.normalized();
	}

	void Rectangle::DoUpdate(const QPoint &toPoint)
	{
		mRect.setBottomRight(toPoint);
	}

	std::unique_ptr<Shape> CreateRectangle(
								const QPoint &topLeft,
								int penWidth,
								const QColor& penColor)
	{
		return std::unique_ptr<Shape>(new Rectangle(topLeft, penWidth, penColor));
	}
}
