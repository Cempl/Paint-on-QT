#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

// QT
#include <QWidget>
#include <QtCore/QList>
#include <QtCore/QPoint>
#include <QtGui/QPainter>
#include <QPicture>
#include <QUndoStack>
#include <QtWidgets>
#include <QWidget>

// STL
#include <functional>
#include <memory>

//
#include "Shape.h"
#include "Command.h"
#include "Counter.h"


namespace Paint 
{

	class PaintWidget : public QWidget
	{
			Q_OBJECT

		public:

			/**
			 * @brief Shapes factory function prototype
			 */
			typedef std::function<std::unique_ptr<Shape>(const QPoint&, int, const QColor&)> ShapeFactory;

			explicit PaintWidget(
						QUndoStack *undoStack, 
						QWidget *parent = 0);

			bool IsModified() const;

			void SetPenColor(const QColor &newColor);
			void SetPenWidth(int newWidth);

			QColor GetPenColor() const { return mPenColor; }
			int GetPenWidth() const { return mPenWidth; }

			void SetShapeFactory(ShapeFactory f);

		protected:

			void mousePressEvent(QMouseEvent *event);
			void mouseReleaseEvent(QMouseEvent *event);
			void mouseMoveEvent(QMouseEvent *event);
			void paintEvent(QPaintEvent *event);
			void resizeEvent(QResizeEvent *event);

		private:

			QImage mImage;
			QUndoStack* mpUndoStack;

			int mPenWidth;
			QColor mPenColor;

			ShapeFactory mFactory;

			std::unique_ptr<Shape> CurrentShape;
	};

}

#endif // PAINTWIDGET_H