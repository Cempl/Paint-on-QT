#include "Command.h"
#include "PaintWidget.h"

namespace Paint 
{
	enum CommandId 
	{ 
		ResizeCommandId = 1 
	};

	ShapeCommand::ShapeCommand(
		QWidget *doc, 
		QImage *image,
		std::unique_ptr<Shape> &&s) 
	:
		mpDoc(doc), 
		mpImage(image),
		mUndoImage(image->copy(s->rect())), mShape(std::move(s))
	{
	}

	void ShapeCommand::undo()
	{
		const QRect rect = mShape->rect();

		QPainter painter(mpImage);

		painter.drawImage(rect, mUndoImage);

		mpDoc->update(rect);
	}

	void ShapeCommand::redo()
	{
		QPainter painter(mpImage);

		mShape->draw(painter);

		mpDoc->update(mShape->rect());
	}

	ResizeCommand::ResizeCommand(
		QWidget *doc, 
		QImage *image, 
		const QSize &size) 
	:
		mpDoc(doc), 
		mpImage(image), 
		mOldSize(mpDoc->size()), 
		mNewSize(size)
	{
	}

	int ResizeCommand::id() const
	{
		return ResizeCommandId;
	}

	bool ResizeCommand::mergeWith(const QUndoCommand *command)
	{
		if (command->id() != id())
			return false;

		mNewSize = static_cast<const ResizeCommand*>(command)->mNewSize;

		return true;
	}

	void ResizeCommand::undo()
	{
		*mpImage = mpImage->copy(0, 0, mOldSize.width(), mOldSize.height());

		mpDoc->update();
	}

	void ResizeCommand::redo()
	{
		if (mpImage->size() == mNewSize) 
		{
			return;
		}

		QImage newImage(mNewSize, QImage::Format_RGB32);

		newImage.fill(Qt::white);

		QPainter painter(&newImage);
		painter.drawImage(QPoint(0, 0), *mpImage);

		*mpImage = newImage;

		mpDoc->update();
	}
}
