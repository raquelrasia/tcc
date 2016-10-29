/********************************************************************************
** Form generated from reading UI file 'maindialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINDIALOG_H
#define UI_MAINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainDialog
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QPushButton *pushButton_4;
    QLabel *label;
    QPushButton *pushButton_5;

    void setupUi(QDialog *MainDialog)
    {
        if (MainDialog->objectName().isEmpty())
            MainDialog->setObjectName(QStringLiteral("MainDialog"));
        MainDialog->resize(640, 480);
        gridLayoutWidget = new QWidget(MainDialog);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 250, 621, 111));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_2 = new QPushButton(gridLayoutWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 1, 1, 1, 1);

        pushButton_3 = new QPushButton(gridLayoutWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        gridLayout->addWidget(pushButton_3, 2, 0, 1, 1);

        pushButton = new QPushButton(gridLayoutWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 1, 0, 1, 1);

        checkBox = new QCheckBox(gridLayoutWidget);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        gridLayout->addWidget(checkBox, 0, 1, 1, 1);

        checkBox_2 = new QCheckBox(gridLayoutWidget);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));

        gridLayout->addWidget(checkBox_2, 0, 0, 1, 1);

        pushButton_4 = new QPushButton(gridLayoutWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        gridLayout->addWidget(pushButton_4, 2, 1, 1, 1);

        label = new QLabel(MainDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 621, 81));
        pushButton_5 = new QPushButton(MainDialog);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(230, 400, 191, 41));

        retranslateUi(MainDialog);

        QMetaObject::connectSlotsByName(MainDialog);
    } // setupUi

    void retranslateUi(QDialog *MainDialog)
    {
        MainDialog->setWindowTitle(QApplication::translate("MainDialog", "Dialog", 0));
        pushButton_2->setText(QApplication::translate("MainDialog", "Record", 0));
        pushButton_3->setText(QApplication::translate("MainDialog", "End Lecture", 0));
        pushButton->setText(QApplication::translate("MainDialog", "New Lecture", 0));
        checkBox->setText(QApplication::translate("MainDialog", "Media Mode", 0));
        checkBox_2->setText(QApplication::translate("MainDialog", "Automatically Create Directories and Files", 0));
        pushButton_4->setText(QApplication::translate("MainDialog", "Stop Recording", 0));
        label->setText(QApplication::translate("MainDialog", "<html><head/><body><p><span style=\" font-size:12pt;\">Status:</span></p></body></html>", 0));
        pushButton_5->setText(QApplication::translate("MainDialog", "Document Camera", 0));
    } // retranslateUi

};

namespace Ui {
    class MainDialog: public Ui_MainDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINDIALOG_H
