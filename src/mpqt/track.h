#ifndef TRACK_H
#define TRACK_H
#include <QtCore>
#ifdef MEDIAPLAYER
class Track
{
public:
    Track();
    QString artist;
    QString title;
    QString path;
    QString durationStr;
    ~Track();
};
Q_DECLARE_METATYPE(Track *)
#endif // TRACK_H
#endif
