#include "VersionChecker2.h"
#include <QDir>
#include <QDesktopwidget>
#include <QDesktopServices>
#include <QTextCodec>
#include <windows.h>
#include <winnt.h>
#include<winver.h>  
VersionChecker2::VersionChecker2(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

	QDir dir;
	dir.setPath("./data");
	QFile file(dir.absoluteFilePath("VersionChecker2Class.qss"));
	file.open(QFile::ReadOnly);
	if (file.isOpen())
	{
		QString mainqss = file.readAll();
		setStyleSheet(mainqss);
		file.close();
	}

	QDesktopWidget* desktopWidget = QApplication::desktop();
	//��ȡ�豸��Ļ��С
	desktop = desktopWidget->screenGeometry();

	ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //����ѡ��
	ui.tableWidget->setColumnWidth(0, 50);
	ui.tableWidget->setColumnWidth(1, 75);
	ui.tableWidget->setColumnWidth(2, 150);
	ui.tableWidget->setColumnWidth(3, 100);
	ui.tableWidget->setColumnWidth(4, 100);
	ui.tableWidget->setColumnWidth(5, 180);
	ui.tableWidget->setColumnWidth(6, 100);
	
	initModule();

	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(showMinimized()));
	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(showAboutWidget()));
	connect(ui.comboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(onComboxChange(const QString &)));
}

VersionChecker2::~VersionChecker2()
{

}

void VersionChecker2::initModule()
{
	disconnect(ui.tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(openFilepath(int, int)));
	QDir dir;
	dir.setPath("./data");
	QFile file2(dir.absoluteFilePath("check.xml"));
	file2.open(QFile::ReadOnly);
	if (file2.isOpen())
	{
		QDomDocument doc;
		doc.setContent(&file2);
		QDomElement root = doc.documentElement();
		QDomElement teacherElement = doc.firstChildElement("S1711").firstChildElement("teacher_list"); //teacher�ڵ�  
		QDomElement studentElement = doc.firstChildElement("S1711").firstChildElement("student_list"); //student�ڵ�  
		QDomNodeList teacherNodeList = teacherElement.childNodes(); //teacher�ڵ��б�  
		QDomNodeList studentNodeList = studentElement.childNodes(); //student�ڵ��б�  
		ui.tableWidget->setRowCount(teacherNodeList.count() + studentNodeList.count());
		ui.tableWidget->verticalHeader()->setVisible(false); //�����б�ͷ  
		
		if (teacherNodeList.count() != 0)
		{
			for (int nteacherIndex = 0; nteacherIndex < teacherNodeList.count(); nteacherIndex++)//����teacher�ڵ�  
			{
				QDomNode teacherNode = teacherNodeList.at(nteacherIndex); //teacher�ڵ�  
				QString str = teacherNode.toElement().text();//�ڵ��е��ı�
				QString strNodeText = str.split(",").at(0);
				QString version = getFileVersion(strNodeText);
				QString version2 = str.split(",").at(1);
				QFileInfo fileinfo(strNodeText);
				QString name = fileinfo.fileName();
				QString lastModfyDate;
				getFileLastModifyTime(strNodeText, lastModfyDate);
				ui.tableWidget->setItem(nteacherIndex, 0, new QTableWidgetItem(QString::number(nteacherIndex+1)));
				ui.tableWidget->setItem(nteacherIndex, 1, new QTableWidgetItem(QString::fromLocal8Bit("��Ա��")));
				ui.tableWidget->setItem(nteacherIndex, 2, new QTableWidgetItem(fileinfo.fileName()));
				//�ļ��汾��Ŀ��汾����ͬ
				if (version != version2)
				{
					if (version.isEmpty())
					{
						QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("�ļ��ް汾��Ϣ"));
						item->setTextColor(QColor(220, 20, 60));
						/*item->setTextAlignment(Qt::AlignHCenter);*/
						ui.tableWidget->setItem(nteacherIndex, 3, item);
					}
					else 
					{
						QTableWidgetItem *item = new QTableWidgetItem(version);
						item->setTextColor(QColor(220, 20, 60));
						ui.tableWidget->setItem(nteacherIndex, 3, item);
					}
				}
				else
				{
					ui.tableWidget->setItem(nteacherIndex, 3, new QTableWidgetItem(version));
				}

				ui.tableWidget->setItem(nteacherIndex, 4, new QTableWidgetItem(version2));
				//����޸�����Ϊ��
				if (lastModfyDate.isEmpty())
				{
					QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("��ʱ����Ϣ"));
					item->setTextColor(QColor(220, 20, 60));
					ui.tableWidget->setItem(nteacherIndex, 5, item);
				}
				else
				{
					ui.tableWidget->setItem(nteacherIndex, 5, new QTableWidgetItem(lastModfyDate));
				}
				//�ļ��Ƿ����
				if (fileinfo.isFile())
				{
					QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("��"));
					item->setTextColor(QColor(0, 0, 255));
					ui.tableWidget->setItem(nteacherIndex, 6, item);
					item->setData(Qt::UserRole, strNodeText);
				}
				else
				{
					QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("�ļ�������"));
					item->setTextColor(QColor(220, 20, 60));
					ui.tableWidget->setItem(nteacherIndex, 6, item);
				}
				
			}
		}
		if (studentNodeList.count() != 0)
		{
			for (int nstudentIndex = 0; nstudentIndex < studentNodeList.count(); nstudentIndex++)//����teacher�ڵ�  
			{
				QDomNode teacherNode = studentNodeList.at(nstudentIndex); //teacher�ڵ�  
				QString str = teacherNode.toElement().text();//�ڵ��е��ı�
				QString strNodeText = str.split(",").at(0);
				QString version = getFileVersion(strNodeText);
				QString version2 = str.split(",").at(1);
 				QFileInfo fileinfo(strNodeText);
				QString name = fileinfo.fileName();
				QString lastModfyDate;
				getFileLastModifyTime(strNodeText, lastModfyDate);
				ui.tableWidget->setItem(nstudentIndex + teacherNodeList.count(), 0, new QTableWidgetItem(QString::number(nstudentIndex + 1 + teacherNodeList.count())));
				ui.tableWidget->setItem(nstudentIndex + teacherNodeList.count(), 1, new QTableWidgetItem(QString::fromLocal8Bit("ѧԱ��")));
				ui.tableWidget->setItem(nstudentIndex + teacherNodeList.count(), 2, new QTableWidgetItem(fileinfo.fileName()));
				if (version != version2)
				{
					if (version.isEmpty())
					{
						QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("�ļ��ް汾��Ϣ"));
						item->setTextColor(QColor(220, 20, 60));
						ui.tableWidget->setItem(nstudentIndex + teacherNodeList.count(), 3, item);
					}
					else
					{
						QTableWidgetItem *item = new QTableWidgetItem(version);
						item->setTextColor(QColor(220, 20, 60));
						ui.tableWidget->setItem(nstudentIndex + teacherNodeList.count(), 3, item);
					}
				}
				else 
				{
					ui.tableWidget->setItem(nstudentIndex + teacherNodeList.count(), 3, new QTableWidgetItem(version));
				}

				ui.tableWidget->setItem(nstudentIndex + teacherNodeList.count(), 4, new QTableWidgetItem(version2));
				
				if (lastModfyDate.isEmpty())
				{
					QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("��ʱ����Ϣ"));
					item->setTextColor(QColor(220, 20, 60));
					ui.tableWidget->setItem(nstudentIndex + teacherNodeList.count(), 5, item);
				}
				else 
				{
					ui.tableWidget->setItem(nstudentIndex + teacherNodeList.count(), 5, new QTableWidgetItem(lastModfyDate));
				}

				if (fileinfo.isFile())
				{
					QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("��"));
					item->setTextColor(QColor(0, 0, 255));
					ui.tableWidget->setItem(nstudentIndex + teacherNodeList.count(), 6, item);
					item->setData(Qt::UserRole, strNodeText);
				}
				else
				{
					QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("�ļ�������"));
					item->setTextColor(QColor(220, 20, 60));
					ui.tableWidget->setItem(nstudentIndex + teacherNodeList.count(), 6, item);
				}
			}
		}
		ui.label_3->setText(QString::fromLocal8Bit("  ����� %1 ����¼").arg(teacherNodeList.count()+studentNodeList.count()));
		file2.close();

		for (size_t i = 0; i < (teacherNodeList.count() + studentNodeList.count()); i++)
		{
			for (size_t j = 0; j < ui.tableWidget->columnCount(); j++)
			{
				ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter| Qt::AlignVCenter);
			}			
		}
	}
	connect(ui.tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(openFilepath(int, int)));
}

void VersionChecker2::showTeacherModule()
{
	QDir dir;
	dir.setPath("./data");
	QFile file2(dir.absoluteFilePath("check.xml"));
	file2.open(QFile::ReadOnly);
	if (file2.isOpen())
	{
		QDomDocument doc;
		doc.setContent(&file2);
		QDomElement root = doc.documentElement();
		QDomElement teacherElement = doc.firstChildElement("S1711").firstChildElement("teacher_list"); //teacher�ڵ�  
		QDomNodeList teacherNodeList = teacherElement.childNodes(); //teacher�ڵ��б�  
		ui.tableWidget->setRowCount(teacherNodeList.count());
		ui.tableWidget->verticalHeader()->setVisible(false); //�����б�ͷ  

		if (teacherNodeList.count() != 0)
		{
			for (int nteacherIndex = 0; nteacherIndex < teacherNodeList.count(); nteacherIndex++)//����teacher�ڵ�  
			{
				QDomNode teacherNode = teacherNodeList.at(nteacherIndex); //teacher�ڵ�  
				QString str = teacherNode.toElement().text();//�ڵ��е��ı�
				QString strNodeText = str.split(",").at(0);
				QString version = getFileVersion(strNodeText);
				QString version2 = str.split(",").at(1);
				QFileInfo fileinfo(strNodeText);
				QString name = fileinfo.fileName();
				QString lastModfyDate;
				getFileLastModifyTime(strNodeText, lastModfyDate);
				ui.tableWidget->setItem(nteacherIndex, 0, new QTableWidgetItem(QString::number(nteacherIndex + 1)));
				ui.tableWidget->setItem(nteacherIndex, 1, new QTableWidgetItem(QString::fromLocal8Bit("��Ա��")));
				ui.tableWidget->setItem(nteacherIndex, 2, new QTableWidgetItem(fileinfo.fileName()));
				//�ļ��汾��Ŀ��汾����ͬ
				if (version != version2)
				{
					if (version.isEmpty())
					{
						QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("�ļ��ް汾��Ϣ"));
						item->setTextColor(QColor(220, 20, 60));
						ui.tableWidget->setItem(nteacherIndex, 3, item);
					}
					else
					{
						QTableWidgetItem *item = new QTableWidgetItem(version);
						item->setTextColor(QColor(220, 20, 60));
						ui.tableWidget->setItem(nteacherIndex, 3, item);
					}
				}
				else
				{
					ui.tableWidget->setItem(nteacherIndex, 3, new QTableWidgetItem(version));
				}

				ui.tableWidget->setItem(nteacherIndex, 4, new QTableWidgetItem(version2));
				//����޸�����Ϊ��
				if (lastModfyDate.isEmpty())
				{
					QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("��ʱ����Ϣ"));
					item->setTextColor(QColor(220, 20, 60));
					ui.tableWidget->setItem(nteacherIndex, 5, item);
				}
				else
				{
					ui.tableWidget->setItem(nteacherIndex, 5, new QTableWidgetItem(lastModfyDate));
				}
				//�ļ��Ƿ����
				if (fileinfo.isFile())
				{
					QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("��"));
					item->setTextColor(QColor(0, 0, 255));
					ui.tableWidget->setItem(nteacherIndex, 6, item);
					item->setData(Qt::UserRole, strNodeText);
				}
				else
				{
					QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("�ļ�������"));
					item->setTextColor(QColor(220, 20, 60));
					ui.tableWidget->setItem(nteacherIndex, 6, item);
				}
			}
		}
		ui.label_3->setText(QString::fromLocal8Bit("  ����� %1 ����¼").arg(teacherNodeList.count()));
		file2.close();

		for (size_t i = 0; i < (teacherNodeList.count()); i++)
		{
			for (size_t j = 0; j < ui.tableWidget->columnCount(); j++)
			{
				ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			}
		}
	}
}

void VersionChecker2::showStudentModule()
{
	QDir dir;
	dir.setPath("./data");
	QFile file2(dir.absoluteFilePath("check.xml"));
	file2.open(QFile::ReadOnly);
	if (file2.isOpen())
	{
		QDomDocument doc;
		doc.setContent(&file2);
		QDomElement root = doc.documentElement();
		QDomElement teacherElement = doc.firstChildElement("S1711").firstChildElement("teacher_list"); //teacher�ڵ�  
		QDomElement studentElement = doc.firstChildElement("S1711").firstChildElement("student_list"); //student�ڵ�  
		QDomNodeList teacherNodeList = teacherElement.childNodes(); //teacher�ڵ��б�  
		QDomNodeList studentNodeList = studentElement.childNodes(); //student�ڵ��б�  
		ui.tableWidget->setRowCount(studentNodeList.count());
		ui.tableWidget->verticalHeader()->setVisible(false); //�����б�ͷ  

		if (studentNodeList.count() != 0)
		{
			for (int nstudentIndex = 0; nstudentIndex < studentNodeList.count(); nstudentIndex++)//����teacher�ڵ�  
			{
				QDomNode teacherNode = studentNodeList.at(nstudentIndex); //teacher�ڵ�  
				QString str = teacherNode.toElement().text();//�ڵ��е��ı�
				QString strNodeText = str.split(",").at(0);
				QString version = getFileVersion(strNodeText);
				QString version2 = str.split(",").at(1);
				QFileInfo fileinfo(strNodeText);
				QString name = fileinfo.fileName();
				QString lastModfyDate;
				getFileLastModifyTime(strNodeText, lastModfyDate);
				ui.tableWidget->setItem(nstudentIndex, 0, new QTableWidgetItem(QString::number(nstudentIndex + 1)));
				ui.tableWidget->setItem(nstudentIndex, 1, new QTableWidgetItem(QString::fromLocal8Bit("ѧԱ��")));
				ui.tableWidget->setItem(nstudentIndex, 2, new QTableWidgetItem(fileinfo.fileName()));
				if (version != version2)
				{
					if (version.isEmpty())
					{
						QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("�ļ��ް汾��Ϣ"));
						item->setTextColor(QColor(220, 20, 60));
						ui.tableWidget->setItem(nstudentIndex , 3, item);
					}
					else
					{
						QTableWidgetItem *item = new QTableWidgetItem(version);
						item->setTextColor(QColor(220, 20, 60));
						ui.tableWidget->setItem(nstudentIndex , 3, item);
					}
				}
				else
				{
					ui.tableWidget->setItem(nstudentIndex , 3, new QTableWidgetItem(version));
				}

				ui.tableWidget->setItem(nstudentIndex , 4, new QTableWidgetItem(version2));

				if (lastModfyDate.isEmpty())
				{
					QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("��ʱ����Ϣ"));
					item->setTextColor(QColor(220, 20, 60));
					ui.tableWidget->setItem(nstudentIndex , 5, item);
				}
				else
				{
					ui.tableWidget->setItem(nstudentIndex , 5, new QTableWidgetItem(lastModfyDate));
				}

				if (fileinfo.isFile())
				{
					QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("��"));
					item->setTextColor(QColor(0, 0, 255));
					ui.tableWidget->setItem(nstudentIndex, 6, item);
					item->setData(Qt::UserRole, strNodeText);
				}
				else
				{
					QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("�ļ�������"));
					item->setTextColor(QColor(220, 20, 60));
					ui.tableWidget->setItem(nstudentIndex, 6, item);
				}
			}
		}
		ui.label_3->setText(QString::fromLocal8Bit("  ����� %1 ����¼").arg(studentNodeList.count()));
		file2.close();
		for (size_t i = 0; i < (studentNodeList.count()); i++)
		{
			for (size_t j = 0; j < ui.tableWidget->columnCount(); j++)
			{
				ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			}
		}
	}
}

void VersionChecker2::onComboxChange(const QString &text)
{
	ui.tableWidget->clearContents();
	if(text == QString::fromLocal8Bit("ȫ��"))
	{
		initModule();
	}
	if(text == QString::fromLocal8Bit("��Ա��"))
	{
		showTeacherModule();
	}
	if(text == QString::fromLocal8Bit("ѧԱ��"))
	{
		showStudentModule();
	}
}

QString VersionChecker2::getFileVersion(QString fullName)
{
#pragma comment(lib, "Version")  
	QString result = "";
	char* pData = nullptr;
	do
	{
		DWORD dwLen = GetFileVersionInfoSize(fullName.toStdWString().c_str(), 0);
		if (0 == dwLen)
		{
			break;
		}
		pData = new char[dwLen + 1];
		BOOL bSuccess = GetFileVersionInfo(fullName.toStdWString().c_str(), 0, dwLen, pData);
		if (!bSuccess)
		{
			break;
		}
		struct LANGANDCODEPAGE
		{
			WORD wLanguage;
			WORD wCodePage;
		} *lpTranslate;
		LPVOID lpBuffer = nullptr;
		UINT uLen = 0;
		bSuccess = VerQueryValue(pData, (TEXT("\\VarFileInfo\\Translation")), (LPVOID*)&lpTranslate, &uLen);
		if (!bSuccess)
		{
			break;
		}
		QString str1, str2;
		str1.setNum(lpTranslate->wLanguage, 16);
		str2.setNum(lpTranslate->wCodePage, 16);
		str1 = "000" + str1;
		str2 = "000" + str2;
		QString verPath = "\\StringFileInfo\\" + str1.right(4) + str2.right(4) + "\\FileVersion";
		bSuccess = VerQueryValue(pData, (verPath.toStdWString().c_str()), &lpBuffer, &uLen);
		if (!bSuccess)
		{
			break;
		}
		result = QString::fromUtf16((const unsigned short int *)lpBuffer);
	} while (false);
	if (nullptr != pData)
	{
		delete pData;
	}
	return result;
}

bool VersionChecker2::getFileLastModifyTime(const QString &fullName, QString &v_cstrLastModifyTime)
{
	std::string FileName = fullName.toStdString();
	std::wstring wstr(FileName.length(), L' ');//��ʼ�����ֽ�wstr
	std::copy(FileName.begin(), FileName.end(), wstr.begin());//��str���Ƶ�wstr
	LPCWSTR v_cstrFileName = wstr.c_str();//��wstrת��ΪC�ַ�����ָ��,Ȼ��ֵ��path

	BOOL  bResult = FALSE;
	FILETIME ftCreationTime, ftLastAccessTime, ftLastWriteTime;
	SYSTEMTIME stTemp, stSTime;
	HANDLE hSrcFile = INVALID_HANDLE_VALUE;

	hSrcFile = CreateFile(v_cstrFileName, GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (INVALID_HANDLE_VALUE == hSrcFile)
	{
		return bResult;
	}


	if (0 == GetFileTime(hSrcFile, &ftCreationTime, &ftLastAccessTime, &ftLastWriteTime))
	{
		//theApp.m_Log.AddMsg("[ArWs]GetFileLastModifyTime fial.<%d>",GetLastError() );  
	}
	else
	{
		FileTimeToSystemTime(&ftLastWriteTime, &stTemp);
		SystemTimeToTzSpecificLocalTime(NULL, &stTemp, &stSTime);
		v_cstrLastModifyTime = QString::number(stSTime.wYear)+ QString::fromLocal8Bit("��")+ QString::number(stSTime.wMonth)+QString::fromLocal8Bit("��") + QString::number(stSTime.wDay)+ QString::fromLocal8Bit("��") + QString::number(stSTime.wHour) +":"+ QString::number(stSTime.wMinute) +":"+ QString::number(stSTime.wSecond);
		bResult = TRUE;
	}

	CloseHandle(hSrcFile);

	return bResult;
}

void VersionChecker2::openFilepath(int row, int column)
{
	if (column != 6)
	{
		return;
	}
	QTableWidget *_widget = (QTableWidget *)sender();
	if (_widget)
	{
		QVector<QMap<QString, QString> > tableData;
		QString file_path = ui.tableWidget->item(row, 6)->data(Qt::UserRole).toString();
		QFileInfo fileinfo(file_path);
		if (!fileinfo.isFile())
		{
			return;
		}
		QString argsStr = "/select, " + file_path.replace("/", "\\"); //�滻�ļ�Ŀ¼�ָ���ʽ  
		QTextCodec *codec = QTextCodec::codecForName("GB18030"); //windowsĬ��ʹ��GBK����,qtĬ��ʹ��UTF-8����,��Ҫת��  
		ShellExecuteA(NULL, "open", "explorer", codec->fromUnicode(argsStr).constData(), NULL, SW_SHOWDEFAULT);//��UTF-8ת��ΪGBK��ת��Ϊconst char* ����  
	}
}

void VersionChecker2::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		this->m_drag = true;
		this->dragPos = event->pos();
		this->resizeDownPos = event->globalPos();
		this->mouseDownRect = this->rect();
	}
}
void VersionChecker2::mouseMoveEvent(QMouseEvent * event)
{
	if (resizeRegion != Default)
	{
		handleResize();
		return;
	}
	if (m_move) {
		move(event->globalPos() - dragPos);
		return;
	}
	QPoint clientCursorPos = event->pos();
	QRect r = this->rect();
	QRect resizeInnerRect(resizeBorderWidth, resizeBorderWidth, r.width() - 2 * resizeBorderWidth, r.height() - 2 * resizeBorderWidth);
	if (r.contains(clientCursorPos) && !resizeInnerRect.contains(clientCursorPos)) { //���������С
		ResizeRegion resizeReg = getResizeRegion(clientCursorPos);
		setResizeCursor(resizeReg);
		if (m_drag && (event->buttons() & Qt::LeftButton)) {
			resizeRegion = resizeReg;
			handleResize();
		}
	}
	else { //�ƶ�����
		setCursor(Qt::ArrowCursor);
		if (m_drag && (event->buttons() & Qt::LeftButton)) {
			m_move = true;
			move(event->globalPos() - dragPos);
		}
	}
}
void VersionChecker2::mouseReleaseEvent(QMouseEvent *event)
{
	m_drag = false;
	if (m_move) {
		m_move = false;
		handleMove(event->globalPos()); //���ſ�����������Ļ�����Զ���������Ļ����/���/�Ҳ�
	}
	resizeRegion = Default;
	setCursor(Qt::ArrowCursor);
}
void VersionChecker2::setResizeCursor(ResizeRegion region)
{
	switch (region)
	{
	case North:
	case South:
		setCursor(Qt::SizeVerCursor);
		break;
	case East:
	case West:
		setCursor(Qt::SizeHorCursor);
		break;
	case NorthWest:
	case SouthEast:
		setCursor(Qt::SizeFDiagCursor);
		break;
	default:
		setCursor(Qt::SizeBDiagCursor);
		break;
	}
}
ResizeRegion VersionChecker2::getResizeRegion(QPoint clientPos)
{
	if (clientPos.y() <= resizeBorderWidth) {
		if (clientPos.x() <= resizeBorderWidth)
			return NorthWest;
		else if (clientPos.x() >= this->width() - resizeBorderWidth)
			return NorthEast;
		else
			return North;
	}
	else if (clientPos.y() >= this->height() - resizeBorderWidth) {
		if (clientPos.x() <= resizeBorderWidth)
			return SouthWest;
		else if (clientPos.x() >= this->width() - resizeBorderWidth)
			return SouthEast;
		else
			return South;
	}
	else {
		if (clientPos.x() <= resizeBorderWidth)
			return West;
		else
			return East;
	}
}
void VersionChecker2::handleMove(QPoint pt)
{
	QPoint currentPos = pt - dragPos;
	if (currentPos.x() < desktop.x()) { //��������Ļ���
		currentPos.setX(desktop.x());
	}
	else if (currentPos.x() + this->width() > desktop.width()) { //��������Ļ�Ҳ�
		currentPos.setX(desktop.width() - this->width());
	}
	if (currentPos.y() < desktop.y()) { //��������Ļ����
		currentPos.setY(desktop.y());
	}
	move(currentPos);
}
void VersionChecker2::handleResize()
{
	int xdiff = QCursor::pos().x() - resizeDownPos.x();
	int ydiff = QCursor::pos().y() - resizeDownPos.y();
	switch (resizeRegion)
	{
	case East:
	{
		resize(mouseDownRect.width() + xdiff, this->height());
		break;
	}
	case West:
	{
		resize(mouseDownRect.width() - xdiff, this->height());
		move(resizeDownPos.x() + xdiff, this->y());
		break;
	}
	case South:
	{
		resize(this->width(), mouseDownRect.height() + ydiff);
		break;
	}
	case North:
	{
		resize(this->width(), mouseDownRect.height() - ydiff);
		move(this->x(), resizeDownPos.y() + ydiff);
		break;
	}
	case SouthEast:
	{
		resize(mouseDownRect.width() + xdiff, mouseDownRect.height() + ydiff);
		break;
	}
	case NorthEast:
	{
		resize(mouseDownRect.width() + xdiff, mouseDownRect.height() - ydiff);
		move(this->x(), resizeDownPos.y() + ydiff);
		break;
	}
	case NorthWest:
	{
		resize(mouseDownRect.width() - xdiff, mouseDownRect.height() - ydiff);
		move(resizeDownPos.x() + xdiff, resizeDownPos.y() + ydiff);
		break;
	}
	case SouthWest:
	{
		resize(mouseDownRect.width() - xdiff, mouseDownRect.height() + ydiff);
		move(resizeDownPos.x() + xdiff, this->y());
		break;
	}
	}
}


