#ifndef MYCOURSEDIALOG_H
#define MYCOURSEDIALOG_H

#include <QDialog>
#include <QtXml>

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
    bool get_dialog_ended() ;

private slots:

    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_listWidget_itemSelectionChanged();

private:
    Ui::MyCourseDialog *ui;

    QDomElement root ;

    QStringList CourseList ;
    QStringList CurrentClassList ;

    QString CurrentCourse ;
    QString CurrentClass ;

    bool dialog_ended ;
};

#endif // MYCOURSEDIALOG_H
