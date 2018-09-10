/********************************************************************************
** Form generated from reading UI file 'dialogmenudisplay.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGMENUDISPLAY_H
#define UI_DIALOGMENUDISPLAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include <qcustombutton.h>
#include <qcustomcombobox.h>
#include <qcustomgroupbox.h>

QT_BEGIN_NAMESPACE

class Ui_DialogMenuDisplay
{
public:
    QCustomGroupBox *groupBox_17;
    QFormLayout *formLayout;
    QLabel *label_command_3;
    QCustomComboBox *comboBox_3;
    QLabel *label_command_4;
    QSlider *horizontalSlider_map_brightness;
    QTabWidget *groupBox_3;
    QWidget *tab;
    QFormLayout *formLayout_3;
    QCustomComboBox *comboBox;
    QLabel *label_record_file_name_2;
    QCustomButton *toolButton_xl_nguong_3;
    QLabel *label_noiseAverrage;
    QWidget *tab2;
    QGridLayout *gridLayout_10;
    QSlider *horizontalSlider_gain;
    QCustomButton *toolButton_xl_nguong;
    QCustomButton *toolButton_filter2of3;
    QCustomButton *toolButton_filter2of3_2;
    QSlider *horizontalSlider_sea;
    QSlider *horizontalSlider_rain;
    QCustomButton *toolButton_xl_dopler_2;
    QLabel *label_sea;
    QLabel *label_rain;
    QLabel *label_gain;
    QCustomButton *toolButton_xl_dopler;
    QFrame *line_2;
    QCustomButton *toolButton_sharp_eye;
    QSlider *horizontalSlider_brightness;
    QCustomComboBox *comboBox_img_mode;
    QCustomGroupBox *groupBox_18;
    QGridLayout *gridLayout;
    QCustomButton *toolButton_zoom_out;
    QLabel *label_range;
    QCustomButton *toolButton_zoom_in;
    QCustomButton *toolButton_auto_adapt;
    QCustomButton *toolButton_setRangeUnit;

    void setupUi(QDialog *DialogMenuDisplay)
    {
        if (DialogMenuDisplay->objectName().isEmpty())
            DialogMenuDisplay->setObjectName(QStringLiteral("DialogMenuDisplay"));
        DialogMenuDisplay->resize(792, 738);
        groupBox_17 = new QCustomGroupBox(DialogMenuDisplay);
        groupBox_17->setObjectName(QStringLiteral("groupBox_17"));
        groupBox_17->setGeometry(QRect(10, 10, 271, 91));
        formLayout = new QFormLayout(groupBox_17);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        label_command_3 = new QLabel(groupBox_17);
        label_command_3->setObjectName(QStringLiteral("label_command_3"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_command_3->sizePolicy().hasHeightForWidth());
        label_command_3->setSizePolicy(sizePolicy);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(16, 32, 64, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(0, 0, 0, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush2);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush2);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush2);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush2);
        QBrush brush3(QColor(255, 255, 220, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush3);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        label_command_3->setPalette(palette);
        QFont font;
        font.setPointSize(12);
        label_command_3->setFont(font);
        label_command_3->setStyleSheet(QStringLiteral("background-color: rgb(16, 32, 64);"));
        label_command_3->setAlignment(Qt::AlignCenter);

        formLayout->setWidget(0, QFormLayout::LabelRole, label_command_3);

        comboBox_3 = new QCustomComboBox(groupBox_17);
        comboBox_3->setObjectName(QStringLiteral("comboBox_3"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboBox_3->sizePolicy().hasHeightForWidth());
        comboBox_3->setSizePolicy(sizePolicy1);
        QPalette palette1;
        QBrush brush4(QColor(30, 50, 70, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        QBrush brush5(QColor(0, 0, 0, 0));
        brush5.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush5);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Dark, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush4);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush4);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush5);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush5);
        palette1.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush2);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipBase, brush3);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush5);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Dark, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush5);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush5);
        palette1.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Dark, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        comboBox_3->setPalette(palette1);
        comboBox_3->setFont(font);

        formLayout->setWidget(0, QFormLayout::FieldRole, comboBox_3);

        label_command_4 = new QLabel(groupBox_17);
        label_command_4->setObjectName(QStringLiteral("label_command_4"));
        sizePolicy.setHeightForWidth(label_command_4->sizePolicy().hasHeightForWidth());
        label_command_4->setSizePolicy(sizePolicy);
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Midlight, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Dark, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Mid, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush);
        palette2.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette2.setBrush(QPalette::Active, QPalette::AlternateBase, brush2);
        palette2.setBrush(QPalette::Active, QPalette::ToolTipBase, brush3);
        palette2.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Midlight, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Dark, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Mid, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush3);
        palette2.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Midlight, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Dark, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Mid, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        label_command_4->setPalette(palette2);
        label_command_4->setFont(font);
        label_command_4->setStyleSheet(QStringLiteral("background-color: rgb(16, 32, 64);"));
        label_command_4->setAlignment(Qt::AlignCenter);

        formLayout->setWidget(1, QFormLayout::LabelRole, label_command_4);

        horizontalSlider_map_brightness = new QSlider(groupBox_17);
        horizontalSlider_map_brightness->setObjectName(QStringLiteral("horizontalSlider_map_brightness"));
        sizePolicy1.setHeightForWidth(horizontalSlider_map_brightness->sizePolicy().hasHeightForWidth());
        horizontalSlider_map_brightness->setSizePolicy(sizePolicy1);
        QPalette palette3;
        QBrush brush6(QColor(0, 128, 0, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush6);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush5);
        palette3.setBrush(QPalette::Active, QPalette::Light, brush);
        palette3.setBrush(QPalette::Active, QPalette::Midlight, brush);
        QBrush brush7(QColor(127, 127, 127, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Dark, brush7);
        QBrush brush8(QColor(170, 170, 170, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Mid, brush8);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush6);
        palette3.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush6);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush5);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush5);
        palette3.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette3.setBrush(QPalette::Active, QPalette::AlternateBase, brush);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipBase, brush3);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush6);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush5);
        palette3.setBrush(QPalette::Inactive, QPalette::Light, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Midlight, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Dark, brush7);
        palette3.setBrush(QPalette::Inactive, QPalette::Mid, brush8);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush6);
        palette3.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush6);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush5);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush5);
        palette3.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush3);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush6);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush5);
        palette3.setBrush(QPalette::Disabled, QPalette::Light, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Midlight, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Dark, brush7);
        palette3.setBrush(QPalette::Disabled, QPalette::Mid, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette3.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush6);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush5);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush5);
        palette3.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush3);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        horizontalSlider_map_brightness->setPalette(palette3);
        horizontalSlider_map_brightness->setStyleSheet(QStringLiteral(""));
        horizontalSlider_map_brightness->setMinimum(1);
        horizontalSlider_map_brightness->setMaximum(50);
        horizontalSlider_map_brightness->setPageStep(1);
        horizontalSlider_map_brightness->setValue(20);
        horizontalSlider_map_brightness->setOrientation(Qt::Horizontal);
        horizontalSlider_map_brightness->setTickPosition(QSlider::NoTicks);

        formLayout->setWidget(1, QFormLayout::FieldRole, horizontalSlider_map_brightness);

        groupBox_3 = new QTabWidget(DialogMenuDisplay);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 390, 271, 331));
        QFont font1;
        font1.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font1.setPointSize(12);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(50);
        groupBox_3->setFont(font1);
        groupBox_3->setStyleSheet(QStringLiteral("background-color: rgb(30, 50, 70);color:rgb(30, 50, 70);font: 12pt \"MS Shell Dlg 2\";"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        formLayout_3 = new QFormLayout(tab);
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        formLayout_3->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        comboBox = new QCustomComboBox(tab);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        sizePolicy1.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy1);
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette4.setBrush(QPalette::Active, QPalette::Button, brush4);
        palette4.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Midlight, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Dark, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Mid, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Text, brush4);
        palette4.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush4);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush4);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush4);
        palette4.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette4.setBrush(QPalette::Active, QPalette::AlternateBase, brush2);
        palette4.setBrush(QPalette::Active, QPalette::ToolTipBase, brush3);
        palette4.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush4);
        palette4.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Midlight, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Dark, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Mid, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Text, brush4);
        palette4.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush4);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush4);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush4);
        palette4.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush3);
        palette4.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush4);
        palette4.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::Midlight, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::Dark, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::Mid, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette4.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush4);
        palette4.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush3);
        palette4.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        comboBox->setPalette(palette4);

        formLayout_3->setWidget(1, QFormLayout::FieldRole, comboBox);

        label_record_file_name_2 = new QLabel(tab);
        label_record_file_name_2->setObjectName(QStringLiteral("label_record_file_name_2"));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette5.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette5.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette5.setBrush(QPalette::Active, QPalette::Midlight, brush2);
        palette5.setBrush(QPalette::Active, QPalette::Dark, brush2);
        palette5.setBrush(QPalette::Active, QPalette::Mid, brush2);
        palette5.setBrush(QPalette::Active, QPalette::Text, brush4);
        palette5.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette5.setBrush(QPalette::Active, QPalette::ButtonText, brush4);
        palette5.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette5.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette5.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette5.setBrush(QPalette::Active, QPalette::AlternateBase, brush2);
        palette5.setBrush(QPalette::Active, QPalette::ToolTipBase, brush3);
        palette5.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette5.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette5.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::Midlight, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::Dark, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::Mid, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::Text, brush4);
        palette5.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::ButtonText, brush4);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette5.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette5.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush3);
        palette5.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette5.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette5.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::Midlight, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::Dark, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::Mid, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette5.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette5.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette5.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush3);
        palette5.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        label_record_file_name_2->setPalette(palette5);
        label_record_file_name_2->setFont(font);
        label_record_file_name_2->setStyleSheet(QStringLiteral("background-color: rgb(16, 32, 64);"));
        label_record_file_name_2->setAlignment(Qt::AlignCenter);

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label_record_file_name_2);

        toolButton_xl_nguong_3 = new QCustomButton(tab);
        toolButton_xl_nguong_3->setObjectName(QStringLiteral("toolButton_xl_nguong_3"));
        sizePolicy1.setHeightForWidth(toolButton_xl_nguong_3->sizePolicy().hasHeightForWidth());
        toolButton_xl_nguong_3->setSizePolicy(sizePolicy1);
        toolButton_xl_nguong_3->setCheckable(true);

        formLayout_3->setWidget(0, QFormLayout::LabelRole, toolButton_xl_nguong_3);

        label_noiseAverrage = new QLabel(tab);
        label_noiseAverrage->setObjectName(QStringLiteral("label_noiseAverrage"));
        sizePolicy.setHeightForWidth(label_noiseAverrage->sizePolicy().hasHeightForWidth());
        label_noiseAverrage->setSizePolicy(sizePolicy);
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette6.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette6.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette6.setBrush(QPalette::Active, QPalette::Midlight, brush2);
        palette6.setBrush(QPalette::Active, QPalette::Dark, brush2);
        palette6.setBrush(QPalette::Active, QPalette::Mid, brush2);
        palette6.setBrush(QPalette::Active, QPalette::Text, brush4);
        palette6.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette6.setBrush(QPalette::Active, QPalette::ButtonText, brush4);
        palette6.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette6.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette6.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette6.setBrush(QPalette::Active, QPalette::AlternateBase, brush2);
        palette6.setBrush(QPalette::Active, QPalette::ToolTipBase, brush3);
        palette6.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette6.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette6.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::Midlight, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::Dark, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::Mid, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::Text, brush4);
        palette6.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::ButtonText, brush4);
        palette6.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette6.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette6.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush3);
        palette6.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette6.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette6.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::Midlight, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::Dark, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::Mid, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette6.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette6.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette6.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette6.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush3);
        palette6.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        label_noiseAverrage->setPalette(palette6);
        label_noiseAverrage->setFont(font1);
        label_noiseAverrage->setStyleSheet(QStringLiteral("background-color: rgb(16, 32, 64);"));
        label_noiseAverrage->setAlignment(Qt::AlignCenter);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, label_noiseAverrage);

        groupBox_3->addTab(tab, QString());
        tab2 = new QWidget();
        tab2->setObjectName(QStringLiteral("tab2"));
        gridLayout_10 = new QGridLayout(tab2);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        horizontalSlider_gain = new QSlider(tab2);
        horizontalSlider_gain->setObjectName(QStringLiteral("horizontalSlider_gain"));
        sizePolicy1.setHeightForWidth(horizontalSlider_gain->sizePolicy().hasHeightForWidth());
        horizontalSlider_gain->setSizePolicy(sizePolicy1);
        QPalette palette7;
        palette7.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette7.setBrush(QPalette::Active, QPalette::Button, brush4);
        palette7.setBrush(QPalette::Active, QPalette::Light, brush);
        palette7.setBrush(QPalette::Active, QPalette::Midlight, brush);
        palette7.setBrush(QPalette::Active, QPalette::Dark, brush7);
        palette7.setBrush(QPalette::Active, QPalette::Mid, brush8);
        palette7.setBrush(QPalette::Active, QPalette::Text, brush4);
        palette7.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette7.setBrush(QPalette::Active, QPalette::ButtonText, brush4);
        palette7.setBrush(QPalette::Active, QPalette::Base, brush4);
        palette7.setBrush(QPalette::Active, QPalette::Window, brush4);
        palette7.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette7.setBrush(QPalette::Active, QPalette::AlternateBase, brush);
        palette7.setBrush(QPalette::Active, QPalette::ToolTipBase, brush3);
        palette7.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette7.setBrush(QPalette::Inactive, QPalette::Button, brush4);
        palette7.setBrush(QPalette::Inactive, QPalette::Light, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::Midlight, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::Dark, brush7);
        palette7.setBrush(QPalette::Inactive, QPalette::Mid, brush8);
        palette7.setBrush(QPalette::Inactive, QPalette::Text, brush4);
        palette7.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::ButtonText, brush4);
        palette7.setBrush(QPalette::Inactive, QPalette::Base, brush4);
        palette7.setBrush(QPalette::Inactive, QPalette::Window, brush4);
        palette7.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette7.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush3);
        palette7.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette7.setBrush(QPalette::Disabled, QPalette::Button, brush4);
        palette7.setBrush(QPalette::Disabled, QPalette::Light, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::Midlight, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::Dark, brush7);
        palette7.setBrush(QPalette::Disabled, QPalette::Mid, brush8);
        palette7.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette7.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette7.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        palette7.setBrush(QPalette::Disabled, QPalette::Window, brush4);
        palette7.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette7.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush3);
        palette7.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        horizontalSlider_gain->setPalette(palette7);
        horizontalSlider_gain->setStyleSheet(QStringLiteral(""));
        horizontalSlider_gain->setMinimum(0);
        horizontalSlider_gain->setMaximum(40);
        horizontalSlider_gain->setPageStep(1);
        horizontalSlider_gain->setValue(20);
        horizontalSlider_gain->setOrientation(Qt::Horizontal);
        horizontalSlider_gain->setTickPosition(QSlider::NoTicks);

        gridLayout_10->addWidget(horizontalSlider_gain, 1, 1, 1, 1);

        toolButton_xl_nguong = new QCustomButton(tab2);
        toolButton_xl_nguong->setObjectName(QStringLiteral("toolButton_xl_nguong"));
        sizePolicy1.setHeightForWidth(toolButton_xl_nguong->sizePolicy().hasHeightForWidth());
        toolButton_xl_nguong->setSizePolicy(sizePolicy1);
        toolButton_xl_nguong->setCheckable(true);

        gridLayout_10->addWidget(toolButton_xl_nguong, 6, 0, 1, 2);

        toolButton_filter2of3 = new QCustomButton(tab2);
        toolButton_filter2of3->setObjectName(QStringLiteral("toolButton_filter2of3"));
        sizePolicy1.setHeightForWidth(toolButton_filter2of3->sizePolicy().hasHeightForWidth());
        toolButton_filter2of3->setSizePolicy(sizePolicy1);
        toolButton_filter2of3->setCheckable(true);

        gridLayout_10->addWidget(toolButton_filter2of3, 7, 0, 1, 1);

        toolButton_filter2of3_2 = new QCustomButton(tab2);
        toolButton_filter2of3_2->setObjectName(QStringLiteral("toolButton_filter2of3_2"));
        sizePolicy1.setHeightForWidth(toolButton_filter2of3_2->sizePolicy().hasHeightForWidth());
        toolButton_filter2of3_2->setSizePolicy(sizePolicy1);
        toolButton_filter2of3_2->setCheckable(true);

        gridLayout_10->addWidget(toolButton_filter2of3_2, 7, 1, 1, 1);

        horizontalSlider_sea = new QSlider(tab2);
        horizontalSlider_sea->setObjectName(QStringLiteral("horizontalSlider_sea"));
        sizePolicy1.setHeightForWidth(horizontalSlider_sea->sizePolicy().hasHeightForWidth());
        horizontalSlider_sea->setSizePolicy(sizePolicy1);
        horizontalSlider_sea->setStyleSheet(QStringLiteral(""));
        horizontalSlider_sea->setMinimum(0);
        horizontalSlider_sea->setMaximum(40);
        horizontalSlider_sea->setPageStep(1);
        horizontalSlider_sea->setOrientation(Qt::Horizontal);
        horizontalSlider_sea->setTickPosition(QSlider::NoTicks);

        gridLayout_10->addWidget(horizontalSlider_sea, 5, 1, 1, 1);

        horizontalSlider_rain = new QSlider(tab2);
        horizontalSlider_rain->setObjectName(QStringLiteral("horizontalSlider_rain"));
        sizePolicy1.setHeightForWidth(horizontalSlider_rain->sizePolicy().hasHeightForWidth());
        horizontalSlider_rain->setSizePolicy(sizePolicy1);
        QPalette palette8;
        palette8.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette8.setBrush(QPalette::Active, QPalette::Button, brush4);
        palette8.setBrush(QPalette::Active, QPalette::Light, brush);
        palette8.setBrush(QPalette::Active, QPalette::Midlight, brush);
        palette8.setBrush(QPalette::Active, QPalette::Dark, brush7);
        palette8.setBrush(QPalette::Active, QPalette::Mid, brush8);
        palette8.setBrush(QPalette::Active, QPalette::Text, brush4);
        palette8.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette8.setBrush(QPalette::Active, QPalette::ButtonText, brush4);
        palette8.setBrush(QPalette::Active, QPalette::Base, brush4);
        palette8.setBrush(QPalette::Active, QPalette::Window, brush4);
        palette8.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette8.setBrush(QPalette::Active, QPalette::AlternateBase, brush);
        palette8.setBrush(QPalette::Active, QPalette::ToolTipBase, brush3);
        palette8.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette8.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette8.setBrush(QPalette::Inactive, QPalette::Button, brush4);
        palette8.setBrush(QPalette::Inactive, QPalette::Light, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::Midlight, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::Dark, brush7);
        palette8.setBrush(QPalette::Inactive, QPalette::Mid, brush8);
        palette8.setBrush(QPalette::Inactive, QPalette::Text, brush4);
        palette8.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::ButtonText, brush4);
        palette8.setBrush(QPalette::Inactive, QPalette::Base, brush4);
        palette8.setBrush(QPalette::Inactive, QPalette::Window, brush4);
        palette8.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette8.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush3);
        palette8.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette8.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette8.setBrush(QPalette::Disabled, QPalette::Button, brush4);
        palette8.setBrush(QPalette::Disabled, QPalette::Light, brush);
        palette8.setBrush(QPalette::Disabled, QPalette::Midlight, brush);
        palette8.setBrush(QPalette::Disabled, QPalette::Dark, brush7);
        palette8.setBrush(QPalette::Disabled, QPalette::Mid, brush8);
        palette8.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette8.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette8.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette8.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        palette8.setBrush(QPalette::Disabled, QPalette::Window, brush4);
        palette8.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette8.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush);
        palette8.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush3);
        palette8.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        horizontalSlider_rain->setPalette(palette8);
        horizontalSlider_rain->setStyleSheet(QStringLiteral(""));
        horizontalSlider_rain->setMinimum(0);
        horizontalSlider_rain->setMaximum(50);
        horizontalSlider_rain->setPageStep(1);
        horizontalSlider_rain->setTracking(false);
        horizontalSlider_rain->setOrientation(Qt::Horizontal);
        horizontalSlider_rain->setTickPosition(QSlider::NoTicks);
        horizontalSlider_rain->setTickInterval(1);

        gridLayout_10->addWidget(horizontalSlider_rain, 4, 1, 1, 1);

        toolButton_xl_dopler_2 = new QCustomButton(tab2);
        toolButton_xl_dopler_2->setObjectName(QStringLiteral("toolButton_xl_dopler_2"));
        sizePolicy1.setHeightForWidth(toolButton_xl_dopler_2->sizePolicy().hasHeightForWidth());
        toolButton_xl_dopler_2->setSizePolicy(sizePolicy1);
        toolButton_xl_dopler_2->setCheckable(true);

        gridLayout_10->addWidget(toolButton_xl_dopler_2, 12, 0, 1, 2);

        label_sea = new QLabel(tab2);
        label_sea->setObjectName(QStringLiteral("label_sea"));
        QPalette palette9;
        palette9.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette9.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette9.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette9.setBrush(QPalette::Active, QPalette::Midlight, brush2);
        palette9.setBrush(QPalette::Active, QPalette::Dark, brush2);
        palette9.setBrush(QPalette::Active, QPalette::Mid, brush2);
        palette9.setBrush(QPalette::Active, QPalette::Text, brush);
        palette9.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette9.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette9.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette9.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette9.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette9.setBrush(QPalette::Active, QPalette::AlternateBase, brush2);
        palette9.setBrush(QPalette::Active, QPalette::ToolTipBase, brush3);
        palette9.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette9.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette9.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette9.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette9.setBrush(QPalette::Inactive, QPalette::Midlight, brush2);
        palette9.setBrush(QPalette::Inactive, QPalette::Dark, brush2);
        palette9.setBrush(QPalette::Inactive, QPalette::Mid, brush2);
        palette9.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette9.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette9.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette9.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette9.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette9.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette9.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush2);
        palette9.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush3);
        palette9.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette9.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette9.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette9.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette9.setBrush(QPalette::Disabled, QPalette::Midlight, brush2);
        palette9.setBrush(QPalette::Disabled, QPalette::Dark, brush2);
        palette9.setBrush(QPalette::Disabled, QPalette::Mid, brush2);
        palette9.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette9.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette9.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette9.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette9.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette9.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette9.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush2);
        palette9.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush3);
        palette9.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        label_sea->setPalette(palette9);
        label_sea->setFont(font);
        label_sea->setStyleSheet(QStringLiteral("color:rgb(255, 255, 255);background-color: rgb(16, 32, 64);"));

        gridLayout_10->addWidget(label_sea, 5, 0, 1, 1);

        label_rain = new QLabel(tab2);
        label_rain->setObjectName(QStringLiteral("label_rain"));
        QPalette palette10;
        palette10.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette10.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette10.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette10.setBrush(QPalette::Active, QPalette::Midlight, brush2);
        palette10.setBrush(QPalette::Active, QPalette::Dark, brush2);
        palette10.setBrush(QPalette::Active, QPalette::Mid, brush2);
        palette10.setBrush(QPalette::Active, QPalette::Text, brush);
        palette10.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette10.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette10.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette10.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette10.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette10.setBrush(QPalette::Active, QPalette::AlternateBase, brush2);
        palette10.setBrush(QPalette::Active, QPalette::ToolTipBase, brush3);
        palette10.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette10.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette10.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette10.setBrush(QPalette::Inactive, QPalette::Midlight, brush2);
        palette10.setBrush(QPalette::Inactive, QPalette::Dark, brush2);
        palette10.setBrush(QPalette::Inactive, QPalette::Mid, brush2);
        palette10.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette10.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette10.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette10.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush2);
        palette10.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush3);
        palette10.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette10.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette10.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette10.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette10.setBrush(QPalette::Disabled, QPalette::Midlight, brush2);
        palette10.setBrush(QPalette::Disabled, QPalette::Dark, brush2);
        palette10.setBrush(QPalette::Disabled, QPalette::Mid, brush2);
        palette10.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette10.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette10.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette10.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette10.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette10.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette10.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush2);
        palette10.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush3);
        palette10.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        label_rain->setPalette(palette10);
        label_rain->setFont(font);
        label_rain->setStyleSheet(QStringLiteral("color:rgb(255, 255, 255);background-color: rgb(16, 32, 64);"));

        gridLayout_10->addWidget(label_rain, 4, 0, 1, 1);

        label_gain = new QLabel(tab2);
        label_gain->setObjectName(QStringLiteral("label_gain"));
        QPalette palette11;
        palette11.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette11.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette11.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette11.setBrush(QPalette::Active, QPalette::Midlight, brush2);
        palette11.setBrush(QPalette::Active, QPalette::Dark, brush2);
        palette11.setBrush(QPalette::Active, QPalette::Mid, brush2);
        palette11.setBrush(QPalette::Active, QPalette::Text, brush);
        palette11.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette11.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette11.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette11.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette11.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette11.setBrush(QPalette::Active, QPalette::AlternateBase, brush2);
        palette11.setBrush(QPalette::Active, QPalette::ToolTipBase, brush3);
        palette11.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette11.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette11.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette11.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette11.setBrush(QPalette::Inactive, QPalette::Midlight, brush2);
        palette11.setBrush(QPalette::Inactive, QPalette::Dark, brush2);
        palette11.setBrush(QPalette::Inactive, QPalette::Mid, brush2);
        palette11.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette11.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette11.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette11.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette11.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette11.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette11.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush2);
        palette11.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush3);
        palette11.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette11.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette11.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette11.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette11.setBrush(QPalette::Disabled, QPalette::Midlight, brush2);
        palette11.setBrush(QPalette::Disabled, QPalette::Dark, brush2);
        palette11.setBrush(QPalette::Disabled, QPalette::Mid, brush2);
        palette11.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette11.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette11.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette11.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette11.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette11.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette11.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush2);
        palette11.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush3);
        palette11.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        label_gain->setPalette(palette11);
        label_gain->setFont(font);
        label_gain->setStyleSheet(QStringLiteral("color:rgb(255, 255, 255);background-color: rgb(16, 32, 64);"));

        gridLayout_10->addWidget(label_gain, 1, 0, 1, 1);

        toolButton_xl_dopler = new QCustomButton(tab2);
        toolButton_xl_dopler->setObjectName(QStringLiteral("toolButton_xl_dopler"));
        sizePolicy1.setHeightForWidth(toolButton_xl_dopler->sizePolicy().hasHeightForWidth());
        toolButton_xl_dopler->setSizePolicy(sizePolicy1);
        toolButton_xl_dopler->setCheckable(true);

        gridLayout_10->addWidget(toolButton_xl_dopler, 9, 0, 1, 2);

        line_2 = new QFrame(tab2);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout_10->addWidget(line_2, 8, 0, 1, 2);

        toolButton_sharp_eye = new QCustomButton(tab2);
        toolButton_sharp_eye->setObjectName(QStringLiteral("toolButton_sharp_eye"));
        sizePolicy1.setHeightForWidth(toolButton_sharp_eye->sizePolicy().hasHeightForWidth());
        toolButton_sharp_eye->setSizePolicy(sizePolicy1);
        toolButton_sharp_eye->setCheckable(true);

        gridLayout_10->addWidget(toolButton_sharp_eye, 11, 0, 1, 2);

        groupBox_3->addTab(tab2, QString());
        horizontalSlider_brightness = new QSlider(DialogMenuDisplay);
        horizontalSlider_brightness->setObjectName(QStringLiteral("horizontalSlider_brightness"));
        horizontalSlider_brightness->setGeometry(QRect(530, 370, 181, 48));
        sizePolicy1.setHeightForWidth(horizontalSlider_brightness->sizePolicy().hasHeightForWidth());
        horizontalSlider_brightness->setSizePolicy(sizePolicy1);
        QPalette palette12;
        palette12.setBrush(QPalette::Active, QPalette::WindowText, brush6);
        palette12.setBrush(QPalette::Active, QPalette::Button, brush5);
        palette12.setBrush(QPalette::Active, QPalette::Light, brush);
        palette12.setBrush(QPalette::Active, QPalette::Midlight, brush);
        palette12.setBrush(QPalette::Active, QPalette::Dark, brush7);
        palette12.setBrush(QPalette::Active, QPalette::Mid, brush8);
        palette12.setBrush(QPalette::Active, QPalette::Text, brush6);
        palette12.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette12.setBrush(QPalette::Active, QPalette::ButtonText, brush6);
        palette12.setBrush(QPalette::Active, QPalette::Base, brush5);
        palette12.setBrush(QPalette::Active, QPalette::Window, brush5);
        palette12.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette12.setBrush(QPalette::Active, QPalette::AlternateBase, brush);
        palette12.setBrush(QPalette::Active, QPalette::ToolTipBase, brush3);
        palette12.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette12.setBrush(QPalette::Inactive, QPalette::WindowText, brush6);
        palette12.setBrush(QPalette::Inactive, QPalette::Button, brush5);
        palette12.setBrush(QPalette::Inactive, QPalette::Light, brush);
        palette12.setBrush(QPalette::Inactive, QPalette::Midlight, brush);
        palette12.setBrush(QPalette::Inactive, QPalette::Dark, brush7);
        palette12.setBrush(QPalette::Inactive, QPalette::Mid, brush8);
        palette12.setBrush(QPalette::Inactive, QPalette::Text, brush6);
        palette12.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette12.setBrush(QPalette::Inactive, QPalette::ButtonText, brush6);
        palette12.setBrush(QPalette::Inactive, QPalette::Base, brush5);
        palette12.setBrush(QPalette::Inactive, QPalette::Window, brush5);
        palette12.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette12.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush);
        palette12.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush3);
        palette12.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette12.setBrush(QPalette::Disabled, QPalette::WindowText, brush6);
        palette12.setBrush(QPalette::Disabled, QPalette::Button, brush5);
        palette12.setBrush(QPalette::Disabled, QPalette::Light, brush);
        palette12.setBrush(QPalette::Disabled, QPalette::Midlight, brush);
        palette12.setBrush(QPalette::Disabled, QPalette::Dark, brush7);
        palette12.setBrush(QPalette::Disabled, QPalette::Mid, brush8);
        palette12.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette12.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette12.setBrush(QPalette::Disabled, QPalette::ButtonText, brush6);
        palette12.setBrush(QPalette::Disabled, QPalette::Base, brush5);
        palette12.setBrush(QPalette::Disabled, QPalette::Window, brush5);
        palette12.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette12.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush);
        palette12.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush3);
        palette12.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        horizontalSlider_brightness->setPalette(palette12);
        horizontalSlider_brightness->setStyleSheet(QStringLiteral(""));
        horizontalSlider_brightness->setMinimum(1);
        horizontalSlider_brightness->setMaximum(40);
        horizontalSlider_brightness->setPageStep(1);
        horizontalSlider_brightness->setValue(20);
        horizontalSlider_brightness->setOrientation(Qt::Horizontal);
        horizontalSlider_brightness->setTickPosition(QSlider::NoTicks);
        comboBox_img_mode = new QCustomComboBox(DialogMenuDisplay);
        comboBox_img_mode->setObjectName(QStringLiteral("comboBox_img_mode"));
        comboBox_img_mode->setGeometry(QRect(530, 290, 181, 48));
        sizePolicy1.setHeightForWidth(comboBox_img_mode->sizePolicy().hasHeightForWidth());
        comboBox_img_mode->setSizePolicy(sizePolicy1);
        QPalette palette13;
        palette13.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette13.setBrush(QPalette::Active, QPalette::Button, brush5);
        palette13.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette13.setBrush(QPalette::Active, QPalette::Midlight, brush2);
        palette13.setBrush(QPalette::Active, QPalette::Dark, brush2);
        palette13.setBrush(QPalette::Active, QPalette::Mid, brush2);
        palette13.setBrush(QPalette::Active, QPalette::Text, brush);
        palette13.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette13.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette13.setBrush(QPalette::Active, QPalette::Base, brush5);
        palette13.setBrush(QPalette::Active, QPalette::Window, brush5);
        palette13.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette13.setBrush(QPalette::Active, QPalette::AlternateBase, brush2);
        palette13.setBrush(QPalette::Active, QPalette::ToolTipBase, brush3);
        palette13.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette13.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette13.setBrush(QPalette::Inactive, QPalette::Button, brush5);
        palette13.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette13.setBrush(QPalette::Inactive, QPalette::Midlight, brush2);
        palette13.setBrush(QPalette::Inactive, QPalette::Dark, brush2);
        palette13.setBrush(QPalette::Inactive, QPalette::Mid, brush2);
        palette13.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette13.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette13.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette13.setBrush(QPalette::Inactive, QPalette::Base, brush5);
        palette13.setBrush(QPalette::Inactive, QPalette::Window, brush5);
        palette13.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette13.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush2);
        palette13.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush3);
        palette13.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette13.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette13.setBrush(QPalette::Disabled, QPalette::Button, brush5);
        palette13.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette13.setBrush(QPalette::Disabled, QPalette::Midlight, brush2);
        palette13.setBrush(QPalette::Disabled, QPalette::Dark, brush2);
        palette13.setBrush(QPalette::Disabled, QPalette::Mid, brush2);
        palette13.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette13.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette13.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette13.setBrush(QPalette::Disabled, QPalette::Base, brush5);
        palette13.setBrush(QPalette::Disabled, QPalette::Window, brush5);
        palette13.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette13.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush2);
        palette13.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush3);
        palette13.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        comboBox_img_mode->setPalette(palette13);
        comboBox_img_mode->setFont(font);
        groupBox_18 = new QCustomGroupBox(DialogMenuDisplay);
        groupBox_18->setObjectName(QStringLiteral("groupBox_18"));
        groupBox_18->setGeometry(QRect(60, 110, 271, 162));
        gridLayout = new QGridLayout(groupBox_18);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        toolButton_zoom_out = new QCustomButton(groupBox_18);
        toolButton_zoom_out->setObjectName(QStringLiteral("toolButton_zoom_out"));
        sizePolicy1.setHeightForWidth(toolButton_zoom_out->sizePolicy().hasHeightForWidth());
        toolButton_zoom_out->setSizePolicy(sizePolicy1);
        QFont font2;
        font2.setBold(true);
        font2.setWeight(75);
        toolButton_zoom_out->setFont(font2);
        toolButton_zoom_out->setCheckable(false);

        gridLayout->addWidget(toolButton_zoom_out, 4, 1, 1, 1);

        label_range = new QLabel(groupBox_18);
        label_range->setObjectName(QStringLiteral("label_range"));
        QPalette palette14;
        palette14.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush9(QColor(40, 60, 100, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette14.setBrush(QPalette::Active, QPalette::Button, brush9);
        palette14.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette14.setBrush(QPalette::Active, QPalette::Midlight, brush2);
        palette14.setBrush(QPalette::Active, QPalette::Dark, brush2);
        palette14.setBrush(QPalette::Active, QPalette::Mid, brush2);
        palette14.setBrush(QPalette::Active, QPalette::Text, brush);
        palette14.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette14.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette14.setBrush(QPalette::Active, QPalette::Base, brush9);
        palette14.setBrush(QPalette::Active, QPalette::Window, brush9);
        palette14.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette14.setBrush(QPalette::Active, QPalette::AlternateBase, brush2);
        palette14.setBrush(QPalette::Active, QPalette::ToolTipBase, brush3);
        palette14.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette14.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::Button, brush9);
        palette14.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette14.setBrush(QPalette::Inactive, QPalette::Midlight, brush2);
        palette14.setBrush(QPalette::Inactive, QPalette::Dark, brush2);
        palette14.setBrush(QPalette::Inactive, QPalette::Mid, brush2);
        palette14.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::Base, brush9);
        palette14.setBrush(QPalette::Inactive, QPalette::Window, brush9);
        palette14.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette14.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush2);
        palette14.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush3);
        palette14.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette14.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette14.setBrush(QPalette::Disabled, QPalette::Button, brush9);
        palette14.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette14.setBrush(QPalette::Disabled, QPalette::Midlight, brush2);
        palette14.setBrush(QPalette::Disabled, QPalette::Dark, brush2);
        palette14.setBrush(QPalette::Disabled, QPalette::Mid, brush2);
        palette14.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette14.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette14.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette14.setBrush(QPalette::Disabled, QPalette::Base, brush9);
        palette14.setBrush(QPalette::Disabled, QPalette::Window, brush9);
        palette14.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette14.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush2);
        palette14.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush3);
        palette14.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        label_range->setPalette(palette14);
        label_range->setStyleSheet(QStringLiteral("font: 14pt \"MS Sans Serif\";"));
        label_range->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_range, 3, 0, 1, 1);

        toolButton_zoom_in = new QCustomButton(groupBox_18);
        toolButton_zoom_in->setObjectName(QStringLiteral("toolButton_zoom_in"));
        sizePolicy1.setHeightForWidth(toolButton_zoom_in->sizePolicy().hasHeightForWidth());
        toolButton_zoom_in->setSizePolicy(sizePolicy1);
        toolButton_zoom_in->setFont(font2);
        toolButton_zoom_in->setCheckable(false);

        gridLayout->addWidget(toolButton_zoom_in, 3, 1, 1, 1);

        toolButton_auto_adapt = new QCustomButton(groupBox_18);
        toolButton_auto_adapt->setObjectName(QStringLiteral("toolButton_auto_adapt"));
        sizePolicy1.setHeightForWidth(toolButton_auto_adapt->sizePolicy().hasHeightForWidth());
        toolButton_auto_adapt->setSizePolicy(sizePolicy1);
        toolButton_auto_adapt->setCheckable(true);

        gridLayout->addWidget(toolButton_auto_adapt, 4, 0, 1, 1);

        toolButton_setRangeUnit = new QCustomButton(groupBox_18);
        toolButton_setRangeUnit->setObjectName(QStringLiteral("toolButton_setRangeUnit"));
        sizePolicy1.setHeightForWidth(toolButton_setRangeUnit->sizePolicy().hasHeightForWidth());
        toolButton_setRangeUnit->setSizePolicy(sizePolicy1);
        toolButton_setRangeUnit->setFont(font);
        toolButton_setRangeUnit->setCheckable(false);

        gridLayout->addWidget(toolButton_setRangeUnit, 0, 0, 1, 2);

#ifndef QT_NO_SHORTCUT
        label_sea->setBuddy(horizontalSlider_sea);
        label_rain->setBuddy(horizontalSlider_rain);
        label_gain->setBuddy(horizontalSlider_gain);
#endif // QT_NO_SHORTCUT

        retranslateUi(DialogMenuDisplay);

        comboBox_3->setCurrentIndex(0);
        groupBox_3->setCurrentIndex(1);
        comboBox->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(DialogMenuDisplay);
    } // setupUi

    void retranslateUi(QDialog *DialogMenuDisplay)
    {
        DialogMenuDisplay->setWindowTitle(QApplication::translate("DialogMenuDisplay", "Dialog", Q_NULLPTR));
        groupBox_17->setTitle(QApplication::translate("DialogMenuDisplay", "B\341\272\243n \304\221\341\273\223 s\341\273\221", Q_NULLPTR));
        label_command_3->setText(QApplication::translate("DialogMenuDisplay", "D\341\273\257 li\341\273\207u:", Q_NULLPTR));
        comboBox_3->clear();
        comboBox_3->insertItems(0, QStringList()
         << QApplication::translate("DialogMenuDisplay", "B\341\272\243n \304\221\341\273\223 c\306\241 b\341\272\243n", Q_NULLPTR)
         << QApplication::translate("DialogMenuDisplay", "\304\220\341\273\213a h\303\254nh  ", Q_NULLPTR)
         << QApplication::translate("DialogMenuDisplay", "\341\272\242nh v\341\273\207 tinh ", Q_NULLPTR)
        );
        comboBox_3->setCurrentText(QApplication::translate("DialogMenuDisplay", "B\341\272\243n \304\221\341\273\223 c\306\241 b\341\272\243n", Q_NULLPTR));
        label_command_4->setText(QApplication::translate("DialogMenuDisplay", "\304\220\341\273\231 s\303\241ng:", Q_NULLPTR));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("DialogMenuDisplay", "Xung d\303\240i", Q_NULLPTR)
         << QApplication::translate("DialogMenuDisplay", "Xung \304\221\306\241n 150", Q_NULLPTR)
         << QApplication::translate("DialogMenuDisplay", "Xung \304\221\306\241n 200", Q_NULLPTR)
         << QApplication::translate("DialogMenuDisplay", "Max xung \304\221\306\241n 150", Q_NULLPTR)
         << QApplication::translate("DialogMenuDisplay", "Max xung \304\221\306\241n 200", Q_NULLPTR)
        );
        label_record_file_name_2->setText(QApplication::translate("DialogMenuDisplay", "Gh\303\251p chu k\341\273\263:", Q_NULLPTR));
        toolButton_xl_nguong_3->setText(QApplication::translate("DialogMenuDisplay", "CH n\341\273\201n t\341\272\241p", Q_NULLPTR));
        label_noiseAverrage->setText(QApplication::translate("DialogMenuDisplay", "--", Q_NULLPTR));
        groupBox_3->setTabText(groupBox_3->indexOf(tab), QApplication::translate("DialogMenuDisplay", "TH th\341\273\213 t\341\272\247n", Q_NULLPTR));
        toolButton_xl_nguong->setText(QApplication::translate("DialogMenuDisplay", "L\341\273\215c t\341\273\261 \304\221\341\273\231ng", Q_NULLPTR));
        toolButton_filter2of3->setText(QApplication::translate("DialogMenuDisplay", "L\341\273\215c m\306\260a", Q_NULLPTR));
        toolButton_filter2of3_2->setText(QApplication::translate("DialogMenuDisplay", "L\341\273\215c 2 chi\341\273\201u", Q_NULLPTR));
        toolButton_xl_dopler_2->setText(QApplication::translate("DialogMenuDisplay", "B\341\273\217 b\304\203ng 0 dopler", Q_NULLPTR));
        label_sea->setText(QApplication::translate("DialogMenuDisplay", "Sea", Q_NULLPTR));
        label_rain->setText(QApplication::translate("DialogMenuDisplay", "Rain", Q_NULLPTR));
        label_gain->setText(QApplication::translate("DialogMenuDisplay", "Gain", Q_NULLPTR));
        toolButton_xl_dopler->setText(QApplication::translate("DialogMenuDisplay", "L\341\273\215c t\306\260\306\241ng quan dopler", Q_NULLPTR));
        toolButton_sharp_eye->setText(QApplication::translate("DialogMenuDisplay", "T\306\260\306\241ng ph\341\272\243n dopler", Q_NULLPTR));
        groupBox_3->setTabText(groupBox_3->indexOf(tab2), QApplication::translate("DialogMenuDisplay", "XL s\306\241 c\341\272\245p", Q_NULLPTR));
        comboBox_img_mode->clear();
        comboBox_img_mode->insertItems(0, QStringList()
         << QApplication::translate("DialogMenuDisplay", "Thang bi\303\252n \304\221\341\273\231", Q_NULLPTR)
         << QApplication::translate("DialogMenuDisplay", "Thang \304\221\306\241n m\303\240u", Q_NULLPTR)
         << QApplication::translate("DialogMenuDisplay", "Thang dopler", Q_NULLPTR)
        );
        groupBox_18->setTitle(QApplication::translate("DialogMenuDisplay", "Thang \304\221o ", Q_NULLPTR));
        toolButton_zoom_out->setText(QApplication::translate("DialogMenuDisplay", "+", Q_NULLPTR));
        label_range->setText(QApplication::translate("DialogMenuDisplay", "--", Q_NULLPTR));
        toolButton_zoom_in->setText(QApplication::translate("DialogMenuDisplay", "-", Q_NULLPTR));
        toolButton_auto_adapt->setText(QApplication::translate("DialogMenuDisplay", "PGTN", Q_NULLPTR));
        toolButton_setRangeUnit->setText(QApplication::translate("DialogMenuDisplay", "\304\220\306\241n v\341\273\213 \304\221o: H\341\272\243i l\303\275(NM)", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class DialogMenuDisplay: public Ui_DialogMenuDisplay {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGMENUDISPLAY_H
