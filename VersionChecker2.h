#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_VersionChecker2.h"
#include<QMouseEvent>
#include<QDomDocument>
enum ResizeRegion
{
	Default,
	North,
	NorthEast,
	East,
	SouthEast,
	South,
	SouthWest,
	West,
	NorthWest
};


class VersionChecker2 : public QMainWindow
{
	Q_OBJECT
public:

	VersionChecker2(QWidget *parent = Q_NULLPTR);
	~VersionChecker2();

	void initModule();
	void showTeacherModule();
	void showStudentModule();

	QString getFileVersion(QString);
	bool getFileLastModifyTime(const QString &, QString &);

	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void setResizeCursor(ResizeRegion region);
	void handleMove(QPoint pt);
	void handleResize();
	ResizeRegion getResizeRegion(QPoint clientPos);

	public slots:
	void onComboxChange(const QString &);
	void openFilepath(int row,int column);
private:
	Ui::VersionChecker2Class ui;
	bool m_drag, m_move;
	QPoint dragPos, resizeDownPos;
	const int resizeBorderWidth = 5;
	ResizeRegion resizeRegion;
	QRect mouseDownRect;
	QRect desktop;
};
