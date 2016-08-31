/********************************************************************************
** Form generated from reading UI file 'mycoursedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYCOURSEDIALOG_H
#define UI_MYCOURSEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyCourseDialog
{
public:
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QFrame *frame;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QListWidget *listWidget_2;
    QListWidget *listWidget;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QDialog *MyCourseDialog)
    {
        if (MyCourseDialog->objectName().isEmpty())
            MyCourseDialog->setObjectName(QStringLiteral("MyCourseDialog"));
        MyCourseDialog->resize(640, 480);
        horizontalLayoutWidget_2 = new QWidget(MyCourseDialog);
        horizontalLayoutWidget_2->setObjectName(QStringLiteral("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(430, 370, 171, 80));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(horizontalLayoutWidget_2);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_2->addWidget(pushButton);

        pushButton_2 = new QPushButton(horizontalLayoutWidget_2);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout_2->addWidget(pushButton_2);

        frame = new QFrame(MyCourseDialog);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(30, 10, 571, 341));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayoutWidget = new QWidget(frame);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 10, 551, 321));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        listWidget_2 = new QListWidget(gridLayoutWidget);
        listWidget_2->setObjectName(QStringLiteral("listWidget_2"));

        gridLayout->addWidget(listWidget_2, 1, 1, 1, 1);

        listWidget = new QListWidget(gridLayoutWidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        gridLayout->addWidget(listWidget, 1, 0, 1, 1);

        label = new QLabel(gridLayoutWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 1, 1, 1);


        retranslateUi(MyCourseDialog);

        QMetaObject::connectSlotsByName(MyCourseDialog);
    } // setupUi

    void retranslateUi(QDialog *MyCourseDialog)
    {
        MyCourseDialog->setWindowTitle(QApplication::translate("MyCourseDialog", "Dialog", 0));
        pushButton->setText(QApplication::translate("MyCourseDialog", "Back", 0));
        pushButton_2->setText(QApplication::translate("MyCourseDialog", "Continue", 0));
        label->setText(QApplication::translate("MyCourseDialog", "Course List", 0));
        label_2->setText(QApplication::translate("MyCourseDialog", "Class List", 0));
    } // retranslateUi

};

namespace Ui {
    class MyCourseDialog: public Ui_MyCourseDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYCOURSEDIALOG_H
