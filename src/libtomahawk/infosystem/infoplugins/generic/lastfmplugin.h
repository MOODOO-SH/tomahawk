/* === This file is part of Tomahawk Player - <http://tomahawk-player.org> ===
 *
 *   Copyright 2010-2011, Christian Muehlhaeuser <muesli@tomahawk-player.org>
 *
 *   Tomahawk is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Tomahawk is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Tomahawk. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LASTFMPLUGIN_H
#define LASTFMPLUGIN_H
#include "infosystem/infosystem.h"
#include "infosystem/infosystemworker.h"
#include "result.h"

#include <lastfm/Track>
#include <lastfm/Audioscrobbler>
#include <lastfm/ScrobblePoint>

#include <QObject>

class QNetworkReply;

namespace Tomahawk
{

namespace InfoSystem
{

class LastFmPlugin : public InfoPlugin
{
    Q_OBJECT

public:
    LastFmPlugin();
    virtual ~LastFmPlugin();

public slots:
    void settingsChanged();

    void onAuthenticated();
    void coverArtReturned();
    void artistImagesReturned();
    void similarArtistsReturned();
    void topTracksReturned();

    void namChangedSlot( QNetworkAccessManager *nam );

protected slots:
    virtual void getInfo( uint requestId, const QString caller, const Tomahawk::InfoSystem::InfoType type, const QVariant input, const QVariantMap customData );
    virtual void notInCacheSlot( uint requestId, const Tomahawk::InfoSystem::InfoCriteriaHash criteria, const QString caller, const Tomahawk::InfoSystem::InfoType type, const QVariant input, const QVariantMap customData );

    virtual void pushInfo( const QString caller, const Tomahawk::InfoSystem::InfoType type, const QVariant data );

private:
    void fetchCoverArt( uint requestId, const QString &caller, const Tomahawk::InfoSystem::InfoType type, const QVariant &input, const QVariantMap &customData );
    void fetchArtistImages( uint requestId, const QString &caller, const Tomahawk::InfoSystem::InfoType type, const QVariant &input, const QVariantMap &customData );
    void fetchSimilarArtists( uint requestId, const QString &caller, const Tomahawk::InfoSystem::InfoType type, const QVariant &input, const QVariantMap &customData );
    void fetchTopTracks( uint requestId, const QString &caller, const Tomahawk::InfoSystem::InfoType type, const QVariant &input, const QVariantMap &customData );

    void createScrobbler();
    void nowPlaying( const QVariant &input );
    void scrobble();
    void sendLoveSong( const InfoType type, QVariant input );

    void dataError( uint requestId, const QString &caller, const Tomahawk::InfoSystem::InfoType type, const QVariant &input, const QVariantMap &customData );

    lastfm::MutableTrack m_track;
    lastfm::Audioscrobbler* m_scrobbler;
    QString m_pw;

    QList< QUrl > m_badUrls;
};

}

}

#endif // LASTFMPLUGIN_H
