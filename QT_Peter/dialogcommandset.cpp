#include "dialogcommandset.h"
#include "ui_dialogcommandset.h"

DialogCommandSet::DialogCommandSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCommandSet)
{
    ui->setupUi(this);
}

DialogCommandSet::~DialogCommandSet()
{
    delete ui;
}

void DialogCommandSet::on_buttonBox_accepted()
{
    mR0Command = ui->plainTextEdit_range_0->toPlainText();
    mR1Command = ui->plainTextEdit_range_1->toPlainText();
    mR2Command = ui->plainTextEdit_range_2->toPlainText();
    mR3Command = ui->plainTextEdit_range_3->toPlainText();
    mR4Command = ui->plainTextEdit_range_4->toPlainText();
    mR5Command = ui->plainTextEdit_range_5->toPlainText();
    mR6Command = ui->plainTextEdit_range_6->toPlainText();
    mR7Command = ui->plainTextEdit_range_7->toPlainText();

    mRxCommand = ui->plainTextEdit_command_rx->toPlainText();
    mTxCommand = ui->plainTextEdit_command_tx->toPlainText();
    mGlobbalConfig.setValue("mR0Command",mR0Command);
    mGlobbalConfig.setValue("mR1Command",mR1Command);
    mGlobbalConfig.setValue("mR2Command",mR2Command);
    mGlobbalConfig.setValue("mR3Command",mR3Command);
    mGlobbalConfig.setValue("mR4Command",mR4Command);
    mGlobbalConfig.setValue("mR5Command",mR5Command);
    mGlobbalConfig.setValue("mR6Command",mR6Command);
    mGlobbalConfig.setValue("mR7Command",mR7Command);
    mGlobbalConfig.setValue("mRxCommand",mRxCommand);
    mGlobbalConfig.setValue("mTxCommand",mTxCommand);
    //
    mFreq1Command =  ui->plainTextEdit_freq_1->toPlainText();
    mFreq2Command =  ui->plainTextEdit_freq_2->toPlainText();
    mFreq3Command =  ui->plainTextEdit_freq_3->toPlainText();
    mFreq4Command =  ui->plainTextEdit_freq_4->toPlainText();
    mFreq5Command =  ui->plainTextEdit_freq_5->toPlainText();
    mFreq6Command =  ui->plainTextEdit_freq_6->toPlainText();
    //
    mGlobbalConfig.setValue("mFreq1Command",mFreq1Command);
    mGlobbalConfig.setValue("mFreq2Command",mFreq2Command);
    mGlobbalConfig.setValue("mFreq3Command",mFreq3Command);
    mGlobbalConfig.setValue("mFreq4Command",mFreq4Command);
    mGlobbalConfig.setValue("mFreq5Command",mFreq5Command);
    mGlobbalConfig.setValue("mFreq6Command",mFreq6Command);
    ui->groupBox_control_setting->setHidden(true);
    ui->toolButton_set_commands->setChecked(false);

}
