#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

// QT
#include <QtGui/QPainter>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLayout>
#include <QUndoStack>


#include "PaintWidget.h"


namespace Paint 
{

	class MainWindow : public QMainWindow
	{
			Q_OBJECT

		public:

			MainWindow(QWidget *parent = Q_NULLPTR);

			~MainWindow();

		protected:

		// UI

			void CreateToolBar();

			QWidget* CreatePenColorWidget();
			QWidget* CreatePenSizeWidget();
			QWidget* CreateTypeWidget();
			QWidget* CreateCommandWidget();

		// Helper methods

			void SetPenColor(
				QList<QPushButton*>& items, 
				int index, 
				QWidget& colorCell);

			void SetPenWidth(int width);

		protected:

			QToolBar*		mpToolBar = nullptr;

			QUndoStack		mUndoStack;
			PaintWidget		mPaintWidget;
	};

}

#endif // UI_MAINWINDOW_H