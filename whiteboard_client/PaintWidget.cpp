#include "MainWindow.h"
#include "PaintWidget.h"

// QT
#include <QtCore/QLine>
#include <QtGui/QMouseEvent>
#include <QImage>
#include <qevent.h>
#include <QPainter>

// STL
#include <algorithm>


namespace Paint 
{

	PaintWidget::PaintWidget(
		QUndoStack *undoStack, 
		QWidget *parent)
	:
		QWidget(parent),
		mpUndoStack(undoStack)
	{
	}

	bool PaintWidget::IsModified() const
	{
		return !mpUndoStack->isClean();
	}

	void PaintWidget::SetPenColor(const QColor &newColor)
	{
		mPenColor = newColor;
	}

	void PaintWidget::SetPenWidth(int newWidth)
	{
		mPenWidth = newWidth;
	}

	void PaintWidget::SetShapeFactory(ShapeFactory f)
	{
		mFactory = f;
	}

	void PaintWidget::mousePressEvent(QMouseEvent *event)
	{
		if (event->button() == Qt::LeftButton && mFactory)
		{
			CurrentShape = mFactory(event->pos(), mPenWidth, mPenColor);
			if (CurrentShape->GetType() == PObjectType::TLine)
				CurrentShape->DoSavePoints(event->pos());
		}

		Counter::getInstance().mCommandCounter++;
	}

	void PaintWidget::mouseMoveEvent(QMouseEvent *event)
	{
		if ((event->buttons() & Qt::LeftButton) && CurrentShape) 
		{
			if (CurrentShape->GetType() == PObjectType::TLine)
				CurrentShape->DoSavePoints(event->pos());

			const QRect prevRect = CurrentShape->rect();
			CurrentShape->update(event->pos());

			update(CurrentShape->rect().united(prevRect));
		}
	}

	void PaintWidget::mouseReleaseEvent(QMouseEvent *event)
	{
		if (event->button() == Qt::LeftButton && CurrentShape) 
		{
			if (CurrentShape->GetType() == PObjectType::TLine)
				CurrentShape->DoClearPoints();

			mpUndoStack->push(new ShapeCommand(this, &mImage, std::move(CurrentShape)));
		}
	}

	void PaintWidget::paintEvent(QPaintEvent *event)
	{
		const QRect paintRect = event->rect();

		QPainter painter(this);

		painter.setRenderHint(QPainter::Antialiasing, true);
		painter.drawImage(paintRect, mImage, paintRect);

		if (CurrentShape) 
		{
			CurrentShape->draw(painter);
		}
	}

	void PaintWidget::resizeEvent(QResizeEvent *event)
	{
		if (width() > mImage.width() || height() > mImage.height())
		{
			const int newWidth = qMax(width() + 128, mImage.width());
			const int newHeight = qMax(height() + 128, mImage.height());

			const QSize newSize(newWidth, newHeight);

			if (IsModified()) 
			{
				mpUndoStack->push(new ResizeCommand(this, &mImage, newSize));
			}
			else 
			{
				ResizeCommand(this, &mImage, newSize).redo();
			}
		}

		QWidget::resizeEvent(event);
	}
}