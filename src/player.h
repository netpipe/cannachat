#include <mainwindow.h>
#include "ui_mainwindow.h"

void MainWindow::showContextMenuForWidget(const QPoint &pos)
{
    QMenu contextMenu(tr("Context menu"), this);
    contextMenu.addAction(clearAction);
    contextMenu.addAction(scanAction);
    contextMenu.exec(ui->listView->mapToGlobal(pos));
}

void MainWindow::dropEvent(QDropEvent *ev)
{
   QList<QUrl> urls = ev->mimeData()->urls();
   stopScanner();

   foreach(QUrl url, urls)
   {
       qDebug()<<"path:"<< url.path();
       QFileInfo fi(url.path());
       if (fi.isDir()) {
           scanner = new Scanner(fi.dir(),this);
           connect(scanner,SIGNAL(fileAdded(QString)),this,SLOT(onFileAdded(QString)));
           scanner->start();
       }
       if (fi.isFile()) {
           addItem(url.path());
       }
   }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *ev)
{
    ev->accept();
}

void MainWindow::onClearList() {
    stopScanner();
    tracklist.clear();
    trackModel->deleteAllTracks();
    audio->stop();
    ui->horizontalSlider_2->setMaximum(0);
}

void MainWindow::stopScanner() {
    if (scanner!=NULL && scanner->isRunning()) {
        scanner->stopped = true;
        if(!scanner->wait(2000)) {
            scanner->terminate();
            scanner->wait(); //Note: We have to wait again here!
        }
        scanner->deleteLater();
        scanner = NULL;
    }
}

void MainWindow::onFindMusic() {
    stopScanner();
    QDir dir =QDir::home();
    scanner = new Scanner(dir,this);
    connect(scanner,SIGNAL(fileAdded(QString)),this,SLOT(onFileAdded(QString)));
    scanner->start();
}

void MainWindow::addItem(QString s) {

    QFileInfo fi(s);
    Track *track = new Track;
    track->path = s;
    QString dirPath = fi.absoluteDir().absolutePath();
    track->artist = dirPath.split("/").last();
    track->title = fi.completeBaseName();
    track->durationStr = audio->getDuration(s);
    tracklist.append(track);
    trackModel->appendTrack(track);
}

void MainWindow::on_closeButton_clicked()
{
    MainWindow::close();
}

/*void MainWindow::onStartOfPlayback(double total)*/
void MainWindow::onStartOfPlayback()
{
   // ui->horizontalSlider_2->setMaximum(total);
    ui->pushButton_play->setIcon(QIcon(":/Resource/img/btn_pause.png"));
    QModelIndex index = trackModel->index(position, 0);

    if ( index.isValid() ) {
        ui->listView->selectionModel()->clear();
        ui->listView->selectionModel()->select( index, QItemSelectionModel::Select );
        ui->listView->scrollTo(index);
        ui->label_artist->setText(tracklist.at(position)->artist);
        ui->label_title->setText(tracklist.at(position)->title);
    }
}

void MainWindow::onEndOfPlayback() {
    ui->pushButton_play->setIcon(QIcon(":/Resource/img/btn_play.png"));
    nextTrack(true);
}
void MainWindow::seek(int seconds)
{
   //  audio->setPosition(seconds * 1000);
  //   audio->playOrPause(tracklist.at(1)->path);
    // audio->changePosition(seconds * 1000);
  //  audio->play(tracklist.at(0)->path);
    audio->setPosition3(seconds  * 1000);


     qDebug() << "Position" << seconds;
}

void MainWindow::nextTrack(bool next) {
    short add = 1;
    if (!next) add = -1;
    switch (repeatMode) {
    case 1:
        //looped track
        break;
    case 2:
        //looped playlist
        position=position+add;
        if (position>=tracklist.size()) {
            position = 0;
        }
        if (position<0) {
            position = tracklist.size()-1;
        }
        break;
    default:
        position=position+add;
        break;
    }
    if (hasNextTrack()) {
        audio->play(tracklist.at(position)->path);
    }
    else {
        audio->stop();
    }
}

void MainWindow::onPauseOfPlayback() {
    ui->pushButton_play->setIcon(QIcon(":/Resource/img/btn_play.png"));
   // audio->setPosition3(curseconds  * 1000);
}

void MainWindow::onCurPos(double position, double total) {
   //qDebug() << "Position" << position << "Total" << total;
  if (!moving) {
        //ui->horizontalSlider_2->setValue(position*1000);
       // ui->horizontalSlider_2->setRange(0, position / 1000);
        // ui->label_duration_2->setText(audio->formattedTime(position));
    }
}
void MainWindow::onSlidertime(QString sliderduration, qint64 duration, qint64 progress)
{
  if (!moving)
  {
     //qDebug() << "Slider position" << sliderduration;
     ui->horizontalSlider_2->setMaximum(duration);
     ui->horizontalSlider_2->setValue(progress);
     ui->label_bgr->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
     ui->horizontalLayout_4->setContentsMargins(0,0,0,0);
     ui->label_duration_2->setAlignment(Qt::AlignTop | Qt::AlignLeft);
     ui->label_duration_2->setContentsMargins(0,0,0,0);
     ui->label_duration_2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
     ui->label_duration_2->setWordWrap(true);
     ui->label_duration_2->setText(sliderduration);
     m_trackDuration=sliderduration;
  }

}

void MainWindow::on_horizontalSlider_sliderPressed()
{
    moving = true;
}

void MainWindow::on_horizontalSlider_sliderReleased()
{
    moving = false;
    audio->changePosition(ui->horizontalSlider_2->value());
}

void MainWindow::on_pushButton_play_clicked()
{
   // if (hasNextTrack()) {
        audio->positionChanged(curseconds);
        audio->playOrPause(tracklist.at(position)->path);

       // audio->setPosition3(curseconds  * 1000);
   // }
}

bool MainWindow::hasNextTrack() {
    if (tracklist.size()<=0) return false;
    if (position<0) return false;
    if (position>(tracklist.size()-1)) return false;
    return true;
}

void MainWindow::setRepeatModeIcon() {
    if (repeatMode>2) {
        repeatMode = 0;
    }
    switch (repeatMode) {
    case 1:
        ui->pushButton_repeat->setIcon(QIcon(":/Resource/img/btn_repeat_one.png"));
        break;
    case 2:
        ui->pushButton_repeat->setIcon(QIcon(":/Resource/img/btn_repeat_all_highlighted.png"));
        break;
    default:
        ui->pushButton_repeat->setIcon(QIcon(":/Resource/img/btn_repeat_off.png"));
        break;
    }
}

void MainWindow::on_pushButton_repeat_clicked()
{
    repeatMode++;
    setRepeatModeIcon();
}

void MainWindow::on_pushButton_shuffle_clicked()
{
    shuffle = !shuffle;
    if (shuffle) {
        ui->pushButton_shuffle->setIcon(QIcon(":/Resource/img/btn_shuffle_highlighted.png"));
    }
    else {
        ui->pushButton_shuffle->setIcon(QIcon(":/Resource/img/btn_shuffle_off.png"));
    }
}

void MainWindow::on_pushButton_prev_clicked()
{
    nextTrack(false);
}

void MainWindow::on_pushButton_next_clicked()
{
    nextTrack(true);
}

void MainWindow::onFileAdded(QString file) {
    addItem(file);
}

void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    position = index.row();
    if (hasNextTrack()) {
        audio->play(tracklist.at(position)->path);
    }
}
