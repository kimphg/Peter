/********************************************************************************
** Form generated from reading UI file 'statuswindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATUSWINDOW_H
#define UI_STATUSWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StatusWindow
{
public:
    QWidget *centralwidget;
    QGroupBox *groupBox;
    QFrame *frame_5;
    QGridLayout *gridLayout_5;
    QLabel *label_res_receiver;
    QLabel *label_123;
    QLabel *label_5;
    QFrame *frame;
    QGridLayout *gridLayout;
    QLabel *label_128;
    QLabel *label;
    QLabel *label_res_dds_out;
    QFrame *frame_4;
    QGridLayout *gridLayout_4;
    QLabel *label_4;
    QLabel *label_127;
    QLabel *label_vco_output_2;
    QFrame *frame_3;
    QGridLayout *gridLayout_3;
    QLabel *label_3;
    QLabel *label_129;
    QLabel *label_vco_output_1;
    QFrame *frame_2;
    QGridLayout *gridLayout_2;
    QLabel *label_2;
    QFrame *frame_6;
    QGridLayout *gridLayout_6;
    QLabel *label_125;
    QLabel *label_res_trans_temp;
    QLabel *label_6;
    QLabel *label_trans_output;
    QLabel *label_126;
    QLabel *label_trans_input;
    QLabel *label_124;
    QFrame *line;
    QFrame *line_2;
    QFrame *line_3;
    QFrame *frame_7;
    QGridLayout *gridLayout_7;
    QLabel *label_8;
    QFrame *line_6;
    QFrame *line_7;
    QFrame *line_8;
    QFrame *line_9;
    QFrame *line_11;
    QFrame *line_12;
    QFrame *frame_9;
    QGridLayout *gridLayout_9;
    QLabel *label_10;
    QFrame *frame_10;
    QGridLayout *gridLayout_10;
    QLabel *label_11;
    QFrame *line_13;
    QFrame *line_14;
    QFrame *frame_11;
    QGridLayout *gridLayout_11;
    QLabel *label_res_main_temp;
    QLabel *label_1256;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_byte_1;
    QLabel *label_byte_2;
    QLabel *label_byte_3;
    QLabel *label_byte_4;

    void setupUi(QMainWindow *StatusWindow)
    {
        if (StatusWindow->objectName().isEmpty())
            StatusWindow->setObjectName(QStringLiteral("StatusWindow"));
        StatusWindow->resize(939, 534);
        QFont font;
        font.setPointSize(16);
        StatusWindow->setFont(font);
        centralwidget = new QWidget(StatusWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(20, 20, 891, 501));
        QFont font1;
        font1.setPointSize(12);
        groupBox->setFont(font1);
        frame_5 = new QFrame(groupBox);
        frame_5->setObjectName(QStringLiteral("frame_5"));
        frame_5->setGeometry(QRect(270, 340, 271, 71));
        frame_5->setFont(font1);
        frame_5->setStyleSheet(QLatin1String("background-color: rgb(85, 170, 127);\n"
"border: 2px solid darkgreen;\n"
""));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        gridLayout_5 = new QGridLayout(frame_5);
        gridLayout_5->setSpacing(2);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setContentsMargins(2, 2, 2, 2);
        label_res_receiver = new QLabel(frame_5);
        label_res_receiver->setObjectName(QStringLiteral("label_res_receiver"));
        label_res_receiver->setFont(font1);

        gridLayout_5->addWidget(label_res_receiver, 1, 1, 1, 1);

        label_123 = new QLabel(frame_5);
        label_123->setObjectName(QStringLiteral("label_123"));
        label_123->setFont(font1);
        label_123->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(label_123, 1, 0, 1, 1);

        label_5 = new QLabel(frame_5);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setFont(font1);
        label_5->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(label_5, 0, 0, 1, 2);

        frame = new QFrame(groupBox);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(20, 50, 181, 91));
        frame->setFont(font1);
        frame->setStyleSheet(QLatin1String("background-color: rgb(85, 170, 127);\n"
"border: 2px solid darkgreen;\n"
""));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setSpacing(2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(2, 2, 2, 2);
        label_128 = new QLabel(frame);
        label_128->setObjectName(QStringLiteral("label_128"));
        label_128->setFont(font1);
        label_128->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_128, 1, 0, 1, 1);

        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));
        label->setFont(font1);
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 2);

        label_res_dds_out = new QLabel(frame);
        label_res_dds_out->setObjectName(QStringLiteral("label_res_dds_out"));
        label_res_dds_out->setFont(font1);

        gridLayout->addWidget(label_res_dds_out, 1, 1, 1, 1);

        frame_4 = new QFrame(groupBox);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        frame_4->setGeometry(QRect(420, 190, 201, 71));
        frame_4->setFont(font1);
        frame_4->setStyleSheet(QLatin1String("background-color: rgb(85, 170, 127);\n"
"border: 2px solid darkgreen;\n"
""));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        gridLayout_4 = new QGridLayout(frame_4);
        gridLayout_4->setSpacing(2);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(2, 2, 2, 2);
        label_4 = new QLabel(frame_4);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setFont(font1);
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(label_4, 0, 0, 1, 2);

        label_127 = new QLabel(frame_4);
        label_127->setObjectName(QStringLiteral("label_127"));
        label_127->setFont(font1);
        label_127->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(label_127, 1, 0, 1, 1);

        label_vco_output_2 = new QLabel(frame_4);
        label_vco_output_2->setObjectName(QStringLiteral("label_vco_output_2"));
        label_vco_output_2->setFont(font1);

        gridLayout_4->addWidget(label_vco_output_2, 1, 1, 1, 1);

        frame_3 = new QFrame(groupBox);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setGeometry(QRect(200, 190, 201, 71));
        frame_3->setFont(font1);
        frame_3->setStyleSheet(QLatin1String("background-color: rgb(85, 170, 127);\n"
"border: 2px solid darkgreen;\n"
""));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame_3);
        gridLayout_3->setSpacing(2);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(2, 2, 2, 2);
        label_3 = new QLabel(frame_3);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font1);
        label_3->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_3, 0, 0, 1, 2);

        label_129 = new QLabel(frame_3);
        label_129->setObjectName(QStringLiteral("label_129"));
        label_129->setFont(font1);
        label_129->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_129, 1, 0, 1, 1);

        label_vco_output_1 = new QLabel(frame_3);
        label_vco_output_1->setObjectName(QStringLiteral("label_vco_output_1"));
        label_vco_output_1->setFont(font1);

        gridLayout_3->addWidget(label_vco_output_1, 1, 1, 1, 1);

        frame_2 = new QFrame(groupBox);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(20, 310, 201, 81));
        frame_2->setFont(font1);
        frame_2->setStyleSheet(QLatin1String("background-color: rgb(200, 200, 50);\n"
"border: 2px;\n"
"border-color: rgb(100,100,10);"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame_2);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_2 = new QLabel(frame_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font1);
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_2, 0, 0, 1, 2);

        frame_6 = new QFrame(groupBox);
        frame_6->setObjectName(QStringLiteral("frame_6"));
        frame_6->setGeometry(QRect(620, 20, 251, 151));
        frame_6->setFont(font1);
        frame_6->setStyleSheet(QLatin1String("background-color: rgb(85, 170, 127);\n"
"border: 2px solid darkgreen;\n"
""));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        gridLayout_6 = new QGridLayout(frame_6);
        gridLayout_6->setSpacing(2);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_6->setContentsMargins(2, 2, 2, 2);
        label_125 = new QLabel(frame_6);
        label_125->setObjectName(QStringLiteral("label_125"));
        label_125->setFont(font1);
        label_125->setAlignment(Qt::AlignCenter);

        gridLayout_6->addWidget(label_125, 2, 0, 1, 1);

        label_res_trans_temp = new QLabel(frame_6);
        label_res_trans_temp->setObjectName(QStringLiteral("label_res_trans_temp"));
        label_res_trans_temp->setFont(font1);

        gridLayout_6->addWidget(label_res_trans_temp, 1, 1, 1, 1);

        label_6 = new QLabel(frame_6);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setFont(font1);
        label_6->setAlignment(Qt::AlignCenter);

        gridLayout_6->addWidget(label_6, 0, 0, 1, 2);

        label_trans_output = new QLabel(frame_6);
        label_trans_output->setObjectName(QStringLiteral("label_trans_output"));
        label_trans_output->setFont(font1);

        gridLayout_6->addWidget(label_trans_output, 3, 1, 1, 1);

        label_126 = new QLabel(frame_6);
        label_126->setObjectName(QStringLiteral("label_126"));
        label_126->setFont(font1);
        label_126->setAlignment(Qt::AlignCenter);

        gridLayout_6->addWidget(label_126, 3, 0, 1, 1);

        label_trans_input = new QLabel(frame_6);
        label_trans_input->setObjectName(QStringLiteral("label_trans_input"));
        label_trans_input->setFont(font1);

        gridLayout_6->addWidget(label_trans_input, 2, 1, 1, 1);

        label_124 = new QLabel(frame_6);
        label_124->setObjectName(QStringLiteral("label_124"));
        label_124->setFont(font1);
        label_124->setAlignment(Qt::AlignCenter);

        gridLayout_6->addWidget(label_124, 1, 0, 1, 1);

        line = new QFrame(groupBox);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(200, 90, 51, 20));
        line->setFont(font1);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(groupBox);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(460, 260, 20, 81));
        line_2->setFont(font1);
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        line_3 = new QFrame(groupBox);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setGeometry(QRect(320, 260, 20, 81));
        line_3->setFont(font1);
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);
        frame_7 = new QFrame(groupBox);
        frame_7->setObjectName(QStringLiteral("frame_7"));
        frame_7->setGeometry(QRect(670, 310, 161, 91));
        frame_7->setFont(font1);
        frame_7->setStyleSheet(QLatin1String("background-color: rgb(200, 200, 50);\n"
"border: 2px;\n"
"border-color: rgb(100,100,10);"));
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);
        gridLayout_7 = new QGridLayout(frame_7);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        label_8 = new QLabel(frame_7);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setFont(font1);
        label_8->setAlignment(Qt::AlignCenter);

        gridLayout_7->addWidget(label_8, 0, 0, 1, 1);

        line_6 = new QFrame(groupBox);
        line_6->setObjectName(QStringLiteral("line_6"));
        line_6->setGeometry(QRect(360, 90, 101, 20));
        line_6->setFont(font1);
        line_6->setFrameShape(QFrame::HLine);
        line_6->setFrameShadow(QFrame::Sunken);
        line_7 = new QFrame(groupBox);
        line_7->setObjectName(QStringLiteral("line_7"));
        line_7->setGeometry(QRect(570, 90, 51, 20));
        line_7->setFont(font1);
        line_7->setFrameShape(QFrame::HLine);
        line_7->setFrameShadow(QFrame::Sunken);
        line_8 = new QFrame(groupBox);
        line_8->setObjectName(QStringLiteral("line_8"));
        line_8->setGeometry(QRect(740, 170, 20, 141));
        line_8->setFont(font1);
        line_8->setFrameShape(QFrame::VLine);
        line_8->setFrameShadow(QFrame::Sunken);
        line_9 = new QFrame(groupBox);
        line_9->setObjectName(QStringLiteral("line_9"));
        line_9->setGeometry(QRect(540, 360, 131, 20));
        line_9->setFont(font1);
        line_9->setFrameShape(QFrame::HLine);
        line_9->setFrameShadow(QFrame::Sunken);
        line_11 = new QFrame(groupBox);
        line_11->setObjectName(QStringLiteral("line_11"));
        line_11->setGeometry(QRect(110, 140, 20, 171));
        line_11->setFont(font1);
        line_11->setFrameShape(QFrame::VLine);
        line_11->setFrameShadow(QFrame::Sunken);
        line_12 = new QFrame(groupBox);
        line_12->setObjectName(QStringLiteral("line_12"));
        line_12->setGeometry(QRect(220, 360, 51, 20));
        line_12->setFont(font1);
        line_12->setFrameShape(QFrame::HLine);
        line_12->setFrameShadow(QFrame::Sunken);
        frame_9 = new QFrame(groupBox);
        frame_9->setObjectName(QStringLiteral("frame_9"));
        frame_9->setGeometry(QRect(250, 70, 111, 51));
        frame_9->setFont(font1);
        frame_9->setStyleSheet(QLatin1String("background-color: rgb(200, 200, 50);\n"
"border: 2px;\n"
"border-color: rgb(100,100,10);"));
        frame_9->setFrameShape(QFrame::StyledPanel);
        frame_9->setFrameShadow(QFrame::Raised);
        gridLayout_9 = new QGridLayout(frame_9);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        label_10 = new QLabel(frame_9);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setFont(font1);
        label_10->setAlignment(Qt::AlignCenter);

        gridLayout_9->addWidget(label_10, 0, 0, 1, 1);

        frame_10 = new QFrame(groupBox);
        frame_10->setObjectName(QStringLiteral("frame_10"));
        frame_10->setGeometry(QRect(460, 70, 111, 51));
        frame_10->setFont(font1);
        frame_10->setStyleSheet(QLatin1String("background-color: rgb(200, 200, 50);\n"
"border: 2px;\n"
"border-color: rgb(100,100,10);"));
        frame_10->setFrameShape(QFrame::StyledPanel);
        frame_10->setFrameShadow(QFrame::Raised);
        gridLayout_10 = new QGridLayout(frame_10);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        label_11 = new QLabel(frame_10);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setFont(font1);
        label_11->setAlignment(Qt::AlignCenter);

        gridLayout_10->addWidget(label_11, 0, 0, 1, 1);

        line_13 = new QFrame(groupBox);
        line_13->setObjectName(QStringLiteral("line_13"));
        line_13->setGeometry(QRect(290, 120, 20, 71));
        line_13->setFont(font1);
        line_13->setFrameShape(QFrame::VLine);
        line_13->setFrameShadow(QFrame::Sunken);
        line_14 = new QFrame(groupBox);
        line_14->setObjectName(QStringLiteral("line_14"));
        line_14->setGeometry(QRect(500, 120, 20, 71));
        line_14->setFont(font1);
        line_14->setFrameShape(QFrame::VLine);
        line_14->setFrameShadow(QFrame::Sunken);
        frame_11 = new QFrame(groupBox);
        frame_11->setObjectName(QStringLiteral("frame_11"));
        frame_11->setGeometry(QRect(20, 400, 211, 51));
        frame_11->setFont(font1);
        frame_11->setStyleSheet(QLatin1String("background-color: rgb(200, 200, 50);\n"
"border: 2px;\n"
"border-color: rgb(100,100,10);"));
        frame_11->setFrameShape(QFrame::StyledPanel);
        frame_11->setFrameShadow(QFrame::Raised);
        gridLayout_11 = new QGridLayout(frame_11);
        gridLayout_11->setObjectName(QStringLiteral("gridLayout_11"));
        label_res_main_temp = new QLabel(frame_11);
        label_res_main_temp->setObjectName(QStringLiteral("label_res_main_temp"));
        label_res_main_temp->setFont(font1);

        gridLayout_11->addWidget(label_res_main_temp, 0, 1, 1, 1);

        label_1256 = new QLabel(frame_11);
        label_1256->setObjectName(QStringLiteral("label_1256"));
        label_1256->setFont(font1);

        gridLayout_11->addWidget(label_1256, 0, 0, 1, 1);

        layoutWidget = new QWidget(groupBox);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(320, 460, 281, 21));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_byte_1 = new QLabel(layoutWidget);
        label_byte_1->setObjectName(QStringLiteral("label_byte_1"));
        label_byte_1->setFont(font1);
        label_byte_1->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_byte_1);

        label_byte_2 = new QLabel(layoutWidget);
        label_byte_2->setObjectName(QStringLiteral("label_byte_2"));
        label_byte_2->setFont(font1);
        label_byte_2->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_byte_2);

        label_byte_3 = new QLabel(layoutWidget);
        label_byte_3->setObjectName(QStringLiteral("label_byte_3"));
        label_byte_3->setFont(font1);
        label_byte_3->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_byte_3);

        label_byte_4 = new QLabel(layoutWidget);
        label_byte_4->setObjectName(QStringLiteral("label_byte_4"));
        label_byte_4->setFont(font1);
        label_byte_4->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_byte_4);

        StatusWindow->setCentralWidget(centralwidget);

        retranslateUi(StatusWindow);

        QMetaObject::connectSlotsByName(StatusWindow);
    } // setupUi

    void retranslateUi(QMainWindow *StatusWindow)
    {
        StatusWindow->setWindowTitle(QApplication::translate("StatusWindow", "MainWindow", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("StatusWindow", "Tr\341\272\241ng th\303\241i c\303\241c m\303\264 \304\221un m\303\241y 2-1", Q_NULLPTR));
        label_res_receiver->setText(QApplication::translate("StatusWindow", "--", Q_NULLPTR));
        label_123->setText(QApplication::translate("StatusWindow", "H\341\273\207 s\341\273\221 K.\304\221\341\272\241i\n"
"(52->58dBm):", Q_NULLPTR));
        label_5->setText(QApplication::translate("StatusWindow", "M\303\264 \304\221un thu", Q_NULLPTR));
        label_128->setText(QApplication::translate("StatusWindow", "C.su\341\272\245t ra\n"
" (4->10dBm):", Q_NULLPTR));
        label->setText(QApplication::translate("StatusWindow", "M\303\264 \304\221un \n"
"t\341\273\225 h\341\273\243p t\341\272\247n s\341\273\221 (DDS)", Q_NULLPTR));
        label_res_dds_out->setText(QApplication::translate("StatusWindow", "--", Q_NULLPTR));
        label_4->setText(QApplication::translate("StatusWindow", "M\303\264 \304\221un VCO2", Q_NULLPTR));
        label_127->setText(QApplication::translate("StatusWindow", "C.su\341\272\245t ra\n"
"(4->10dBm):", Q_NULLPTR));
        label_vco_output_2->setText(QApplication::translate("StatusWindow", "--", Q_NULLPTR));
        label_3->setText(QApplication::translate("StatusWindow", "M\303\264 \304\221un VCO1", Q_NULLPTR));
        label_129->setText(QApplication::translate("StatusWindow", "C.su\341\272\245t ra\n"
"(4->10dBm):", Q_NULLPTR));
        label_vco_output_1->setText(QApplication::translate("StatusWindow", "7dBm", Q_NULLPTR));
        label_2->setText(QApplication::translate("StatusWindow", "M\303\264 \304\221un \304\221i\341\273\201u khi\341\273\203n\n"
"v\303\240 XLTH (DSP)", Q_NULLPTR));
        label_125->setText(QApplication::translate("StatusWindow", "C.su\341\272\245t v\303\240o\n"
"(-15 -> -5dBm):", Q_NULLPTR));
        label_res_trans_temp->setText(QApplication::translate("StatusWindow", "--", Q_NULLPTR));
        label_6->setText(QApplication::translate("StatusWindow", "M\303\264 \304\221un ti\341\273\201n khu\341\272\277ch", Q_NULLPTR));
        label_trans_output->setText(QApplication::translate("StatusWindow", "--", Q_NULLPTR));
        label_126->setText(QApplication::translate("StatusWindow", "C.su\341\272\245t ra\n"
"(80->100W):", Q_NULLPTR));
        label_trans_input->setText(QApplication::translate("StatusWindow", "--", Q_NULLPTR));
        label_124->setText(QApplication::translate("StatusWindow", "Nhi\341\273\207t \304\221\341\273\231():", Q_NULLPTR));
        label_8->setText(QApplication::translate("StatusWindow", "M\303\241y 2-2", Q_NULLPTR));
        label_10->setText(QApplication::translate("StatusWindow", "B\341\273\231 tr\341\273\231n 1", Q_NULLPTR));
        label_11->setText(QApplication::translate("StatusWindow", "B\341\273\231 tr\341\273\231n 2", Q_NULLPTR));
        label_res_main_temp->setText(QApplication::translate("StatusWindow", "--", Q_NULLPTR));
        label_1256->setText(QApplication::translate("StatusWindow", "N.\304\221\341\273\231 h\341\273\207 th\341\273\221ng(\304\221\341\273\231 C):", Q_NULLPTR));
        label_byte_1->setText(QApplication::translate("StatusWindow", "--", Q_NULLPTR));
        label_byte_2->setText(QApplication::translate("StatusWindow", "--", Q_NULLPTR));
        label_byte_3->setText(QApplication::translate("StatusWindow", "--", Q_NULLPTR));
        label_byte_4->setText(QApplication::translate("StatusWindow", "--", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class StatusWindow: public Ui_StatusWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATUSWINDOW_H
