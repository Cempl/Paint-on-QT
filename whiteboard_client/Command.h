#ifndef COMMAND_H
#define COMMAND_H

#include "shape.h"

// QT
#include <QImage>
#include <QWidget>
#include <QUndoCommand>

// STL
#include <memory>
#include <vector>


namespace Paint
{

	/**
	 * Here we have all commands to be used for undo/redo support.
	 */

	class ShapeCommand : public QUndoCommand
	{
		public:

			explicit ShapeCommand(
						QWidget* doc,
						QImage* image,
						std::unique_ptr<Shape>&& s);

			virtual void undo() override;
			virtual void redo() override;

		private:

			QWidget* mpDoc;
			QImage* mpImage;

			QImage mUndoImage;
			std::unique_ptr<Shape> mShape;
	};


	class ResizeCommand : public QUndoCommand
	{
		public:

			explicit ResizeCommand(
						QWidget *mpDoc, 
						QImage *image, 
						const QSize &size);

			virtual int id() const override;
			virtual bool mergeWith(const QUndoCommand *command) override;

			virtual void undo() override;
			virtual void redo() override;

		private:

			QWidget* mpDoc;
			QImage* mpImage;

			QSize mOldSize;
			QSize mNewSize;
	};
}
#endif // COMMAND_H
