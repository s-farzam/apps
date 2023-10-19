#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
//#include <QPrinter>
#include <QPrintDialog>  
#include <QPrintPreviewDialog>
#include <QApplication>
#include <QFontDialog>
#include <QColorDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // File menu
//    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
    connect(ui->actionSave_as, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::print);
    connect(ui->actionPrint_preview, &QAction::triggered, this, &MainWindow::printPreviewDialog);
    connect(ui->actionExport_PDF, &QAction::triggered, this, &MainWindow::exportPdf);
    connect(ui->actionExit, &QAction::triggered, this, &QApplication::quit);

    // Edit menu
    connect(ui->actionUndo, &QAction::triggered, this,  &MainWindow::undo);
    connect(ui->actionRedo, &QAction::triggered, this,  &MainWindow::redo);
    connect(ui->actionCopy, &QAction::triggered, this,  &MainWindow::copy);
    connect(ui->actionCut, &QAction::triggered, this,  &MainWindow::cut);
    connect(ui->actionPaste, &QAction::triggered, this,  &MainWindow::paste);

    // Format menu
    connect(ui->actionBold, &QAction::triggered, this,  &MainWindow::fontBold);
    connect(ui->actionItalic, &QAction::triggered, this,  &MainWindow::fontItalic);
    connect(ui->actionUnderline, &QAction::triggered, this,  &MainWindow::fontUnderline);

    connect(ui->actionRight, &QAction::triggered, this,  &MainWindow::right);
    connect(ui->actionLeft, &QAction::triggered, this,  &MainWindow::left);
    connect(ui->actionCenter, &QAction::triggered, this,  &MainWindow::center);
    connect(ui->actionJustify, &QAction::triggered, this,  &MainWindow::justify);

    connect(ui->actionFont, &QAction::triggered, this,  &MainWindow::changeFont);
    connect(ui->actionColor, &QAction::triggered, this,  &MainWindow::changeColor);
    connect(ui->actionAbout, &QAction::triggered, this,  &MainWindow::about);



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    currentFile.clear();
    ui->textEdit->setText(QString());
}


void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file name");
    QFile file(fileName);
    currentFile = fileName;

    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Notepad cannot open the file. \nError: " + file.errorString());
        return;
    }

    // Set the window title.
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();

}

void MainWindow::save()
{
    QString fileName;
    if (currentFile.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this, "Save");
        currentFile = fileName;
    }
    else {
        fileName = currentFile;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Can not save the file. \nError: " + file.errorString());
        return; 
    }

    setWindowTitle(fileName);
    QTextStream out(&file);
    // Get text from the textEdit.
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Can not save the file. \nError: " + file.errorString());
        return;
    }

    currentFile = fileName;
    setWindowTitle(fileName);
    QTextStream out(&file);
    // Get text from the textEdit.
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void MainWindow::print()
{
    QPrinter printDev;
    QPrintDialog dialog(&printDev, this);
    if (dialog.exec() == QDialog::Rejected)
        return;
    ui->textEdit->print(&printDev);

}

void MainWindow::printPreviewDialog()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    preview.setWindowFlags(Qt::Window);
    connect(&preview, SIGNAL(paintRequested(QPrinter *)), SLOT(printPreview(QPrinter *)));
    preview.exec();  // Execute the preview.
}

void MainWindow::printPreview(QPrinter *printer)
{
    ui->textEdit->print(printer);
}

void MainWindow::exportPdf()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export PDF");

    if (fileName != ' ') {  // Not empty.
        QPrinter printer(QPrinter::PrinterResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        printer.setPageMargins(QMarginsF(30, 30, 30, 30));
        ui->textEdit->document()->print(&printer);  // Converts text to PDF.
    }

}

// Edit menu
void MainWindow::undo()
{
    ui->textEdit->undo();
}

void MainWindow::redo()
{
    ui->textEdit->redo();
}

void MainWindow::copy()
{
    ui->textEdit->copy();
}

void MainWindow::cut()
{
    ui->textEdit->cut();
}

void MainWindow::paste()
{
    ui->textEdit->paste();
}

// Format menu
void MainWindow::fontBold()
{
    QFont font;
    font.setBold(true);
    ui->textEdit->setFont(font);
}

void MainWindow::fontItalic()
{
    QFont font;
    font.setItalic(true);
    ui->textEdit->setFont(font);
}

void MainWindow::fontUnderline()
{
    QFont font;
    font.setUnderline(true);
    ui->textEdit->setFont(font);
}

void MainWindow::right()
{
    ui->textEdit->setAlignment(Qt::AlignmentFlag::AlignRight);
}

void MainWindow::left()
{
    ui->textEdit->setAlignment(Qt::AlignmentFlag::AlignLeft);
}

void MainWindow::center()
{
    ui->textEdit->setAlignment(Qt::AlignmentFlag::AlignCenter);
}

void MainWindow::justify()
{
    ui->textEdit->setAlignment(Qt::AlignmentFlag::AlignJustify);
}

void MainWindow::changeFont()
{
    bool okay;
    QFont font = QFontDialog::getFont(&okay, this);
    ui->textEdit->setFont(font);
}

void MainWindow::changeColor()
{
    QColorDialog colorDialog;
    QColor color = colorDialog.getColor();
    ui->textEdit->setTextColor(color);
}

void MainWindow::about()
{
    QMessageBox::about(this, "About the application", "Version 1.0");
}
