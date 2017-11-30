#ifndef SHAPE_H
#define SHAPE_H

#include <QColor>
#include <QPainter>
#include <QPoint>
#include <QRect>

// STL
#include <memory>


namespace Paint 
{

enum PObjectType
{
	TLine,
	TCurve,
	TRectangle,
	TEllipse
};

class PaintWidget;

/**
 * @brief The Shape base class representing all shapes drawn on the PaintWidget.
 */
class Shape
{
	public:

		Shape(
			int penWidth, 
			const QColor& penColor);

		virtual ~Shape();

		/**
		 * @brief Draw the shape using a painter.
		 * @param painter The painter to use for drawing.
		 */
		void draw(QPainter &painter);

		/**
		 * @brief Get the rectangle containing the shape.
		 * @return The shape's rectangle.
		 */
		QRect rect() const;

		/**
		 * @brief Update the shape while "stretching" (drawing) it
		 * @param toPoint The last recorded point where the shape should be extended
		 *                to.
		 */
		void update(const QPoint &toPoint);

		virtual PObjectType GetType() const = 0;

		virtual void DoSavePoints(const QPoint& currPoints){};

		virtual void DoClearPoints(){};

	protected:

		virtual void DoDraw(QPainter& painter) = 0;

		virtual QRect DoRect() const = 0;

		virtual void DoUpdate(const QPoint& toPoint) = 0;

	private:

		int mPenWidth;
		QColor mPenColor;
};

std::unique_ptr<Shape> CreateEllipse(const QPoint &topLeft,
                                     int penWidth,
                                     const QColor& penColor);

std::unique_ptr<Shape> CreateRectangle(const QPoint &topLeft,
                                       int penWidth,
                                       const QColor& penColor);

std::unique_ptr<Shape> CreateCurve(const QPoint &topLeft,
                                      int penWidth,
                                      const QColor& penColor);

std::unique_ptr<Shape> CreateLine(const QPoint &topLeft,
                                      int penWidth,
                                      const QColor& penColor);

}

#endif // SHAPE_H
