#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>

#include "Converter.h"

#define DRAWSOUND_VERSION "1.0.0"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("DrawSound " DRAWSOUND_VERSION);

    connect(ui->pushButtonImportWav, &QPushButton::clicked, this, &MainWindow::slotImportWav);
    connect(ui->pushButtonImportSvg, &QPushButton::clicked, this, &MainWindow::slotImportSvg);
    connect(ui->pushButtonExportWav, &QPushButton::clicked, this, &MainWindow::slotExportWav);
    connect(ui->pushButtonExportSvg, &QPushButton::clicked, this, &MainWindow::slotExportSvg);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotImportWav()
{
    mInputWavFileName = QFileDialog::getOpenFileName(this,
          tr("Open WAV"), "", tr("Image Files (*.wav)"));

    QFileInfo inf(mInputWavFileName);
    ui->labelFileNameWav->setText(inf.fileName());
}

void MainWindow::slotImportSvg()
{
    mInputSvgFileName = QFileDialog::getOpenFileName(this,
          tr("Open SVG"), "", tr("Image Files (*.svg)"));

    QFileInfo inf(mInputSvgFileName);
    ui->labelFileNameSvg->setText(inf.fileName());
}

void MainWindow::slotExportWav()
{
    QString outputFileName = QFileDialog::getSaveFileName(this, tr("Export to WAV"),
                                                    "",
                                                    tr("Images (*.wav)"));
    int bitDepth = 16;
    uint32_t sampleRate = 44100;
    if (ui->radioButton44K->isChecked())
    {
        sampleRate = 44100;
    }
    else if(ui->radioButton48K->isChecked())
    {
        sampleRate = 48000;
    }
    else
    {
        sampleRate = 96000;
    }

    if (ui->radioButton16b->isChecked())
    {
        bitDepth = 16;
    }
    else if(ui->radioButton24b->isChecked())
    {
        bitDepth = 24;
    }
    else
    {
        bitDepth = 32;
    }

    SvgToWav(mInputSvgFileName.toStdString(), outputFileName.toStdString(), sampleRate, bitDepth);
}

void MainWindow::slotExportSvg()
{
    QString outputFileName = QFileDialog::getSaveFileName(this, tr("Export to SVG"),
                                                     "",
                                                     tr("Images (*.svg)"));

    WavToSvg(mInputWavFileName.toStdString(), outputFileName.toStdString());
}
