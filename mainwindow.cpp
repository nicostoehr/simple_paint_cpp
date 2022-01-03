#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QDir>
#include <QTextStream>
#include <QFile>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QVector<QVector<int>> polygon_liste;
QVector<bool> polygon_liste_abgeschlossen;
QVector<QVector<int>> ellipsen_liste;
QVector<QVector<int>> rechteck_liste;
QVector<QVector<int>> stern_liste;
QVector<QString> text_text_liste;
QVector<QVector<int>> text_int_liste;

QString speicher_pfad = "";

bool falsche_koordinaten = false;
bool mouse_is_clicked = false;
bool bereits_gezeichnet = true;
bool clear_click = false;

bool tool_polygon = false;
bool tool_ellipse = false;
bool tool_rechteck = false;
bool tool_stern = false;
bool tool_text = false;
bool tool_pan = false;

bool new_poly = false;

int cursorX = 0;
int cursorY = 0;
int cursorXdist = 0;
int cursorYdist = 0;

int poly_last_point_x = 0;
int poly_last_point_y = 0;
int poly_first_point_x = 0;
int poly_first_point_y = 0;

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //---------altzeichnungen----------
    //rechtecke zeichnen:
    if(rechteck_liste.length() > 0){
        for(int i = 0; i < rechteck_liste.length(); i++){
            painter.drawRect(rechteck_liste[i][0],rechteck_liste[i][1],rechteck_liste[i][2],rechteck_liste[i][3]);
        }
    }
    //ellipse zeichnen
    if(ellipsen_liste.length() > 0){
        for(int i = 0; i < ellipsen_liste.length(); i++){
            painter.drawEllipse(ellipsen_liste[i][0],ellipsen_liste[i][1],ellipsen_liste[i][2],ellipsen_liste[i][3]);
        }
    }
    //text zeichnen
    if(text_text_liste.length() > 0){
        for(int i = 0; i < text_text_liste.length(); i++){
            painter.drawText(text_int_liste[i][0],text_int_liste[i][1],text_text_liste[i]);
        }
    }
    //polygon zeichnen
    if(polygon_liste.length() > 0){

        for(int i = 0; i < polygon_liste.length(); i++){

            for(int n = 0; n < ((polygon_liste[i].length())/2); n++){


                if(n==0){

                    painter.drawPoint(polygon_liste[i][0],polygon_liste[i][1]);

                }
                else{
                    painter.drawPoint(polygon_liste[i][n*2],polygon_liste[i][((n*2)+1)]);
                    painter.drawLine(polygon_liste[i][n*2],polygon_liste[i][((n*2)+1)],polygon_liste[i][((n*2)-2)],polygon_liste[i][(((n*2)+1)-2)]);
                }
                if(polygon_liste_abgeschlossen[i] == true && n == (((polygon_liste[i].length())/2)-1) && n > 0){
                    painter.drawPoint(polygon_liste[i][n*2],polygon_liste[i][(n*2)+1]);
                    painter.drawLine(polygon_liste[i][n*2],polygon_liste[i][(n*2)+1],polygon_liste[i][0],polygon_liste[i][1]);
                }
            }
        }
    }




    //---------neuzeichnungen----------
    if(tool_polygon){

        if(bereits_gezeichnet == false){
            if(poly_last_point_x == 0 && poly_last_point_y == 0 && new_poly){

                //falls neues poly
                qDebug() << "new polygon added";
                poly_first_point_x = cursorX;
                poly_first_point_y = cursorY;
                QVector<int> neues_polygon;
                neues_polygon.append(cursorX);
                neues_polygon.append(cursorY);
                polygon_liste.append(neues_polygon);
                polygon_liste_abgeschlossen.append(false);

            }
            else if(new_poly){

                qDebug() << polygon_liste.length();
                qDebug() << "polygon point added";
                qDebug() << polygon_liste[(polygon_liste.length()-1)][0] << polygon_liste[(polygon_liste.length()-1)][1];
                polygon_liste[(polygon_liste.length()-1)].append(cursorX);
                polygon_liste[(polygon_liste.length()-1)].append(cursorY);
            }
        }
        if(clear_click == false){

            if(poly_last_point_x == 0 && poly_last_point_y == 0 && new_poly){


                painter.drawPoint(cursorX, cursorY);
                poly_last_point_x = cursorX;
                poly_last_point_y = cursorY;

            }
            else if(new_poly){

                qDebug() << "point b";

                painter.drawPoint(cursorX, cursorY);
                painter.drawLine(cursorX, cursorY, poly_last_point_x, poly_last_point_y);
                poly_last_point_x = cursorX;
                poly_last_point_y = cursorY;
            }
        }


        clear_click = false;
        bereits_gezeichnet = true;
    }
    else if(tool_ellipse){
        if(bereits_gezeichnet == false){
            QVector<int> neuer_kreis;
            neuer_kreis.append(cursorX);
            neuer_kreis.append(cursorY);
            neuer_kreis.append(cursorXdist);
            neuer_kreis.append(cursorYdist);
            ellipsen_liste.append(neuer_kreis);
        }
        if(clear_click == false){
            painter.drawEllipse(cursorX,cursorY,cursorXdist,cursorYdist);
        }
        clear_click = false;
        bereits_gezeichnet = true;
    }
    else if(tool_rechteck){
        if(bereits_gezeichnet == false){
            QVector<int> neues_rechteck;
            neues_rechteck.append(cursorX);
            neues_rechteck.append(cursorY);
            neues_rechteck.append(cursorXdist);
            neues_rechteck.append(cursorYdist);
            rechteck_liste.append(neues_rechteck);
        }
        if(clear_click == false){
            painter.drawRect(cursorX,cursorY,cursorXdist,cursorYdist);
        }
        clear_click = false;
        bereits_gezeichnet = true;
    }
    else if(tool_stern){
        if(bereits_gezeichnet == false){

        }
        clear_click = false;
        bereits_gezeichnet = true;
    }
    else if(tool_text){
        if(bereits_gezeichnet == false){
            QVector<int> neue_text_koords;
            text_text_liste.append(ui->lineEdit->text());
            neue_text_koords.append(cursorX);
            neue_text_koords.append(cursorY);
            text_int_liste.append(neue_text_koords);
        }
        if(clear_click == false){
            painter.drawText(cursorX,cursorY,ui->lineEdit->text());
        }
        clear_click = false;
        bereits_gezeichnet = true;
    }




    //-----------painter beenden----------------
    painter.end();

}



void MainWindow::mousePressEvent(QMouseEvent *event){


    if(tool_stern || tool_ellipse || tool_rechteck){

        mouse_is_clicked = true;
        qDebug() << "set:";
        QPoint cursorPos = event->pos();
        if(cursorPos.x() >= 92){
            cursorX = cursorPos.x();
            qDebug() << "x: " << cursorX;
        }
        else{
            cursorX = 92;
            qDebug() << "x: " << cursorX;
        }
        if(cursorPos.y() >= 69){
            cursorY = cursorPos.y();
            qDebug() << "y: " << cursorY;
        }
        else{
            cursorY = 69;
            qDebug() << "y: " << cursorY;
        }
    }



    else if(tool_text){

        mouse_is_clicked = true;
        qDebug() << "set:";
        QPoint cursorPos = event->pos();
        if(cursorPos.x() >= 92){
            cursorX = cursorPos.x();
            qDebug() << "x: " << cursorX;
        }
        else{
            cursorX = 92;
            qDebug() << "x: " << cursorX;
        }
        if(cursorPos.y() >= 79){
            cursorY = cursorPos.y();
            qDebug() << "y: " << cursorY;
        }
        else{
            cursorY = 79;
            qDebug() << "y: " << cursorY;
        }
        MainWindow::repaint();
    }




    else if(tool_polygon){

        mouse_is_clicked = true;

        if(event->button() == Qt::LeftButton){

            qDebug() << "set:";
            QPoint cursorPos = event->pos();
            if(cursorPos.x() >= 92){
                cursorX = cursorPos.x();
                qDebug() << "x: " << cursorX;
            }
            else{
                cursorX = 92;
                qDebug() << "x: " << cursorX;
            }
            if(cursorPos.y() >= 79){
                cursorY = cursorPos.y();
                qDebug() << "y: " << cursorY;
            }
            else{
                cursorY = 79;
                qDebug() << "y: " << cursorY;
            }
        }
    }


    else if(tool_pan){
        mouse_is_clicked = true;


        qDebug() << "set:";
        QPoint cursorPos = event->pos();
        if(cursorPos.x() >= 92){
            cursorX = cursorPos.x();
            qDebug() << "x: " << cursorX;
        }
        else{
            cursorX = 92;
            qDebug() << "x: " << cursorX;
        }
        if(cursorPos.y() >= 79){
            cursorY = cursorPos.y();
            qDebug() << "y: " << cursorY;
        }
        else{
            cursorY = 79;
            qDebug() << "y: " << cursorY;
        }

    }
}




void MainWindow::mouseReleaseEvent(QMouseEvent *event){

    if(tool_stern || tool_ellipse || tool_rechteck){

        mouse_is_clicked = false;
        bereits_gezeichnet = false;
        qDebug() << "stopped:";
        QPoint cursorPos = event->pos();
        if(cursorPos.x() >= 92){
            cursorXdist = cursorPos.x() - cursorX;
            qDebug() << "x_dist: " << cursorXdist;
        }
        if(cursorPos.y() >= 69){
            cursorYdist = cursorPos.y() - cursorY;
            qDebug() << "y_dist: " << cursorYdist;
        }
        MainWindow::repaint();
    }



    else if(tool_text){

        mouse_is_clicked = false;
        bereits_gezeichnet = false;
        qDebug() << "stopped:";
        QPoint cursorPos = event->pos();
        if(cursorPos.x() >= 92){
            cursorX = cursorPos.x();
            qDebug() << "x: " << cursorX;
        }
        else{
            cursorX = 92;
            qDebug() << "x: " << cursorX;
        }
        if(cursorPos.y() >= 79){
            cursorY = cursorPos.y();
            qDebug() << "y: " << cursorY;
        }
        else{
            cursorY = 79;
            qDebug() << "y: " << cursorY;
        }
        cursorXdist = 0;
        cursorYdist = 0;
        MainWindow::repaint();

    }

    else if(tool_polygon){

        mouse_is_clicked = false;
        bereits_gezeichnet = false;
        if(event->button() == Qt::LeftButton){
            qDebug() << "stopped:";
            QPoint cursorPos = event->pos();
            if(cursorPos.x() >= 92){
                cursorX = cursorPos.x();
                qDebug() << "x: " << cursorX;
            }
            else{
                cursorX = 92;
                qDebug() << "x: " << cursorX;
            }
            if(cursorPos.y() >= 79){
                cursorY = cursorPos.y();
                qDebug() << "y: " << cursorY;
            }
            else{
                cursorY = 79;
                qDebug() << "y: " << cursorY;
            }
            new_poly = true;
            MainWindow::repaint();
        }
        else if(event->button() == Qt::RightButton){
            poly_last_point_x = 0;
            poly_last_point_y = 0;
            polygon_liste_abgeschlossen[(polygon_liste_abgeschlossen.length())-1] = true;
            new_poly = false;
            MainWindow::repaint();
        }
    }
}






void MainWindow::mouseMoveEvent(QMouseEvent *event){

    // tool_polygon ||
    if(tool_stern || tool_ellipse || tool_rechteck){

        qDebug() << "moving:";
        QPoint cursorPos = event->pos();
        if(cursorPos.x() >= 92){
            cursorXdist = cursorPos.x() - cursorX;
            qDebug() << "x_dist: " << cursorXdist;
        }
        if(cursorPos.y() >= 69){
            cursorYdist = cursorPos.y() - cursorY;
            qDebug() << "y_dist: " << cursorYdist;
        }
        MainWindow::repaint();
    }




    else if(tool_text){
        qDebug() << "set:";
        QPoint cursorPos = event->pos();
        if(cursorPos.x() >= 92){
            cursorX = cursorPos.x();
            qDebug() << "x: " << cursorX;
        }
        else{
            cursorX = 92;
            qDebug() << "x: " << cursorX;
        }
        if(cursorPos.y() >= 79){
            cursorY = cursorPos.y();
            qDebug() << "y: " << cursorY;
        }
        else{
            cursorY = 79;
            qDebug() << "y: " << cursorY;
        }
        cursorXdist = 0;
        cursorYdist = 0;
        MainWindow::repaint();
    }
    else if(tool_polygon){
        qDebug() << "set:";
        QPoint cursorPos = event->pos();
        if(cursorPos.x() >= 92){
            cursorX = cursorPos.x();
            qDebug() << "x: " << cursorX;
        }
        else{
            cursorX = 92;
            qDebug() << "x: " << cursorX;
        }
        if(cursorPos.y() >= 79){
            cursorY = cursorPos.y();
            qDebug() << "y: " << cursorY;
        }
        else{
            cursorY = 79;
            qDebug() << "y: " << cursorY;
        }
        cursorXdist = 0;
        cursorYdist = 0;

    }


    else if(tool_pan){

        qDebug() << "moving:";
        QPoint cursorPos = event->pos();
        if(cursorPos.x() >= 92){
            cursorXdist = cursorPos.x() - cursorX;
            qDebug() << "x_dist: " << cursorXdist;
            cursorX = cursorPos.x();
        }
        if(cursorPos.y() >= 69){
            cursorYdist = cursorPos.y() - cursorY;
            qDebug() << "y_dist: " << cursorYdist;
            cursorY = cursorPos.y();
        }
        MainWindow::repaint();

        if(polygon_liste.length() > 0){
            for(int i = 0; i < polygon_liste.length(); i++){
                for(int n = 0; n <polygon_liste[i].length(); n++){
                    if(n%2 == 0){
                        polygon_liste[i][n] = polygon_liste[i][n]+cursorXdist;
                    }
                    else{
                        polygon_liste[i][n] = polygon_liste[i][n]+cursorYdist;
                    }
                }
            }
        }

        if(ellipsen_liste.length() > 0){
            for(int i = 0; i < ellipsen_liste.length(); i++){
                ellipsen_liste[i][0] = (ellipsen_liste[i][0])+cursorXdist;
                ellipsen_liste[i][1] = (ellipsen_liste[i][1])+cursorYdist;
            }
        }

        if(rechteck_liste.length() > 0){
            for(int i = 0; i < rechteck_liste.length(); i++){
                rechteck_liste[i][0] = (rechteck_liste[i][0])+cursorXdist;
                rechteck_liste[i][1] = (rechteck_liste[i][1])+cursorYdist;
            }

        }

        if(text_int_liste.length() > 0){
            for(int i = 0; i < text_int_liste.length(); i++){
                text_int_liste[i][0] = (text_int_liste[i][0])+cursorXdist;
                text_int_liste[i][1] = (text_int_liste[i][1])+cursorYdist;
            }

        }
        MainWindow::repaint();
    }
}









void MainWindow::on_pushButton_3_clicked()
{
    QMessageBox::information(this, "EIS - Draw 2020", "Dies ist der Lösungsansatz zum EIS Draw Programm (Abschlussprüfung JGU Einführung in die Softwareentwicklung SoSem 2020).");
}

void MainWindow::on_pushButton_5_clicked()
{
    //überprüfung ob jetzt gerad enoch ein angefangenes polygon besteht
    qDebug() << "deleting...";
    if(polygon_liste_abgeschlossen.length() > 0){
        if(tool_polygon && polygon_liste_abgeschlossen[((polygon_liste_abgeschlossen.length())-1)] == false){
            qDebug() << "open polygon deleting...";
            poly_last_point_x = 0;
            poly_last_point_y = 0;
            polygon_liste_abgeschlossen[(polygon_liste_abgeschlossen.length())-1] = true;
            new_poly = false;
        }
    }

    if(polygon_liste_abgeschlossen.length() > 0 || rechteck_liste.length() > 0 || ellipsen_liste.length() > 0 || text_text_liste.length() > 0){
        QMessageBox::StandardButton handle = QMessageBox::question(this, "Wollen Sie ihr aktuelles Projekt vorher Speichern?", "Sie haben ein geöffnetes Projekt, wollen Sie dieses speichern, damit die Daten nicht verloren gehen?",
                              QMessageBox::Yes | QMessageBox::No);
        if(handle == QMessageBox::Yes){
            if(speicher_pfad == ""){

                speicher_pfad = QFileDialog::getSaveFileName(this, "Wählen Sie einen Dateinamen!","C://", "EIS Datei (*.eis)");
                QFile Datei(speicher_pfad);

                if(!Datei.open(QFile::WriteOnly | QFile::Text)){
                    QMessageBox::warning(this, "Fehler", "Datei wurde nicht gefunden! Verwenden Sie Speichern unter.");
                }
                else{
                    QTextStream Write(&Datei);
                    if(rechteck_liste.length() > 0){
                        Write << "r:" << rechteck_liste.length() << ":";
                        for(int i = 0; i < rechteck_liste.length(); i++){
                            Write << rechteck_liste[i][0] << ":" << rechteck_liste[i][1] << ":" << rechteck_liste[i][2] << ":" << rechteck_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(ellipsen_liste.length() > 0){
                        Write << "e:" << ellipsen_liste.length() << ":";
                        for(int i = 0; i < ellipsen_liste.length(); i++){
                            Write << ellipsen_liste[i][0] << ":" << ellipsen_liste[i][1] << ":" << ellipsen_liste[i][2] << ":" << ellipsen_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(text_text_liste.length() > 0){
                        Write << "t:" << text_text_liste.length() << ":";
                        for(int i = 0; i < text_text_liste.length(); i++){
                            Write << text_int_liste[i][0] << ":" << text_int_liste[i][1] << ":" << text_text_liste[i] << ":";
                        }
                        Write << "$:";
                    }
                    if(polygon_liste_abgeschlossen.length() > 0){
                        for(int i = 0; i < polygon_liste_abgeschlossen.length(); i++){
                            Write << "p:" << polygon_liste[i].length() << ":";
                            for(int n = 0; n < polygon_liste[i].length(); n++){
                                Write << polygon_liste[i][n] << ":";
                            }
                        }
                        Write << "$:";
                    }
                }
                Datei.flush();

            }

            else{

                QFile Datei(speicher_pfad);

                if(!Datei.open(QFile::WriteOnly | QFile::Text)){
                    QMessageBox::warning(this, "Fehler", "Datei wurde nicht gefunden! Verwenden Sie Speichern unter.");
                }
                else{
                    QTextStream Write(&Datei);
                    if(rechteck_liste.length() > 0){
                        Write << "r:" << rechteck_liste.length() << ":";
                        for(int i = 0; i < rechteck_liste.length(); i++){
                            Write << rechteck_liste[i][0] << ":" << rechteck_liste[i][1] << ":" << rechteck_liste[i][2] << ":" << rechteck_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(ellipsen_liste.length() > 0){
                        Write << "e:" << ellipsen_liste.length() << ":";
                        for(int i = 0; i < ellipsen_liste.length(); i++){
                            Write << ellipsen_liste[i][0] << ":" << ellipsen_liste[i][1] << ":" << ellipsen_liste[i][2] << ":" << ellipsen_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(text_text_liste.length() > 0){
                        Write << "t:" << text_text_liste.length() << ":";
                        for(int i = 0; i < text_text_liste.length(); i++){
                            Write << text_int_liste[i][0] << ":" << text_int_liste[i][1] << ":" << text_text_liste[i] << ":";
                        }
                        Write << "$:";
                    }
                    if(polygon_liste_abgeschlossen.length() > 0){
                        for(int i = 0; i < polygon_liste_abgeschlossen.length(); i++){
                            Write << "p:" << polygon_liste[i].length() << ":";
                            for(int n = 0; n < polygon_liste[i].length(); n++){
                                Write << polygon_liste[i][n] << ":";
                            }
                        }
                        Write << "$:";
                    }
                }
                Datei.flush();
            }
        }
    }
    QApplication::quit();
}

void MainWindow::on_actionInfo_triggered()
{
    QMessageBox::information(this, "EIS - Draw 2020", "Dies ist der Lösungsansatz zum EIS Draw Programm (Abschlussprüfung JGU Einführung in die Softwareentwicklung SoSem 2020).");
}

void MainWindow::on_actionSchlie_en_triggered()
{
    //überprüfung ob jetzt gerad enoch ein angefangenes polygon besteht
    qDebug() << "deleting...";
    if(polygon_liste_abgeschlossen.length() > 0){
        if(tool_polygon && polygon_liste_abgeschlossen[((polygon_liste_abgeschlossen.length())-1)] == false){
            qDebug() << "open polygon deleting...";
            poly_last_point_x = 0;
            poly_last_point_y = 0;
            polygon_liste_abgeschlossen[(polygon_liste_abgeschlossen.length())-1] = true;
            new_poly = false;
        }
    }

    if(polygon_liste_abgeschlossen.length() > 0 || rechteck_liste.length() > 0 || ellipsen_liste.length() > 0 || text_text_liste.length() > 0){
        QMessageBox::StandardButton handle = QMessageBox::question(this, "Wollen Sie ihr aktuelles Projekt vorher Speichern?", "Sie haben ein geöffnetes Projekt, wollen Sie dieses speichern, damit die Daten nicht verloren gehen?",
                              QMessageBox::Yes | QMessageBox::No);
        if(handle == QMessageBox::Yes){
            if(speicher_pfad == ""){

                speicher_pfad = QFileDialog::getSaveFileName(this, "Wählen Sie einen Dateinamen!","C://", "EIS Datei (*.eis)");
                QFile Datei(speicher_pfad);

                if(!Datei.open(QFile::WriteOnly | QFile::Text)){
                    QMessageBox::warning(this, "Fehler", "Datei wurde nicht gefunden! Verwenden Sie Speichern unter.");
                }
                else{
                    QTextStream Write(&Datei);
                    if(rechteck_liste.length() > 0){
                        Write << "r:" << rechteck_liste.length() << ":";
                        for(int i = 0; i < rechteck_liste.length(); i++){
                            Write << rechteck_liste[i][0] << ":" << rechteck_liste[i][1] << ":" << rechteck_liste[i][2] << ":" << rechteck_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(ellipsen_liste.length() > 0){
                        Write << "e:" << ellipsen_liste.length() << ":";
                        for(int i = 0; i < ellipsen_liste.length(); i++){
                            Write << ellipsen_liste[i][0] << ":" << ellipsen_liste[i][1] << ":" << ellipsen_liste[i][2] << ":" << ellipsen_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(text_text_liste.length() > 0){
                        Write << "t:" << text_text_liste.length() << ":";
                        for(int i = 0; i < text_text_liste.length(); i++){
                            Write << text_int_liste[i][0] << ":" << text_int_liste[i][1] << ":" << text_text_liste[i] << ":";
                        }
                        Write << "$:";
                    }
                    if(polygon_liste_abgeschlossen.length() > 0){
                        for(int i = 0; i < polygon_liste_abgeschlossen.length(); i++){
                            Write << "p:" << polygon_liste[i].length() << ":";
                            for(int n = 0; n < polygon_liste[i].length(); n++){
                                Write << polygon_liste[i][n] << ":";
                            }
                        }
                        Write << "$:";
                    }
                }
                Datei.flush();

            }

            else{

                QFile Datei(speicher_pfad);

                if(!Datei.open(QFile::WriteOnly | QFile::Text)){
                    QMessageBox::warning(this, "Fehler", "Datei wurde nicht gefunden! Verwenden Sie Speichern unter.");
                }
                else{
                    QTextStream Write(&Datei);
                    if(rechteck_liste.length() > 0){
                        Write << "r:" << rechteck_liste.length() << ":";
                        for(int i = 0; i < rechteck_liste.length(); i++){
                            Write << rechteck_liste[i][0] << ":" << rechteck_liste[i][1] << ":" << rechteck_liste[i][2] << ":" << rechteck_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(ellipsen_liste.length() > 0){
                        Write << "e:" << ellipsen_liste.length() << ":";
                        for(int i = 0; i < ellipsen_liste.length(); i++){
                            Write << ellipsen_liste[i][0] << ":" << ellipsen_liste[i][1] << ":" << ellipsen_liste[i][2] << ":" << ellipsen_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(text_text_liste.length() > 0){
                        Write << "t:" << text_text_liste.length() << ":";
                        for(int i = 0; i < text_text_liste.length(); i++){
                            Write << text_int_liste[i][0] << ":" << text_int_liste[i][1] << ":" << text_text_liste[i] << ":";
                        }
                        Write << "$:";
                    }
                    if(polygon_liste_abgeschlossen.length() > 0){
                        for(int i = 0; i < polygon_liste_abgeschlossen.length(); i++){
                            Write << "p:" << polygon_liste[i].length() << ":";
                            for(int n = 0; n < polygon_liste[i].length(); n++){
                                Write << polygon_liste[i][n] << ":";
                            }
                        }
                        Write << "$:";
                    }
                }
                Datei.flush();
            }
        }
    }
    QApplication::quit();
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->plainTextEdit->setPlainText(" EIS DRAW 2020 Infofenster\n\nWilkommen bei EIS-Draw 2020!\n\nSie haben das Polygon-Werkzeug ausgewählt!\n\n - Mit der linken Maustaste klicken um einen Punkt zu setzen.\n\n - Mit der rechten Maustaste klicken um das Polygon zu schließen. ");
    tool_polygon = true;
    tool_ellipse = false;
    tool_rechteck = false;
    tool_stern = false;
    tool_text = false;
    tool_pan = false;
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->plainTextEdit->setPlainText(" EIS DRAW 2020 Infofenster\n\nWilkommen bei EIS-Draw 2020!\n\nSie haben das Ellipse-Werkzeug ausgewählt!\n\n - Mit der linken Maustaste klicken um den Bezugspunkt zu setzen.\n\n - Durch das Ziehen mit der gedrückten linken Maustaste die Form anpassen.");
    tool_polygon = false;
    tool_ellipse = true;
    tool_rechteck = false;
    tool_stern = false;
    tool_text = false;
    tool_pan = false;
}

void MainWindow::on_pushButton_8_clicked()
{
    ui->plainTextEdit->setPlainText(" EIS DRAW 2020 Infofenster\n\nWilkommen bei EIS-Draw 2020!\n\nSie haben das Rechteck-Werkzeug ausgewählt!\n\n - Mit der linken Maustaste klicken um den Bezugspunkt zu setzen.\n\n - Durch das Ziehen mit der gedrückten linken Maustaste die Form anpassen.");
    tool_polygon = false;
    tool_ellipse = false;
    tool_rechteck = true;
    tool_stern = false;
    tool_text = false;
    tool_pan = false;
}

void MainWindow::on_pushButton_9_clicked()
{
    ui->plainTextEdit->setPlainText(" EIS DRAW 2020 Infofenster\n\nWilkommen bei EIS-Draw 2020!\n\nSie haben das Stern-Werkzeug ausgewählt!\n\n - Mit der linken Maustaste klicken und gedrückt halten um den Bezugspunkt zu setzen.\n\n - Durch das Ziehen mit der gedrückten linken Maustaste die Form anpassen.\n\n\n\n\n DAS STERN-TOOL HAT ES NICHT RECHTZEITIG IN DIE FINALE VERSION GESCHAFT UND HAT DESHALB KEINE FUNKTIONALITÄT.");
    tool_polygon = false;
    tool_ellipse = false;
    tool_rechteck = false;
    tool_stern = true;
    tool_text = false;
    tool_pan = false;
}

void MainWindow::on_pushButton_10_clicked()
{
    ui->plainTextEdit->setPlainText(" EIS DRAW 2020 Infofenster\n\nWilkommen bei EIS-Draw 2020!\n\nSie haben das Text-Werkzeug ausgewählt!\n\n - Den gewünschten Text in die Zeile überhalb dieser Infobox eingeben.\n\n - Mit der linken Maustaste klicken und gedrückt halten um die Position des Textes zu wählen.");
    tool_polygon = false;
    tool_ellipse = false;
    tool_rechteck = false;
    tool_stern = false;
    tool_text = true;
    tool_pan = false;
}

void MainWindow::on_pushButton_4_clicked()
{

    //überprüfung ob jetzt gerad enoch ein angefangenes polygon besteht
    qDebug() << "deleting...";
    if(polygon_liste_abgeschlossen.length() > 0){
        if(tool_polygon && polygon_liste_abgeschlossen[((polygon_liste_abgeschlossen.length())-1)] == false){
            qDebug() << "open polygon deleting...";
            poly_last_point_x = 0;
            poly_last_point_y = 0;
            polygon_liste_abgeschlossen[(polygon_liste_abgeschlossen.length())-1] = true;
            new_poly = false;
        }
    }

    if(polygon_liste_abgeschlossen.length() > 0 || rechteck_liste.length() > 0 || ellipsen_liste.length() > 0 || text_text_liste.length() > 0){
        QMessageBox::StandardButton handle = QMessageBox::question(this, "Wollen Sie ihr aktuelles Projekt vorher Speichern?", "Sie haben ein geöffnetes Projekt, wollen Sie dieses speichern, damit die Daten nicht verloren gehen?",
                              QMessageBox::Yes | QMessageBox::No);
        if(handle == QMessageBox::Yes){
            if(speicher_pfad == ""){

                speicher_pfad = QFileDialog::getSaveFileName(this, "Wählen Sie einen Dateinamen!","C://", "EIS Datei (*.eis)");
                QFile Datei(speicher_pfad);

                if(!Datei.open(QFile::WriteOnly | QFile::Text)){
                    QMessageBox::warning(this, "Fehler", "Datei wurde nicht gefunden! Verwenden Sie Speichern unter.");
                }
                else{
                    QTextStream Write(&Datei);
                    if(rechteck_liste.length() > 0){
                        Write << "r:" << rechteck_liste.length() << ":";
                        for(int i = 0; i < rechteck_liste.length(); i++){
                            Write << rechteck_liste[i][0] << ":" << rechteck_liste[i][1] << ":" << rechteck_liste[i][2] << ":" << rechteck_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(ellipsen_liste.length() > 0){
                        Write << "e:" << ellipsen_liste.length() << ":";
                        for(int i = 0; i < ellipsen_liste.length(); i++){
                            Write << ellipsen_liste[i][0] << ":" << ellipsen_liste[i][1] << ":" << ellipsen_liste[i][2] << ":" << ellipsen_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(text_text_liste.length() > 0){
                        Write << "t:" << text_text_liste.length() << ":";
                        for(int i = 0; i < text_text_liste.length(); i++){
                            Write << text_int_liste[i][0] << ":" << text_int_liste[i][1] << ":" << text_text_liste[i] << ":";
                        }
                        Write << "$:";
                    }
                    if(polygon_liste_abgeschlossen.length() > 0){
                        for(int i = 0; i < polygon_liste_abgeschlossen.length(); i++){
                            Write << "p:" << polygon_liste[i].length() << ":";
                            for(int n = 0; n < polygon_liste[i].length(); n++){
                                Write << polygon_liste[i][n] << ":";
                            }
                        }
                        Write << "$:";
                    }
                }
                Datei.flush();

            }

            else{

                QFile Datei(speicher_pfad);

                if(!Datei.open(QFile::WriteOnly | QFile::Text)){
                    QMessageBox::warning(this, "Fehler", "Datei wurde nicht gefunden! Verwenden Sie Speichern unter.");
                }
                else{
                    QTextStream Write(&Datei);
                    if(rechteck_liste.length() > 0){
                        Write << "r:" << rechteck_liste.length() << ":";
                        for(int i = 0; i < rechteck_liste.length(); i++){
                            Write << rechteck_liste[i][0] << ":" << rechteck_liste[i][1] << ":" << rechteck_liste[i][2] << ":" << rechteck_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(ellipsen_liste.length() > 0){
                        Write << "e:" << ellipsen_liste.length() << ":";
                        for(int i = 0; i < ellipsen_liste.length(); i++){
                            Write << ellipsen_liste[i][0] << ":" << ellipsen_liste[i][1] << ":" << ellipsen_liste[i][2] << ":" << ellipsen_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(text_text_liste.length() > 0){
                        Write << "t:" << text_text_liste.length() << ":";
                        for(int i = 0; i < text_text_liste.length(); i++){
                            Write << text_int_liste[i][0] << ":" << text_int_liste[i][1] << ":" << text_text_liste[i] << ":";
                        }
                        Write << "$:";
                    }
                    if(polygon_liste_abgeschlossen.length() > 0){
                        for(int i = 0; i < polygon_liste_abgeschlossen.length(); i++){
                            Write << "p:" << polygon_liste[i].length() << ":";
                            for(int n = 0; n < polygon_liste[i].length(); n++){
                                Write << polygon_liste[i][n] << ":";
                            }
                        }
                        Write << "$:";
                    }
                }
                Datei.flush();
            }
        }
        qDebug() << "deleting...";
        if(polygon_liste_abgeschlossen.length() > 0){
            if(tool_polygon && polygon_liste_abgeschlossen[((polygon_liste_abgeschlossen.length())-1)] == false){
                qDebug() << "open polygon deleting...";
                poly_last_point_x = 0;
                poly_last_point_y = 0;
                polygon_liste_abgeschlossen[(polygon_liste_abgeschlossen.length())-1] = true;
                new_poly = false;
            }
        }
        //löschung aller gezeichneten objekte
        speicher_pfad = "";
        polygon_liste.clear();
        polygon_liste_abgeschlossen.clear();
        ellipsen_liste.clear();
        rechteck_liste.clear();
        stern_liste.clear();
        text_text_liste.clear();
        text_int_liste.clear();
        //bildschirm update
        clear_click = true;
        clear_click = true;
        cursorX=0;
        cursorY=0;
        cursorXdist=0;
        cursorYdist=0;
        MainWindow::repaint();
    }
}


void MainWindow::on_pushButton_11_clicked()
{
    ui->plainTextEdit->setPlainText(" EIS DRAW 2020 Infofenster\n\nWilkommen bei EIS-Draw 2020!\n\nSie haben das Pan-Werkzeug ausgewählt!\n\n - Mit der linken Maustaste irgendwo auf den Canvas klicken und gedrückt halten. Die Positionsveränderung der grdrückten Maus wird dementsprechend auf die Zeichnungen übertragen.");
    tool_polygon = false;
    tool_ellipse = false;
    tool_rechteck = false;
    tool_stern = false;
    tool_text = false;
    tool_pan = true;
}

//laden
void MainWindow::on_pushButton_clicked()
{
    tool_polygon = false;
    tool_ellipse = false;
    tool_rechteck = false;
    tool_stern = false;
    tool_text = false;
    tool_pan = false;

    if(polygon_liste_abgeschlossen.length() > 0 || rechteck_liste.length() > 0 || ellipsen_liste.length() > 0 || text_text_liste.length() > 0){
        QMessageBox::StandardButton handle = QMessageBox::question(this, "Wollen Sie ihr aktuelles Projekt vorher Speichern?", "Sie haben ein geöffnetes Projekt, wollen Sie dieses speichern, damit die Daten nicht verloren gehen?",
                              QMessageBox::Yes | QMessageBox::No);
        if(handle == QMessageBox::Yes){
            if(speicher_pfad == ""){

                speicher_pfad = QFileDialog::getSaveFileName(this, "Wählen Sie einen Dateinamen!","C://", "EIS Datei (*.eis)");
                QFile Datei(speicher_pfad);

                if(!Datei.open(QFile::WriteOnly | QFile::Text)){
                    QMessageBox::warning(this, "Fehler", "Datei wurde nicht gefunden! Verwenden Sie Speichern unter.");
                }
                else{
                    QTextStream Write(&Datei);
                    if(rechteck_liste.length() > 0){
                        Write << "r:" << rechteck_liste.length() << ":";
                        for(int i = 0; i < rechteck_liste.length(); i++){
                            Write << rechteck_liste[i][0] << ":" << rechteck_liste[i][1] << ":" << rechteck_liste[i][2] << ":" << rechteck_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(ellipsen_liste.length() > 0){
                        Write << "e:" << ellipsen_liste.length() << ":";
                        for(int i = 0; i < ellipsen_liste.length(); i++){
                            Write << ellipsen_liste[i][0] << ":" << ellipsen_liste[i][1] << ":" << ellipsen_liste[i][2] << ":" << ellipsen_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(text_text_liste.length() > 0){
                        Write << "t:" << text_text_liste.length() << ":";
                        for(int i = 0; i < text_text_liste.length(); i++){
                            Write << text_int_liste[i][0] << ":" << text_int_liste[i][1] << ":" << text_text_liste[i] << ":";
                        }
                        Write << "$:";
                    }
                    if(polygon_liste_abgeschlossen.length() > 0){
                        for(int i = 0; i < polygon_liste_abgeschlossen.length(); i++){
                            Write << "p:" << polygon_liste[i].length() << ":";
                            for(int n = 0; n < polygon_liste[i].length(); n++){
                                Write << polygon_liste[i][n] << ":";
                            }
                        }
                        Write << "$:";
                    }
                }
                Datei.flush();

            }

            else{

                QFile Datei(speicher_pfad);

                if(!Datei.open(QFile::WriteOnly | QFile::Text)){
                    QMessageBox::warning(this, "Fehler", "Datei wurde nicht gefunden! Verwenden Sie Speichern unter.");
                }
                else{
                    QTextStream Write(&Datei);
                    if(rechteck_liste.length() > 0){
                        Write << "r:" << rechteck_liste.length() << ":";
                        for(int i = 0; i < rechteck_liste.length(); i++){
                            Write << rechteck_liste[i][0] << ":" << rechteck_liste[i][1] << ":" << rechteck_liste[i][2] << ":" << rechteck_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(ellipsen_liste.length() > 0){
                        Write << "e:" << ellipsen_liste.length() << ":";
                        for(int i = 0; i < ellipsen_liste.length(); i++){
                            Write << ellipsen_liste[i][0] << ":" << ellipsen_liste[i][1] << ":" << ellipsen_liste[i][2] << ":" << ellipsen_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(text_text_liste.length() > 0){
                        Write << "t:" << text_text_liste.length() << ":";
                        for(int i = 0; i < text_text_liste.length(); i++){
                            Write << text_int_liste[i][0] << ":" << text_int_liste[i][1] << ":" << text_text_liste[i] << ":";
                        }
                        Write << "$:";
                    }
                    if(polygon_liste_abgeschlossen.length() > 0){
                        for(int i = 0; i < polygon_liste_abgeschlossen.length(); i++){
                            Write << "p:" << polygon_liste[i].length() << ":";
                            for(int n = 0; n < polygon_liste[i].length(); n++){
                                Write << polygon_liste[i][n] << ":";
                            }
                        }
                        Write << "$:";
                    }
                }
                Datei.flush();
            }
        }
        else if(handle == QMessageBox::No){

            qDebug() << "deleting...";
            if(polygon_liste_abgeschlossen.length() > 0){
                if(tool_polygon && polygon_liste_abgeschlossen[((polygon_liste_abgeschlossen.length())-1)] == false){
                    qDebug() << "open polygon deleting...";
                    poly_last_point_x = 0;
                    poly_last_point_y = 0;
                    polygon_liste_abgeschlossen[(polygon_liste_abgeschlossen.length())-1] = true;
                    new_poly = false;
                }
            }
            //löschung aller gezeichneten objekte
            polygon_liste.clear();
            polygon_liste_abgeschlossen.clear();
            ellipsen_liste.clear();
            rechteck_liste.clear();
            stern_liste.clear();
            text_text_liste.clear();
            text_int_liste.clear();
            //bildschirm update
            clear_click = true;
            cursorX=0;
            cursorY=0;
            cursorXdist=0;
            cursorYdist=0;
            MainWindow::repaint();
        }
    }

    QString speicher_pfad = QFileDialog::getOpenFileName(this, "Öffnen Sie ein gespeichertes .eis Projekt!","C://","EIS Datei (*.eis)");
    qDebug() << speicher_pfad;

    QFile Ladedatei(speicher_pfad);

    if(!Ladedatei.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Fehler", "Datei wurde nicht gefunden! Versuchen Sie eine andere Datei.");
    }
    else{
            QTextStream Read(&Ladedatei);
            QString Text = Read.readAll();

            QStringList TextListe = Text.split(":");

            qDebug() << TextListe;

            for(int i = 0; i < TextListe.length(); i++){
                if(TextListe[i] == "r"){
                    for(int n = 0; n < TextListe[(i+1)].toInt(); n++){
                        QVector<int> temp;
                        temp.append(TextListe[(((i+2)+(n*4))+0)].toInt());
                        temp.append(TextListe[(((i+2)+(n*4))+1)].toInt());
                        temp.append(TextListe[(((i+2)+(n*4))+2)].toInt());
                        temp.append(TextListe[(((i+2)+(n*4))+3)].toInt());
                        rechteck_liste.append(temp);
                    }
                }
                else if(TextListe[i] == "e"){
                    for(int n = 0; n < TextListe[(i+1)].toInt(); n++){
                        QVector<int> temp;
                        temp.append(TextListe[(((i+2)+(n*4))+0)].toInt());
                        temp.append(TextListe[(((i+2)+(n*4))+1)].toInt());
                        temp.append(TextListe[(((i+2)+(n*4))+2)].toInt());
                        temp.append(TextListe[(((i+2)+(n*4))+3)].toInt());
                        ellipsen_liste.append(temp);
                    }
                }
                else if(TextListe[i] == "t"){
                    for(int n = 0; n < TextListe[(i+1)].toInt(); n++){
                        QVector<int> temp;
                        temp.append(TextListe[(((i+2)+(n*3))+0)].toInt());
                        temp.append(TextListe[(((i+2)+(n*3))+1)].toInt());
                        text_text_liste.append(TextListe[(((i+2)+(n*3))+2)]);
                        text_int_liste.append(temp);
                    }
                }
                else if(TextListe[i] == "p"){
                    QVector<int> temp;
                    for(int k = 0; k < TextListe[(i+1)].toInt(); k++){
                        temp.append(TextListe[(i+2+k)].toInt());
                    }
                    polygon_liste.append(temp);
                    polygon_liste_abgeschlossen.append(true);
                }
            }
            Ladedatei.flush();
            MainWindow::repaint();
        }
}

//speichern
void MainWindow::on_pushButton_2_clicked()
{
    tool_polygon = false;
    tool_ellipse = false;
    tool_rechteck = false;
    tool_stern = false;
    tool_text = false;
    tool_pan = false;

    if(speicher_pfad == ""){

        speicher_pfad = QFileDialog::getSaveFileName(this, "Wählen Sie einen Dateinamen!","C://", "EIS Datei (*.eis)");
        QFile Datei(speicher_pfad);

        if(!Datei.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::warning(this, "Fehler", "Datei wurde nicht gefunden! Verwenden Sie Speichern unter");
        }
        else{
            QTextStream Write(&Datei);
            if(rechteck_liste.length() > 0){
                Write << "r:" << rechteck_liste.length() << ":";
                for(int i = 0; i < rechteck_liste.length(); i++){
                    Write << rechteck_liste[i][0] << ":" << rechteck_liste[i][1] << ":" << rechteck_liste[i][2] << ":" << rechteck_liste[i][3] << ":";
                }
                Write << "$:";
            }
            if(ellipsen_liste.length() > 0){
                Write << "e:" << ellipsen_liste.length() << ":";
                for(int i = 0; i < ellipsen_liste.length(); i++){
                    Write << ellipsen_liste[i][0] << ":" << ellipsen_liste[i][1] << ":" << ellipsen_liste[i][2] << ":" << ellipsen_liste[i][3] << ":";
                }
                Write << "$:";
            }
            if(text_text_liste.length() > 0){
                Write << "t:" << text_text_liste.length() << ":";
                for(int i = 0; i < text_text_liste.length(); i++){
                    Write << text_int_liste[i][0] << ":" << text_int_liste[i][1] << ":" << text_text_liste[i] << ":";
                }
                Write << "$:";
            }
            if(polygon_liste_abgeschlossen.length() > 0){
                for(int i = 0; i < polygon_liste_abgeschlossen.length(); i++){
                    Write << "p:" << polygon_liste[i].length() << ":";
                    for(int n = 0; n < polygon_liste[i].length(); n++){
                        Write << polygon_liste[i][n] << ":";
                    }
                }
                Write << "$:";
            }
        }
        Datei.flush();

    }

    else{

        QFile Datei(speicher_pfad);

        if(!Datei.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::warning(this, "Fehler", "Datei wurde nicht gefunden! Verwenden Sie Speichern unter");
        }
        else{
            QTextStream Write(&Datei);
            if(rechteck_liste.length() > 0){
                Write << "r:" << rechteck_liste.length() << ":";
                for(int i = 0; i < rechteck_liste.length(); i++){
                    Write << rechteck_liste[i][0] << ":" << rechteck_liste[i][1] << ":" << rechteck_liste[i][2] << ":" << rechteck_liste[i][3] << ":";
                }
                Write << "$:";
            }
            if(ellipsen_liste.length() > 0){
                Write << "e:" << ellipsen_liste.length() << ":";
                for(int i = 0; i < ellipsen_liste.length(); i++){
                    Write << ellipsen_liste[i][0] << ":" << ellipsen_liste[i][1] << ":" << ellipsen_liste[i][2] << ":" << ellipsen_liste[i][3] << ":";
                }
                Write << "$:";
            }
            if(text_text_liste.length() > 0){
                Write << "t:" << text_text_liste.length() << ":";
                for(int i = 0; i < text_text_liste.length(); i++){
                    Write << text_int_liste[i][0] << ":" << text_int_liste[i][1] << ":" << text_text_liste[i] << ":";
                }
                Write << "$:";
            }
            if(polygon_liste_abgeschlossen.length() > 0){
                for(int i = 0; i < polygon_liste_abgeschlossen.length(); i++){
                    Write << "p:" << polygon_liste[i].length() << ":";
                    for(int n = 0; n < polygon_liste[i].length(); n++){
                        Write << polygon_liste[i][n] << ":";
                    }
                }
                Write << "$:";
            }
        }
        Datei.flush();
    }
}



//speichern unter
void MainWindow::on_pushButton_12_clicked()
{
    tool_polygon = false;
    tool_ellipse = false;
    tool_rechteck = false;
    tool_stern = false;
    tool_text = false;
    tool_pan = false;

    speicher_pfad = QFileDialog::getSaveFileName(this, "Wählen Sie einen Dateinamen!","C://", "EIS Datei (*.eis)");
    QFile Datei(speicher_pfad);

    if(!Datei.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Fehler", "Datei wurde nicht gefunden! Verwenden Sie Speichern unter");
    }
    else{
        QTextStream Write(&Datei);
        if(rechteck_liste.length() > 0){
            Write << "r:" << rechteck_liste.length() << ":";
            for(int i = 0; i < rechteck_liste.length(); i++){
                Write << rechteck_liste[i][0] << ":" << rechteck_liste[i][1] << ":" << rechteck_liste[i][2] << ":" << rechteck_liste[i][3] << ":";
            }
            Write << "$:";
        }
        if(ellipsen_liste.length() > 0){
            Write << "e:" << ellipsen_liste.length() << ":";
            for(int i = 0; i < ellipsen_liste.length(); i++){
                Write << ellipsen_liste[i][0] << ":" << ellipsen_liste[i][1] << ":" << ellipsen_liste[i][2] << ":" << ellipsen_liste[i][3] << ":";
            }
            Write << "$:";
        }
        if(text_text_liste.length() > 0){
            Write << "t:" << text_text_liste.length() << ":";
            for(int i = 0; i < text_text_liste.length(); i++){
                Write << text_int_liste[i][0] << ":" << text_int_liste[i][1] << ":" << text_text_liste[i] << ":";
            }
            Write << "$:";
        }
        if(polygon_liste_abgeschlossen.length() > 0){
            for(int i = 0; i < polygon_liste_abgeschlossen.length(); i++){
                Write << "p:" << polygon_liste[i].length() << ":";
                for(int n = 0; n < polygon_liste[i].length(); n++){
                    Write << polygon_liste[i][n] << ":";
                }
            }
            Write << "$:";
        }
    }
    Datei.flush();

}


void MainWindow::on_actionSpeichern_triggered()
{
    tool_polygon = false;
    tool_ellipse = false;
    tool_rechteck = false;
    tool_stern = false;
    tool_text = false;
    tool_pan = false;

    if(speicher_pfad == ""){

        speicher_pfad = QFileDialog::getSaveFileName(this, "Wählen Sie einen Dateinamen!","C://", "EIS Datei (*.eis)");
        QFile Datei(speicher_pfad);

        if(!Datei.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::warning(this, "Fehler", "Datei wurde nicht gefunden! Verwenden Sie Speichern unter");
        }
        else{
            QTextStream Write(&Datei);
            if(rechteck_liste.length() > 0){
                Write << "r:" << rechteck_liste.length() << ":";
                for(int i = 0; i < rechteck_liste.length(); i++){
                    Write << rechteck_liste[i][0] << ":" << rechteck_liste[i][1] << ":" << rechteck_liste[i][2] << ":" << rechteck_liste[i][3] << ":";
                }
                Write << "$:";
            }
            if(ellipsen_liste.length() > 0){
                Write << "e:" << ellipsen_liste.length() << ":";
                for(int i = 0; i < ellipsen_liste.length(); i++){
                    Write << ellipsen_liste[i][0] << ":" << ellipsen_liste[i][1] << ":" << ellipsen_liste[i][2] << ":" << ellipsen_liste[i][3] << ":";
                }
                Write << "$:";
            }
            if(text_text_liste.length() > 0){
                Write << "t:" << text_text_liste.length() << ":";
                for(int i = 0; i < text_text_liste.length(); i++){
                    Write << text_int_liste[i][0] << ":" << text_int_liste[i][1] << ":" << text_text_liste[i] << ":";
                }
                Write << "$:";
            }
            if(polygon_liste_abgeschlossen.length() > 0){
                for(int i = 0; i < polygon_liste_abgeschlossen.length(); i++){
                    Write << "p:" << polygon_liste[i].length() << ":";
                    for(int n = 0; n < polygon_liste[i].length(); n++){
                        Write << polygon_liste[i][n] << ":";
                    }
                }
                Write << "$:";
            }
        }
        Datei.flush();

    }

    else{

        QFile Datei(speicher_pfad);

        if(!Datei.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::warning(this, "Fehler", "Datei wurde nicht gefunden! Verwenden Sie Speichern unter");
        }
        else{
            QTextStream Write(&Datei);
            if(rechteck_liste.length() > 0){
                Write << "r:" << rechteck_liste.length() << ":";
                for(int i = 0; i < rechteck_liste.length(); i++){
                    Write << rechteck_liste[i][0] << ":" << rechteck_liste[i][1] << ":" << rechteck_liste[i][2] << ":" << rechteck_liste[i][3] << ":";
                }
                Write << "$:";
            }
            if(ellipsen_liste.length() > 0){
                Write << "e:" << ellipsen_liste.length() << ":";
                for(int i = 0; i < ellipsen_liste.length(); i++){
                    Write << ellipsen_liste[i][0] << ":" << ellipsen_liste[i][1] << ":" << ellipsen_liste[i][2] << ":" << ellipsen_liste[i][3] << ":";
                }
                Write << "$:";
            }
            if(text_text_liste.length() > 0){
                Write << "t:" << text_text_liste.length() << ":";
                for(int i = 0; i < text_text_liste.length(); i++){
                    Write << text_int_liste[i][0] << ":" << text_int_liste[i][1] << ":" << text_text_liste[i] << ":";
                }
                Write << "$:";
            }
            if(polygon_liste_abgeschlossen.length() > 0){
                for(int i = 0; i < polygon_liste_abgeschlossen.length(); i++){
                    Write << "p:" << polygon_liste[i].length() << ":";
                    for(int n = 0; n < polygon_liste[i].length(); n++){
                        Write << polygon_liste[i][n] << ":";
                    }
                }
                Write << "$:";
            }
        }
        Datei.flush();
    }
}

void MainWindow::on_actionLaden_triggered()
{
    tool_polygon = false;
    tool_ellipse = false;
    tool_rechteck = false;
    tool_stern = false;
    tool_text = false;
    tool_pan = false;

    if(polygon_liste_abgeschlossen.length() > 0 || rechteck_liste.length() > 0 || ellipsen_liste.length() > 0 || text_text_liste.length() > 0){
        QMessageBox::StandardButton handle = QMessageBox::question(this, "Wollen Sie ihr aktuelles Projekt vorher Speichern?", "Sie haben ein geöffnetes Projekt, wollen Sie dieses speichern, damit die Daten nicht verloren gehen?",
                              QMessageBox::Yes | QMessageBox::No);
        if(handle == QMessageBox::Yes){
            if(speicher_pfad == ""){

                speicher_pfad = QFileDialog::getSaveFileName(this, "Wählen Sie einen Dateinamen!","C://", "EIS Datei (*.eis)");
                QFile Datei(speicher_pfad);

                if(!Datei.open(QFile::WriteOnly | QFile::Text)){
                    QMessageBox::warning(this, "Fehler", "Datei wurde nicht gefunden! Verwenden Sie Speichern unter.");
                }
                else{
                    QTextStream Write(&Datei);
                    if(rechteck_liste.length() > 0){
                        Write << "r:" << rechteck_liste.length() << ":";
                        for(int i = 0; i < rechteck_liste.length(); i++){
                            Write << rechteck_liste[i][0] << ":" << rechteck_liste[i][1] << ":" << rechteck_liste[i][2] << ":" << rechteck_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(ellipsen_liste.length() > 0){
                        Write << "e:" << ellipsen_liste.length() << ":";
                        for(int i = 0; i < ellipsen_liste.length(); i++){
                            Write << ellipsen_liste[i][0] << ":" << ellipsen_liste[i][1] << ":" << ellipsen_liste[i][2] << ":" << ellipsen_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(text_text_liste.length() > 0){
                        Write << "t:" << text_text_liste.length() << ":";
                        for(int i = 0; i < text_text_liste.length(); i++){
                            Write << text_int_liste[i][0] << ":" << text_int_liste[i][1] << ":" << text_text_liste[i] << ":";
                        }
                        Write << "$:";
                    }
                    if(polygon_liste_abgeschlossen.length() > 0){
                        for(int i = 0; i < polygon_liste_abgeschlossen.length(); i++){
                            Write << "p:" << polygon_liste[i].length() << ":";
                            for(int n = 0; n < polygon_liste[i].length(); n++){
                                Write << polygon_liste[i][n] << ":";
                            }
                        }
                        Write << "$:";
                    }
                }
                Datei.flush();

            }

            else{

                QFile Datei(speicher_pfad);

                if(!Datei.open(QFile::WriteOnly | QFile::Text)){
                    QMessageBox::warning(this, "Fehler", "Datei wurde nicht gefunden! Verwenden Sie Speichern unter.");
                }
                else{
                    QTextStream Write(&Datei);
                    if(rechteck_liste.length() > 0){
                        Write << "r:" << rechteck_liste.length() << ":";
                        for(int i = 0; i < rechteck_liste.length(); i++){
                            Write << rechteck_liste[i][0] << ":" << rechteck_liste[i][1] << ":" << rechteck_liste[i][2] << ":" << rechteck_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(ellipsen_liste.length() > 0){
                        Write << "e:" << ellipsen_liste.length() << ":";
                        for(int i = 0; i < ellipsen_liste.length(); i++){
                            Write << ellipsen_liste[i][0] << ":" << ellipsen_liste[i][1] << ":" << ellipsen_liste[i][2] << ":" << ellipsen_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(text_text_liste.length() > 0){
                        Write << "t:" << text_text_liste.length() << ":";
                        for(int i = 0; i < text_text_liste.length(); i++){
                            Write << text_int_liste[i][0] << ":" << text_int_liste[i][1] << ":" << text_text_liste[i] << ":";
                        }
                        Write << "$:";
                    }
                    if(polygon_liste_abgeschlossen.length() > 0){
                        for(int i = 0; i < polygon_liste_abgeschlossen.length(); i++){
                            Write << "p:" << polygon_liste[i].length() << ":";
                            for(int n = 0; n < polygon_liste[i].length(); n++){
                                Write << polygon_liste[i][n] << ":";
                            }
                        }
                        Write << "$:";
                    }
                }
                Datei.flush();
            }
        }
        else if(handle == QMessageBox::No){

            qDebug() << "deleting...";
            if(polygon_liste_abgeschlossen.length() > 0){
                if(tool_polygon && polygon_liste_abgeschlossen[((polygon_liste_abgeschlossen.length())-1)] == false){
                    qDebug() << "open polygon deleting...";
                    poly_last_point_x = 0;
                    poly_last_point_y = 0;
                    polygon_liste_abgeschlossen[(polygon_liste_abgeschlossen.length())-1] = true;
                    new_poly = false;
                }
            }
            //löschung aller gezeichneten objekte
            polygon_liste.clear();
            polygon_liste_abgeschlossen.clear();
            ellipsen_liste.clear();
            rechteck_liste.clear();
            stern_liste.clear();
            text_text_liste.clear();
            text_int_liste.clear();
            //bildschirm update
            clear_click = true;
            cursorX=0;
            cursorY=0;
            cursorXdist=0;
            cursorYdist=0;
            MainWindow::repaint();
        }
    }

    QString speicher_pfad = QFileDialog::getOpenFileName(this, "Öffnen Sie ein gespeichertes .eis Projekt!","C://","EIS Datei (*.eis)");
    qDebug() << speicher_pfad;

    QFile Ladedatei(speicher_pfad);

    if(!Ladedatei.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Fehler", "Datei wurde nicht gefunden! Versuchen Sie eine andere Datei.");
    }
    else{
            QTextStream Read(&Ladedatei);
            QString Text = Read.readAll();

            QStringList TextListe = Text.split(":");

            qDebug() << TextListe;

            for(int i = 0; i < TextListe.length(); i++){
                if(TextListe[i] == "r"){
                    for(int n = 0; n < TextListe[(i+1)].toInt(); n++){
                        QVector<int> temp;
                        temp.append(TextListe[(((i+2)+(n*4))+0)].toInt());
                        temp.append(TextListe[(((i+2)+(n*4))+1)].toInt());
                        temp.append(TextListe[(((i+2)+(n*4))+2)].toInt());
                        temp.append(TextListe[(((i+2)+(n*4))+3)].toInt());
                        rechteck_liste.append(temp);
                    }
                }
                else if(TextListe[i] == "e"){
                    for(int n = 0; n < TextListe[(i+1)].toInt(); n++){
                        QVector<int> temp;
                        temp.append(TextListe[(((i+2)+(n*4))+0)].toInt());
                        temp.append(TextListe[(((i+2)+(n*4))+1)].toInt());
                        temp.append(TextListe[(((i+2)+(n*4))+2)].toInt());
                        temp.append(TextListe[(((i+2)+(n*4))+3)].toInt());
                        ellipsen_liste.append(temp);
                    }
                }
                else if(TextListe[i] == "t"){
                    for(int n = 0; n < TextListe[(i+1)].toInt(); n++){
                        QVector<int> temp;
                        temp.append(TextListe[(((i+2)+(n*3))+0)].toInt());
                        temp.append(TextListe[(((i+2)+(n*3))+1)].toInt());
                        text_text_liste.append(TextListe[(((i+2)+(n*3))+2)]);
                        text_int_liste.append(temp);
                    }
                }
                else if(TextListe[i] == "p"){
                    QVector<int> temp;
                    for(int k = 0; k < TextListe[(i+1)].toInt(); k++){
                        temp.append(TextListe[(i+2+k)].toInt());
                    }
                    polygon_liste.append(temp);
                    polygon_liste_abgeschlossen.append(true);
                }
            }
            Ladedatei.flush();
            MainWindow::repaint();
        }
}

void MainWindow::on_actionSpeichern_unter_triggered()
{
    tool_polygon = false;
    tool_ellipse = false;
    tool_rechteck = false;
    tool_stern = false;
    tool_text = false;
    tool_pan = false;

    speicher_pfad = QFileDialog::getSaveFileName(this, "Wählen Sie einen Dateinamen!","C://", "EIS Datei (*.eis)");
    QFile Datei(speicher_pfad);

    if(!Datei.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Fehler", "Datei wurde nicht gefunden! Verwenden Sie Speichern unter");
    }
    else{
        QTextStream Write(&Datei);
        if(rechteck_liste.length() > 0){
            Write << "r:" << rechteck_liste.length() << ":";
            for(int i = 0; i < rechteck_liste.length(); i++){
                Write << rechteck_liste[i][0] << ":" << rechteck_liste[i][1] << ":" << rechteck_liste[i][2] << ":" << rechteck_liste[i][3] << ":";
            }
            Write << "$:";
        }
        if(ellipsen_liste.length() > 0){
            Write << "e:" << ellipsen_liste.length() << ":";
            for(int i = 0; i < ellipsen_liste.length(); i++){
                Write << ellipsen_liste[i][0] << ":" << ellipsen_liste[i][1] << ":" << ellipsen_liste[i][2] << ":" << ellipsen_liste[i][3] << ":";
            }
            Write << "$:";
        }
        if(text_text_liste.length() > 0){
            Write << "t:" << text_text_liste.length() << ":";
            for(int i = 0; i < text_text_liste.length(); i++){
                Write << text_int_liste[i][0] << ":" << text_int_liste[i][1] << ":" << text_text_liste[i] << ":";
            }
            Write << "$:";
        }
        if(polygon_liste_abgeschlossen.length() > 0){
            for(int i = 0; i < polygon_liste_abgeschlossen.length(); i++){
                Write << "p:" << polygon_liste[i].length() << ":";
                for(int n = 0; n < polygon_liste[i].length(); n++){
                    Write << polygon_liste[i][n] << ":";
                }
            }
            Write << "$:";
        }
    }
    Datei.flush();
}

void MainWindow::on_actionNeu_triggered()
{
    //überprüfung ob jetzt gerad enoch ein angefangenes polygon besteht
    qDebug() << "deleting...";
    if(polygon_liste_abgeschlossen.length() > 0){
        if(tool_polygon && polygon_liste_abgeschlossen[((polygon_liste_abgeschlossen.length())-1)] == false){
            qDebug() << "open polygon deleting...";
            poly_last_point_x = 0;
            poly_last_point_y = 0;
            polygon_liste_abgeschlossen[(polygon_liste_abgeschlossen.length())-1] = true;
            new_poly = false;
        }
    }

    if(polygon_liste_abgeschlossen.length() > 0 || rechteck_liste.length() > 0 || ellipsen_liste.length() > 0 || text_text_liste.length() > 0){
        QMessageBox::StandardButton handle = QMessageBox::question(this, "Wollen Sie ihr aktuelles Projekt vorher Speichern?", "Sie haben ein geöffnetes Projekt, wollen Sie dieses speichern, damit die Daten nicht verloren gehen?",
                              QMessageBox::Yes | QMessageBox::No);
        if(handle == QMessageBox::Yes){
            if(speicher_pfad == ""){

                speicher_pfad = QFileDialog::getSaveFileName(this, "Wählen Sie einen Dateinamen!","C://", "EIS Datei (*.eis)");
                QFile Datei(speicher_pfad);

                if(!Datei.open(QFile::WriteOnly | QFile::Text)){
                    QMessageBox::warning(this, "Fehler", "Datei wurde nicht gefunden! Verwenden Sie Speichern unter.");
                }
                else{
                    QTextStream Write(&Datei);
                    if(rechteck_liste.length() > 0){
                        Write << "r:" << rechteck_liste.length() << ":";
                        for(int i = 0; i < rechteck_liste.length(); i++){
                            Write << rechteck_liste[i][0] << ":" << rechteck_liste[i][1] << ":" << rechteck_liste[i][2] << ":" << rechteck_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(ellipsen_liste.length() > 0){
                        Write << "e:" << ellipsen_liste.length() << ":";
                        for(int i = 0; i < ellipsen_liste.length(); i++){
                            Write << ellipsen_liste[i][0] << ":" << ellipsen_liste[i][1] << ":" << ellipsen_liste[i][2] << ":" << ellipsen_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(text_text_liste.length() > 0){
                        Write << "t:" << text_text_liste.length() << ":";
                        for(int i = 0; i < text_text_liste.length(); i++){
                            Write << text_int_liste[i][0] << ":" << text_int_liste[i][1] << ":" << text_text_liste[i] << ":";
                        }
                        Write << "$:";
                    }
                    if(polygon_liste_abgeschlossen.length() > 0){
                        for(int i = 0; i < polygon_liste_abgeschlossen.length(); i++){
                            Write << "p:" << polygon_liste[i].length() << ":";
                            for(int n = 0; n < polygon_liste[i].length(); n++){
                                Write << polygon_liste[i][n] << ":";
                            }
                        }
                        Write << "$:";
                    }
                }
                Datei.flush();

            }

            else{

                QFile Datei(speicher_pfad);

                if(!Datei.open(QFile::WriteOnly | QFile::Text)){
                    QMessageBox::warning(this, "Fehler", "Datei wurde nicht gefunden! Verwenden Sie Speichern unter.");
                }
                else{
                    QTextStream Write(&Datei);
                    if(rechteck_liste.length() > 0){
                        Write << "r:" << rechteck_liste.length() << ":";
                        for(int i = 0; i < rechteck_liste.length(); i++){
                            Write << rechteck_liste[i][0] << ":" << rechteck_liste[i][1] << ":" << rechteck_liste[i][2] << ":" << rechteck_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(ellipsen_liste.length() > 0){
                        Write << "e:" << ellipsen_liste.length() << ":";
                        for(int i = 0; i < ellipsen_liste.length(); i++){
                            Write << ellipsen_liste[i][0] << ":" << ellipsen_liste[i][1] << ":" << ellipsen_liste[i][2] << ":" << ellipsen_liste[i][3] << ":";
                        }
                        Write << "$:";
                    }
                    if(text_text_liste.length() > 0){
                        Write << "t:" << text_text_liste.length() << ":";
                        for(int i = 0; i < text_text_liste.length(); i++){
                            Write << text_int_liste[i][0] << ":" << text_int_liste[i][1] << ":" << text_text_liste[i] << ":";
                        }
                        Write << "$:";
                    }
                    if(polygon_liste_abgeschlossen.length() > 0){
                        for(int i = 0; i < polygon_liste_abgeschlossen.length(); i++){
                            Write << "p:" << polygon_liste[i].length() << ":";
                            for(int n = 0; n < polygon_liste[i].length(); n++){
                                Write << polygon_liste[i][n] << ":";
                            }
                        }
                        Write << "$:";
                    }
                }
                Datei.flush();
            }
        }
        qDebug() << "deleting...";
        if(polygon_liste_abgeschlossen.length() > 0){
            if(tool_polygon && polygon_liste_abgeschlossen[((polygon_liste_abgeschlossen.length())-1)] == false){
                qDebug() << "open polygon deleting...";
                poly_last_point_x = 0;
                poly_last_point_y = 0;
                polygon_liste_abgeschlossen[(polygon_liste_abgeschlossen.length())-1] = true;
                new_poly = false;
            }
        }
        //löschung aller gezeichneten objekte
        speicher_pfad = "";
        polygon_liste.clear();
        polygon_liste_abgeschlossen.clear();
        ellipsen_liste.clear();
        rechteck_liste.clear();
        stern_liste.clear();
        text_text_liste.clear();
        text_int_liste.clear();
        //bildschirm update
        clear_click = true;
        clear_click = true;
        cursorX=0;
        cursorY=0;
        cursorXdist=0;
        cursorYdist=0;
        MainWindow::repaint();
    }
}
