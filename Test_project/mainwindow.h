#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_4_clicked();

    void on_GetInfo_clicked();

    void on_lineEdit_2_cursorPositionChanged(int arg1, int arg2);

    void on_ShutDown_clicked();

    void on_Statusinfo_clicked();

    void on_EventLog_clicked();

    void on_FormatRSA_clicked();

    void on_Reset_clicked();

    void on_CurrentIPAddress_clicked();

    void on_Quit_clicked();

    void on_pushButton_9_clicked();

    void on_Date_clicked();

    void on_PresentDirectory_clicked();

    void on_pushButton_clicked();

    void on_Version_clicked();

    void on_SizeofDisk_clicked();

    void on_ShowChannelFault_clicked();

    void on_ChannelsReset_clicked();

    void on_RSAMEDIA_clicked();

    void on_SaveEventLog_clicked();

    void on_BIT_clicked();

    void on_frame_customContextMenuRequested(const QPoint &pos);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_CommandsPostData_customContextMenuRequested(const QPoint &pos);

    //void on_mdiArea_subWindowActivated(QMdiSubWindow *arg1);

    void on_plainTextEdit_blockCountChanged(int newBlockCount);

    void on_frame_4_customContextMenuRequested(const QPoint &pos);

    void on_frame_3_customContextMenuRequested(const QPoint &pos);

    void on_frame_2_customContextMenuRequested(const QPoint &pos);

    void on_frame_4_windowTitleChanged(const QString &title);

    void on_frame_6_customContextMenuRequested(const QPoint &pos);

    void on_frame_5_customContextMenuRequested(const QPoint &pos);

    void on_pushButton_5_clicked();

    void on_Timeset_clicked();

    void on_mv_clicked();

    void on_remdir_clicked();

    void on_rmdirlineEdit_cursorPositionChanged(int arg1, int arg2);

    void on_SourcelineEdit_cursorPositionChanged(int arg1, int arg2);

    void on_DestnlineEdit_cursorPositionChanged(int arg1, int arg2);

    void on_timeEdit_userTimeChanged(const QTime &time);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
