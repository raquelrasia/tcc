#ifndef MYCOURSEDIALOG_H
#define MYCOURSEDIALOG_H

#include <QDialog>
#include <QtXml>
#include <QDate>

namespace Ui {
class MyCourseDialog;
}

class MyCourseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MyCourseDialog(QWidget *parent = 0, QString xml_path = " ");
    ~MyCourseDialog();

    QString get_current_course() ;
    QString get_current_class() ;
    bool get_dialog_ended() ;
    QString get_path() ;
    QString get_filename() ;
    QString get_file_extension() ;

private slots:

    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_listWidget_itemSelectionChanged();


    void on_listWidget_2_itemSelectionChanged();

private:
    Ui::MyCourseDialog *ui;

    QDomElement root ;

    QStringList CourseList ;
    QStringList CourseIDList ;
    QStringList CurrentClassList ;
    QStringList ClassesNamesList;
    QStringList YearsNamesList;
    QStringList SemestersNamesList;

    QString CurrentCourse ;
    QString CurrentClass ;
    QString path ;
    QString semester ;
    QString year ;
    QString filename ;
    QString file_extension ;

    QDate date ;

    bool dialog_ended ;
};

#endif // MYCOURSEDIALOG_H
