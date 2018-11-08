/********************************************************************************
** Form generated from reading UI file 'dialogcommandlog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGCOMMANDLOG_H
#define UI_DIALOGCOMMANDLOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_DialogCommandLog
{
public:
    QGridLayout *gridLayout;
    QTextBrowser *textBrowser;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogCommandLog)
    {
        if (DialogCommandLog->objectName().isEmpty())
            DialogCommandLog->setObjectName(QStringLiteral("DialogCommandLog"));
        DialogCommandLog->resize(598, 501);
        gridLayout = new QGridLayout(DialogCommandLog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        textBrowser = new QTextBrowser(DialogCommandLog);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        gridLayout->addWidget(textBrowser, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(DialogCommandLog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 1, 0, 1, 1);


        retranslateUi(DialogCommandLog);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogCommandLog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogCommandLog, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogCommandLog);
    } // setupUi

    void retranslateUi(QDialog *DialogCommandLog)
    {
        DialogCommandLog->setWindowTitle(QApplication::translate("DialogCommandLog", "Dialog", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class DialogCommandLog: public Ui_DialogCommandLog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGCOMMANDLOG_H
