#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPrinter>

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
    // File menu
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void save();
    void saveAs();
    void print();
    void printPreviewDialog();
    void printPreview(QPrinter *printer);
    void exportPdf();

    // Edit menu
    void undo();
    void redo();
    void copy();
    void cut();
    void paste();

    // Format menu
    void fontBold();
    void fontItalic();
    void fontUnderline();

    void right();
    void left();
    void center();
    void justify();

    void changeFont();
    void changeColor();
    void about();




private:
    Ui::MainWindow *ui;
    QString currentFile;

};
#endif // MAINWINDOW_H
