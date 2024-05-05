#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QPixmap"
#include "QScrollBar"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix("E:/Year3/Photo/Logo_BK.png");
    int w = ui -> label_pic -> width();
    int h = ui -> label_pic -> height();
    ui -> label_pic->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    ui-> pushButton_6 -> setEnabled(false);
    ui-> pushButton_4 -> setEnabled(false);
    ui-> pushButton_7 -> setEnabled(false);
    ui-> pushButton_8 -> setEnabled(false);
    ui-> pushButton_9 -> setEnabled(false);
    // Disable maximizing
    setFixedSize(width(), height());

    // Adding title for widget
    QMainWindow::setWindowTitle("Serial Port");

    // Ports
    QList<QSerialPortInfo> ports = info.availablePorts();
    QList<QString> stringPorts;
    for(int i = 0 ; i < ports.size() ; i++){
        stringPorts.append(ports.at(i).portName());
    }
    ui->comboBox->addItems(stringPorts);

    // Baud Rate Ratios
    QList<qint32> baudRates = info.standardBaudRates(); // What baudrates does my computer support ?
    QList<QString> stringBaudRates;
    for(int i = 0 ; i < baudRates.size() ; i++){
        stringBaudRates.append(QString::number(baudRates.at(i)));
    }
    ui->comboBox_2->addItems(stringBaudRates);

    // Data Bits
    ui->comboBox_3->addItem("5");
    ui->comboBox_3->addItem("6");
    ui->comboBox_3->addItem("7");
    ui->comboBox_3->addItem("8");

    // Stop Bits
    ui->comboBox_4->addItem("1 Bit");
    ui->comboBox_4->addItem("1,5 Bits");
    ui->comboBox_4->addItem("2 Bits");

    // Parities
    ui->comboBox_5->addItem("No Parity");
    ui->comboBox_5->addItem("Even Parity");
    ui->comboBox_5->addItem("Odd Parity");
    ui->comboBox_5->addItem("Mark Parity");
    ui->comboBox_5->addItem("Space Parity");

    //Flow Controls
    ui->comboBox_6->addItem("No Flow Control");
    ui->comboBox_6->addItem("Hardware Flow Control");
    ui->comboBox_6->addItem("Software Flow Control");

}

MainWindow::~MainWindow()
{
    delete ui;
}
//nut DISP_TEMP
void MainWindow::on_pushButton_6_clicked()
{
    QByteArray str("02 44 49 53 50 54 45 4D 00 00 00 00 00 01 00 01 16 03");
    QByteArray t = str.replace(" ", "");
    QByteArray bytes = QByteArray::fromHex(t);
    serialPort.write(bytes);
}

//nut DISP_EEPROM
void MainWindow::on_pushButton_4_clicked()
{
    QByteArray str("02 45 45 50 52 4F 4D 53 00 00 00 00 00 01 00 01 16 03");
    QByteArray t = str.replace(" ", "");
    QByteArray bytes = QByteArray::fromHex(t);
    serialPort.write(bytes);
}

// Button MAX7219 LEG_7SEG
void MainWindow::on_pushButton_7_clicked()
{
    QByteArray str("02 37 53 45 47 4C 45 44 00 00 00 00 00 01 00 01 16 03"); //(7SEGLEG)
    QByteArray t = str.replace(" ", "");
    QByteArray bytes = QByteArray::fromHex(t);
    serialPort.write(bytes);
}
//nut OFF_LED
void MainWindow::on_pushButton_8_clicked()
{
    QByteArray str("02 4F 46 46 50 43 31 33 00 00 00 00 00 01 00 01 16 03"); //OFFPC13
    QByteArray t = str.replace(" ", "");
    QByteArray bytes = QByteArray::fromHex(t);
    serialPort.write(bytes);
}
//nut ON_LED
void MainWindow::on_pushButton_9_clicked()
{
    QByteArray str("02 4F 4E 31 33 4C 45 44 00 00 00 00 00 01 00 01 16 03"); //ON13LED
    QByteArray t = str.replace(" ", "");
    QByteArray bytes = QByteArray::fromHex(t);
    serialPort.write(bytes);
}
//nut connect
void MainWindow::on_pushButton_2_clicked()
{


    QString portName = ui->comboBox->currentText();
    serialPort.setPortName(portName);

    serialPort.open(QIODevice::ReadWrite);
    if(!serialPort.isOpen()){
        ui->textBrowser->setTextColor(Qt::red);
        ui->textBrowser->append("!!!! Something went Wrong !!!!");
    }
    else {
        ui-> pushButton_6 -> setEnabled(true);
        ui-> pushButton_4 -> setEnabled(true);
        ui-> pushButton_7 -> setEnabled(true);
        ui-> pushButton_8 -> setEnabled(true);
        ui-> pushButton_9 -> setEnabled(true);
        QString stringbaudRate = ui->comboBox_2->currentText();
        int intbaudRate = stringbaudRate.toInt();
        serialPort.setBaudRate(intbaudRate);

        QString dataBits = ui->comboBox_3->currentText();
        if(dataBits == "5 Bits") {
            serialPort.setDataBits(QSerialPort::Data5);
        }
        else if((dataBits == "6 Bits")) {
            serialPort.setDataBits(QSerialPort::Data6);
        }
        else if(dataBits == "7 Bits") {
            serialPort.setDataBits(QSerialPort::Data7);
        }
        else if(dataBits == "8 Bits"){
            serialPort.setDataBits(QSerialPort::Data8);
        }

        QString stopBits = ui->comboBox_4->currentText();
        if(stopBits == "1 Bit") {
            serialPort.setStopBits(QSerialPort::OneStop);
        }
        else if(stopBits == "1,5 Bits") {
            serialPort.setStopBits(QSerialPort::OneAndHalfStop);
        }
        else if(stopBits == "2 Bits") {
            serialPort.setStopBits(QSerialPort::TwoStop);
        }

        QString parity = ui->comboBox_5->currentText();
        if(parity == "No Parity"){
            serialPort.setParity(QSerialPort::NoParity);
        }
        else if(parity == "Even Parity"){
            serialPort.setParity(QSerialPort::EvenParity);
        }
        else if(parity == "Odd Parity"){
            serialPort.setParity(QSerialPort::OddParity);
        }
        else if(parity == "Mark Parity"){
            serialPort.setParity(QSerialPort::MarkParity);
        }
        else if(parity == "Space Parity") {
            serialPort.setParity(QSerialPort::SpaceParity);
        }


        QString flowControl = ui->comboBox_6->currentText();
        if(flowControl == "No Flow Control") {
            serialPort.setFlowControl(QSerialPort::NoFlowControl);
        }
        else if(flowControl == "Hardware Flow Control") {
            serialPort.setFlowControl(QSerialPort::HardwareControl);
        }
        else if(flowControl == "Software Flow Control") {
            serialPort.setFlowControl(QSerialPort::SoftwareControl);
        }
       connect(&serialPort,SIGNAL(readyRead()),this,SLOT(receiveMessage()));
    }
}

/*void MainWindow::receiveMessage()
{
    QByteArray dataBA = serialPort.readAll();
    QString data(dataBA);
    ui->textBrowser->setTextColor(Qt::blue); // Receieved message's color is blue.
    ui->textBrowser->insertPlainText(data);
    QScrollBar *sb = ui->textBrowser->verticalScrollBar();
    sb->setValue(sb->maximum());

}*/
QByteArray accumulatedData;

void MainWindow::receiveMessage()
{
    // Read available data from the serial port
    QByteArray receivedData = serialPort.readAll();

    // Append the received data to the accumulated data
    accumulatedData.append(receivedData);
    qDebug() << accumulatedData << "\n" ;
    QString cmd = "Command received: ";
    QString data = "Data received: ";
    // Check if a complete frame is received (e.g., ending with ETX)
    if (accumulatedData.endsWith("\x03")) { // Assuming ETX is defined as "\x03"
        // Extract command and data parts
        int stxIndex = accumulatedData.indexOf("\x02"); // Find the index of STX
        int colonIndex = accumulatedData.indexOf(":"); // Find the index of ":"

        if (stxIndex != -1 && colonIndex != -1 && colonIndex > stxIndex) {
            QByteArray command = accumulatedData.mid(stxIndex + 1, 6); // Extract 6 bits after STX
            QByteArray data1 = accumulatedData.mid(colonIndex + 1, 5); // Extract 5 bits after ":"

            // Process the command and data parts
            //qDebug() << "Command: " << command;
            //qDebug() << "Data: " << data;
            ui -> textBrowser -> insertPlainText(cmd);
            command.append("\n");
            ui -> textBrowser -> insertPlainText(command);
            ui -> textBrowser -> insertPlainText(data);
            data1.append("\n");
            ui -> textBrowser -> insertPlainText(data1);
        }

        // Clear the accumulated data for the next frame
        accumulatedData.clear();
    }
    else if (accumulatedData.endsWith("\x06")) { // Assuming ETX is defined as "\x03"

        // Extract command and data parts
        int stxIndex = accumulatedData.indexOf("\x02"); // Find the index of STX
        int colonIndex = accumulatedData.indexOf(":"); // Find the index of ":"

        if (stxIndex != -1 && colonIndex != -1 && colonIndex > stxIndex) {
            QByteArray command1 = accumulatedData.mid(stxIndex + 1, 6); // Extract 6 bits after STX
            QByteArray data2 = accumulatedData.mid(colonIndex + 1, 10); // Extract 5 bits after ":"

            // Process the command and data parts
            //qDebug() << "Command: " << command;
            //qDebug() << "Data: " << data;
            ui -> textBrowser -> insertPlainText(cmd);
            command1.append("\n");
            ui -> textBrowser -> insertPlainText(command1);
            ui -> textBrowser -> insertPlainText(data);
            data2.append("\n");
            ui -> textBrowser -> insertPlainText(data2);
        }

        // Clear the accumulated data for the next frame
        accumulatedData.clear();
    }
    else if (accumulatedData.endsWith("*")) { // Assuming ETX is defined as "\x03"
        // Extract command and data parts
        QByteArray command2 = accumulatedData.mid(1, 7); // Extract first 7 bits for command
        QByteArray data3 = accumulatedData.mid(8, 8); // Extract next 8 bits for data

        // Process the command and data parts
        // qDebug() << "Command: " << command;
        // qDebug() << "Data: " << data;
        ui->textBrowser->insertPlainText(cmd);
        command2.append("\n");
        ui->textBrowser->insertPlainText(command2);
        ui->textBrowser->insertPlainText(data);
        data3.append("\n");
        ui->textBrowser->insertPlainText(data3);

        // Clear the accumulated data for the next frame
        accumulatedData.clear();
    }
    else if (accumulatedData.endsWith("O")) { // Assuming ETX is defined as "\x03"
        // Extract command and data parts
        int stxIndex = accumulatedData.indexOf("\x02"); // Find the index of STX
        QByteArray command3 = accumulatedData.mid(stxIndex+8, 7); // Extract first 7 bits for command
        //QByteArray data3 = accumulatedData.mid(8, 8); // Extract next 8 bits for data

        // Process the command and data parts
        // qDebug() << "Command: " << command;
        // qDebug() << "Data: " << data;
        ui->textBrowser->insertPlainText(cmd);
        command3.append("\n");
        ui->textBrowser->insertPlainText(command3);

        // Clear the accumulated data for the next frame
        accumulatedData.clear();
    }
    else if (accumulatedData.endsWith("F")) { // Assuming ETX is defined as "\x03"
        // Extract command and data parts
        int stxIndex = accumulatedData.indexOf("\x02"); // Find the index of STX
        QByteArray command4 = accumulatedData.mid(stxIndex+8, 8); // Extract first 7 bits for command
        //QByteArray data3 = accumulatedData.mid(8, 8); // Extract next 8 bits for data

        // Process the command and data parts
        // qDebug() << "Command: " << command;
        // qDebug() << "Data: " << data;
        ui->textBrowser->insertPlainText(cmd);
        command4.append("\n");
        ui->textBrowser->insertPlainText(command4);

        // Clear the accumulated data for the next frame
        accumulatedData.clear();
    }
}
//nut send
void MainWindow::on_pushButton_clicked()
{
    QString message = ui->lineEdit_2->text();
    ui->textBrowser->setTextColor(Qt::darkGreen); // Color of message to send is green.
    ui->textBrowser->append(message);
    serialPort.write(message.toUtf8());
}

// Button of Disconnect
void MainWindow::on_pushButton_3_clicked()
{
    serialPort.close();
    ui-> pushButton_6 -> setEnabled(false);
    ui-> pushButton_4 -> setEnabled(false);
    ui-> pushButton_7 -> setEnabled(false);
    ui-> pushButton_8 -> setEnabled(false);
    ui-> pushButton_9 -> setEnabled(false);
}
// Button of Refresh Ports

// Button of Clear

void MainWindow::on_pushButton_5_clicked()
{
    ui->textBrowser->clear();
}












