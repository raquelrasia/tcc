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
    explicit MyCourseDialog(QWidget *parent = 0);
    ~MyCourseDialog();

    QString get_current_course() ;
    QString get_current_class() ;
    QString get_current_semester() ;
    QString get_rec_dir() ;
    QString get_dir_videos() ;
    QString get_filename() ;

    void set_dir_mode(bool mode) ;

    bool get_dialog_ended() ;

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

    QString CurrentCourse ;
    QString CurrentClass ;
    QString dir_videos ;

    bool autopath ;

    // Directory of the recording
    QString rec_dir ;
    QString semester ;
    QString filename ;

    QDate date ;

    bool dialog_ended ;
};

#endif // MYCOURSEDIALOG_H
