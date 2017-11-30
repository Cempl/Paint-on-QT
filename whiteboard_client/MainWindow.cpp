#include <QList>

#include "MainWindow.h"
#include "PaintWidget.h"
#include "Shape.h"


namespace Paint
{
	MainWindow::MainWindow(QWidget *parent)
	:
		QMainWindow(parent),
		mPaintWidget(&mUndoStack)
	{
		// Paint Widget settings
		mPaintWidget.setObjectName(QStringLiteral("paintWidget"));
		mPaintWidget.SetPenColor(QColor("#dc0808"));
		mPaintWidget.SetPenWidth(3);
		mPaintWidget.SetShapeFactory(CreateLine);

		setCentralWidget(&mPaintWidget);

		CreateToolBar();

		if (objectName().isEmpty())
			setObjectName(QStringLiteral("MainWindow"));

		setWindowTitle(QApplication::translate("MainWindow", "WhiteBoard - Client", 0));

		resize(1000, 800);
	}

	MainWindow::~MainWindow()
	{
		mPaintWidget.deleteLater();
	}

	void MainWindow::CreateToolBar()
	{
		QGridLayout* mainGrid = new QGridLayout(this);

		mainGrid->addWidget(CreatePenColorWidget(), 0, 0, Qt::AlignLeft);
		mainGrid->addWidget(CreateTypeWidget(), 1, 0, Qt::AlignLeft);

		// Toolbar setting
		mpToolBar = addToolBar("mainToolBar");

		addToolBar(mpToolBar);

		mpToolBar->setFloatable(false);
		mpToolBar->setMovable(false);
		mpToolBar->setObjectName("mainToolBar");

		QWidget* mainWidget = new QWidget(this);

		mainWidget->setLayout(mainGrid);

		mpToolBar->addWidget(mainWidget);
	}

	QWidget* MainWindow::CreatePenColorWidget()
	{
		QLabel* label = new QLabel(this);

		const QString labelStyle = "font-size: 12px;";

		label->setText("Pen Color: ");
		label->setStyleSheet(labelStyle);

		QWidget* currColor = new QWidget(this);

		QString cn = mPaintWidget.GetPenColor().name();

		currColor->setFixedWidth(55);
		currColor->setFixedHeight(55);
		currColor->setStyleSheet(QString("background-color: %1; ").arg(cn));

		const int width = 25;
		const int height = 25;
		const int countItems = 16;
		const int rowCount = 2;

		QStringList styles;

		double h = 0;
		double ratio = 0.918033988749895;
		for (int i = 0; i < countItems; i++)
		{
			h = ratio * 360 / countItems * i;
			const QString cn = QColor::fromHsv(int(h), 245, 220, 255).name();
			styles.push_back(QString("background-color: %1;").arg(cn));
		}
		
		QList<QPushButton*>* buttons = new QList<QPushButton *>();

		QGridLayout* cl = new QGridLayout(this);

		for (int i = 0, column = 0, row = 0; i < countItems; i++, column++)
		{
			buttons->push_back( new QPushButton( this ) );

			buttons->at(i)->setFixedWidth(width);
			buttons->at(i)->setFixedHeight(height);
			buttons->at(i)->setStyleSheet(styles[i]);

			if (column == countItems / rowCount)
			{
				column = 0;
				++row;
			}

			cl->addWidget(buttons->at(i), row, column);
			
			connect(buttons->at(i), &QPushButton::clicked, this, [=] {SetPenColor(*buttons, i, *currColor); });
		}

		QWidget* wc = new QWidget(this);

		wc->setLayout(cl);

		QHBoxLayout *layout = new QHBoxLayout(this);

		layout->addWidget(label);
		layout->addWidget(currColor);
		layout->addWidget(wc);
		layout->addWidget(CreatePenSizeWidget());

		QWidget* res = new QWidget(this);

		res->setLayout(layout);

		return res;
	}

	QWidget* MainWindow::CreatePenSizeWidget()
	{
		QLabel* label = new QLabel(this);

		const QString labelStyle = "font-size: 12px;";

		label->setText("Pen Size: ");
		label->setStyleSheet(labelStyle);
		label->setFixedWidth(50);

		QSlider* slider = new QSlider(Qt::Horizontal, this);
		QSpinBox* spinBox = new QSpinBox(this);

		slider->setFixedWidth(100);
		slider->setRange(1, 10);
		slider->setValue(3);

		spinBox->setRange(1, 10);
		spinBox->setFixedWidth(35);
		spinBox->setValue(3);

		connect(spinBox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
		connect(slider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
		connect(slider, static_cast<void (QSlider::*)(int index)>(&QSlider::valueChanged), this, [&](int w){ SetPenWidth(w); });

		QHBoxLayout *layout = new QHBoxLayout(this);

		layout->addWidget(label);
		layout->addWidget(slider);
		layout->addWidget(spinBox);

		QWidget* res = new QWidget(this);

		res->setLayout(layout);

		return res;
	}

	QWidget* MainWindow::CreateTypeWidget()
	{
		QLabel* label = new QLabel(this);

		const QString labelStyle = "font-size: 12px;";

		label->setFixedWidth(80);
		label->setText("Shape Type: ");
		label->setStyleSheet(labelStyle);

		QComboBox* box = new QComboBox(this);

		QStringList items{ "Line", "Curve", "Rectangle", "Ellipse" };

		box->addItems(items);
		box->setCurrentIndex(0);

		auto changeObject = [=](int i)
		{
			switch (i)
			{
				case 0: mPaintWidget.SetShapeFactory(CreateLine);		break;
				case 1: mPaintWidget.SetShapeFactory(CreateCurve);		break;
				case 2: mPaintWidget.SetShapeFactory(CreateRectangle);	break;
				case 3: mPaintWidget.SetShapeFactory(CreateEllipse);	break;
			}
		};

		connect(box, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged), this, changeObject);

		QHBoxLayout *layout = new QHBoxLayout(this);

		layout->addWidget(label);
		layout->addWidget(box);
		layout->addWidget(CreateCommandWidget());

		QWidget* res = new QWidget(this);

		res->setLayout(layout);

		return res;
	}

	QWidget* MainWindow::CreateCommandWidget()
	{
		// Command Undo
		QPushButton* undo = new QPushButton(this);

		undo->setText("Undo");
		undo->setShortcut(QKeySequence::Undo);

		connect(undo, &QPushButton::clicked, this, [&]
		{ 
			mUndoStack.undo(); 

			Counter::getInstance().mCommandCounter++; 
		} );

		// Command Redo
		QPushButton* redo = new QPushButton(this);

		redo->setText("Redo");
		redo->setShortcut(QKeySequence::Redo);

		connect(redo, &QPushButton::clicked, this, [&]
		{
			mUndoStack.redo(); 

			Counter::getInstance().mCommandCounter++; 
		});

		// Command Cleanup
		QPushButton* clean = new QPushButton(this);

		clean->setText("Cleanup");

		auto DoClear = [&]
		{
			while( Counter::getInstance().mCommandCounter-- )
				   mUndoStack.undo();
			
			int count = mUndoStack.index();
			while( count-- )
				   mUndoStack.undo();

			Counter::getInstance().mCommandCounter = 0;

			mUndoStack.clear();
		};

		connect(clean, &QPushButton::clicked, this, DoClear);

		QHBoxLayout *layout = new QHBoxLayout(this);

		layout->addWidget(undo);
		layout->addWidget(redo);
		layout->addWidget(clean);

		QWidget* res = new QWidget(this);

		res->setLayout(layout);

		return res;
	}

	void MainWindow::SetPenColor(
		QList<QPushButton*>& items, 
		int index, 
		QWidget& colorCell)
	{
		QColor c = items[index]->palette().color(QPalette::Button);
		if (c.isValid())
		{
			mPaintWidget.SetPenColor(c);

			const QString cn = c.name();

			colorCell.setStyleSheet(QString("background-color: %1; ").arg(cn));
			colorCell.update();
		}
	}

	void MainWindow::SetPenWidth(int width)
	{
		mPaintWidget.SetPenWidth(width);
	}
}