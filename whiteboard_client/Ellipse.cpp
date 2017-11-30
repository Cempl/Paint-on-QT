#include "Shape.h"


namespace Paint 
{
	class Ellipse : public Shape
	{
		public:

			explicit Ellipse(
						const QPoint &topLeft,
						int penWidth,
						const QColor& penColor);

		protected:

			void DoDraw(QPainter &painter) override;

			QRect DoRect() const override;

			void DoUpdate(const QPoint &toPoint) override;

			PObjectType GetType() const override { return PObjectType::TEllipse; };

		private:

			QRect mRect;
	};

	Ellipse::Ellipse(
		const QPoint &topLeft, 
		int penWidth, 
		const QColor &penColor) 
	:
		Shape(penWidth, penColor), 
		mRect(topLeft, topLeft)
	{
	}

	void Ellipse::DoDraw(QPainter &painter)
	{
		if (!mRect.isNull())
			painter.drawEllipse(mRect.normalized());
	}

	QRect Ellipse::DoRect() const
	{
		return mRect.normalized();
	}

	void Ellipse::DoUpdate(const QPoint &toPoint)
	{
		mRect.setBottomRight(toPoint);
	}

	std::unique_ptr<Shape> CreateEllipse(
								const QPoint &topLeft,
								int penWidth,
								const QColor& penColor)
	{
		return std::unique_ptr<Shape>(new Ellipse(topLeft, penWidth, penColor));
	}
}
