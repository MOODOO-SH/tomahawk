/* === This file is part of Tomahawk Player - <http://tomahawk-player.org> ===
 *
 *   Copyright 2010-2012, Christian Muehlhaeuser <muesli@tomahawk-player.org>
 *   Copyright 2011       Leo Franchi <lfranchi@kde.org>
 *   Copyright 2010-2011, Jeff Mitchell <jeff@tomahawk-player.org>
 *
 *   Tomahawk is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
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

#ifndef PLAYABLEMODEL_H
#define PLAYABLEMODEL_H

#include <QAbstractItemModel>
#include <QPixmap>

#include "PlaylistInterface.h"
#include "Typedefs.h"

#include "DllMacro.h"

class QMetaData;

class PlayableItem;

class DLLEXPORT PlayableModel : public QAbstractItemModel
{
Q_OBJECT

public:
    enum PlayableItemStyle
    { Detailed = 0, Short = 1, ShortWithAvatars = 2, Large = 3, Collection = 4 };

    enum PlayableModelRole
    { StyleRole = Qt::UserRole + 1 };

    enum Columns {
        Artist = 0,
        Track = 1,
        Composer = 2,
        Album = 3,
        AlbumPos = 4,
        Duration = 5,
        Bitrate = 6,
        Age = 7,
        Year = 8,
        Filesize = 9,
        Origin = 10,
        Score = 11,
        Name = 12
    };

    explicit PlayableModel( QObject* parent = 0 );
    virtual ~PlayableModel();

    PlayableModel::PlayableItemStyle style() const { return m_style; }
    void setStyle( PlayableModel::PlayableItemStyle style );

    virtual QModelIndex index( int row, int column, const QModelIndex& parent ) const;
    virtual QModelIndex parent( const QModelIndex& child ) const;

    virtual bool isReadOnly() const { return m_readOnly; }
    virtual void setReadOnly( bool b ) { m_readOnly = b; }
    virtual bool isLoading() const { return m_loading; }

    virtual QString title() const { return m_title; }
    virtual void setTitle( const QString& title ) { m_title = title; }
    virtual QString description() const { return m_description; }
    virtual void setDescription( const QString& description ) { m_description = description; }
    virtual QPixmap icon() const { return m_icon; }
    virtual void setIcon( const QPixmap& pixmap ) { m_icon = pixmap; }

    virtual int trackCount() const { return rowCount( QModelIndex() ); }
    virtual int itemCount() const { return rowCount( QModelIndex() ); }

    virtual int rowCount( const QModelIndex& parent ) const;
    virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual bool hasChildren( const QModelIndex& parent ) const;

    QList< double > columnWeights() const;

    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    virtual QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    
    virtual QVariant artistData( const Tomahawk::artist_ptr& artist, int role = Qt::DisplayRole ) const;
    virtual QVariant albumData( const Tomahawk::album_ptr& album, int role = Qt::DisplayRole ) const;
    virtual QVariant queryData( const Tomahawk::query_ptr&, int column, int role = Qt::DisplayRole ) const;

    virtual QMimeData* mimeData( const QModelIndexList& indexes ) const;
    virtual QStringList mimeTypes() const;
    virtual Qt::DropActions supportedDropActions() const;
    virtual Qt::ItemFlags flags( const QModelIndex& index ) const;

    virtual QPersistentModelIndex currentItem() { return m_currentIndex; }
    virtual Tomahawk::QID currentItemUuid() { return m_currentUuid; }

    virtual Tomahawk::PlaylistModes::RepeatMode repeatMode() const { return Tomahawk::PlaylistModes::NoRepeat; }
    virtual bool shuffled() const { return false; }

    virtual void ensureResolved();

    PlayableItem* itemFromIndex( const QModelIndex& index ) const;
    /// Returns a flat list of all tracks in this model
    QList< Tomahawk::query_ptr > queries() const;

    void updateDetailedInfo( const QModelIndex& index );
    
    void setItemSize( const QSize& size ) { m_itemSize = size; }

signals:
    void repeatModeChanged( Tomahawk::PlaylistModes::RepeatMode mode );
    void shuffleModeChanged( bool enabled );

    void trackCountChanged( unsigned int tracks );
    void itemCountChanged( unsigned int items );

    void loadingStarted();
    void loadingFinished();

public slots:
    virtual void setCurrentItem( const QModelIndex& index );

    virtual void clear();

    virtual void append( const QList< Tomahawk::query_ptr >& queries );
    virtual void append( const QList< Tomahawk::artist_ptr >& artists );
    virtual void append( const QList< Tomahawk::album_ptr >& albums );
    virtual void append( const Tomahawk::query_ptr& query );
    virtual void append( const Tomahawk::artist_ptr& artist );
    virtual void append( const Tomahawk::album_ptr& album );

    virtual void insert( const QList< Tomahawk::query_ptr >& queries, int row = 0 );
    virtual void insert( const QList< Tomahawk::artist_ptr >& artists, int row = 0 );
    virtual void insert( const QList< Tomahawk::album_ptr >& albums, int row = 0 );
    virtual void insert( const Tomahawk::query_ptr& query, int row = 0 );
    virtual void insert( const Tomahawk::artist_ptr& artist, int row = 0 );
    virtual void insert( const Tomahawk::album_ptr& album, int row = 0 );

    virtual void remove( int row, bool moreToCome = false );
    virtual void remove( const QModelIndex& index, bool moreToCome = false );
    virtual void remove( const QList<QModelIndex>& indexes );
    virtual void remove( const QList<QPersistentModelIndex>& indexes );

    virtual void setRepeatMode( Tomahawk::PlaylistModes::RepeatMode /*mode*/ ) {}
    virtual void setShuffled( bool /*shuffled*/ ) {}

protected:
    PlayableItem* rootItem() const { return m_rootItem; }
    void startLoading();
    void finishLoading();

private slots:
    void onDataChanged();

    void onPlaybackStarted( const Tomahawk::result_ptr& result );
    void onPlaybackStopped();

private:
    template <typename T>
    void insertInternal( const QList< T >& items, int row );
    template <typename T>
    void insertInternal( const T& item, int row );

    Qt::Alignment columnAlignment( int column ) const;

    PlayableItem* m_rootItem;
    QPersistentModelIndex m_currentIndex;
    Tomahawk::QID m_currentUuid;
    QSize m_itemSize;

    bool m_readOnly;

    QString m_title;
    QString m_description;
    QPixmap m_icon;

    QHash< PlayableItemStyle, QList<Columns> > m_headerStyle;
    QStringList m_header;

    PlayableItemStyle m_style;
    bool m_loading;
};

#endif // PLAYABLEMODEL_H
