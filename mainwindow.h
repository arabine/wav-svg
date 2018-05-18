#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slotImportWav();
    void slotImportSvg();
    void slotExportWav();
    void slotExportSvg();
private:
    Ui::MainWindow *ui;

    QString mInputWavFileName;
    QString mInputSvgFileName;
};

#endif // MAINWINDOW_H
