/*****************************************************************************
 * extendedmetamanager.cpp : Extended Metadata Manager dialogs
 ****************************************************************************
 * Copyright (C) 2017 Asier Santos Valcárcel
 * Authors: Asier Santos Valcárcel
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "dialogs/extendedmetamanager.hpp"

#include "input_manager.hpp"
#include "dialogs_provider.hpp" /* THEDP creation */
#include <vlc_playlist.h>  /* playlist_t */

#include "components/interface_widgets.hpp"       /* CoverArtLabel */

// #include <QTabWidget>
// #include <QLabel>
// #include <QGroupBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QMessageBox> //for the Help and About popups

#define UNUSED(x) (void)(x) //TODO: delete this. Unused variable warning removal

ExtMetaManagerDialog::ExtMetaManagerDialog( intf_thread_t *_p_intf)
               : QVLCDialog( (QWidget*)_p_intf->p_sys->p_mi, _p_intf )
{
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] Initializing" ); //TODO: delete this

    // Basic UI setup
    ui.setupUi( this ); //setup the UI from de compiled (.h) version of de QT ui (.ui)
    setWindowFlags( Qt::Tool );
    setWindowRole( "vlc-ext-meta-manager" );
    setWindowTitle( qtr( "Extended Metadata Manager" ) );

    //Button bindings
    BUTTONACT( ui.pushButton_cancel, close() );
    BUTTONACT( ui.pushButton_getFromPlaylist, getFromPlaylist() );
    BUTTONACT( ui.pushButton_getFromFolder, getFromFolder() );
    BUTTONACT( ui.pushButton_searchNow, searchNow() );
    BUTTONACT( ui.pushButton_saveAll, saveAll() );
    BUTTONACT( ui.pushButton_restoreAll, restoreAll() );
    BUTTONACT( ui.pushButton_help, help() );
    BUTTONACT( ui.pushButton_about, about() );
    BUTTONACT( ui.pushButton_clearTable, clearTable() );

    //Events
    CONNECT( ui.tableWidget_metadata, cellClicked(int, int), this, updateArtwork(int, int) );

    //Set de table columns' size
    ui.tableWidget_metadata->setColumnWidth(COL_CHECKBOX, 30); //CheckBox
    ui.tableWidget_metadata->setColumnWidth(COL_TITLE, 200); //Title
    ui.tableWidget_metadata->setColumnWidth(COL_ARTIST, 200); //Artist
    ui.tableWidget_metadata->setColumnWidth(COL_ALBUM, 200); //Album
    ui.tableWidget_metadata->setColumnWidth(COL_GENRE, 120); //Genre
    ui.tableWidget_metadata->setColumnWidth(COL_TRACKNUM, 70); //Track #
    ui.tableWidget_metadata->setColumnWidth(COL_PUBLISHER, 120); //Publisher
    ui.tableWidget_metadata->setColumnWidth(COL_COPYRIGHT, 120); //Copyright
    ui.tableWidget_metadata->setColumnWidth(COL_ARTWORK, 70); //Artwork
    ui.tableWidget_metadata->setColumnWidth(COL_PATH, 50); //Path

    /* ART_URL */
    art_cover = new CoverArtLabel( this, p_intf );
    ui.gridLayout_artwork->layout()->addWidget(art_cover);

    QVLCTools::restoreWidgetPosition( p_intf, "ExtMetaManagerDialog", this );
}

ExtMetaManagerDialog::~ExtMetaManagerDialog()
{
    QVLCTools::saveWidgetPosition( p_intf, "ExtMetaManagerDialog", this );
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] Saving position" ); //TODO: delete this
}

void ExtMetaManagerDialog::toggleVisible()
{
    QVLCDialog::toggleVisible();
    if(isVisible())
        activateWindow();

    msg_Dbg( p_intf, "[ExtMetaManagerDialog] Toggle Visible" ); //TODO: delete this
}

void ExtMetaManagerDialog::close()
{
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] Closing" ); //TODO: delete this
    toggleVisible();
}

void ExtMetaManagerDialog::getFromPlaylist()
{
    clearTable();
    playlist_Lock(THEPL); //Lock the playlist so we can work with it

    int size = THEPL->items.i_size; //Get the size of the playlist
    playlist_item_t *playlist_item;
    input_item_t *input_item;

    for(int i = 4;  i <= size+3; i++) //the list starts at 4 because the first 3 are not files
    {
        playlist_item = playlist_ItemGetById(THEPL, i); // Get the playlist_item
        input_item = playlist_item->p_input; // This creates segmentation fault if playlist_item doesnt exist, be careful
        addTableEntry(input_item); //add item to the table
    }

    playlist_Unlock(THEPL); //Unlock the playlist
}

void ExtMetaManagerDialog::getFromFolder()
{
    // Open a file explorer just with audio files
    QStringList uris = THEDP->showSimpleOpen(
        qtr("Open audio files to manage"),
        EXT_FILTER_AUDIO,
        p_intf->p_sys->filepath );

    if( uris.isEmpty() ) return; //if no files selected, finish

    clearTable();
    foreach( const QString &uri, uris )
    {
        // Get the item from the URI
        input_item_t *p_item = getItemFromURI(uri.toLatin1().constData());

        addTableEntry(p_item);
        //msg_Dbg( p_intf, url.toLatin1() ); //TODO: delete this
    }
}

void ExtMetaManagerDialog::searchNow()
{
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] searchNow" ); //TODO: delete this
}

void ExtMetaManagerDialog::saveAll()
{
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] saveAll" ); //TODO: delete this
}

void ExtMetaManagerDialog::restoreAll()
{
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] restoreAll" ); //TODO: delete this
}

void ExtMetaManagerDialog::help()
{
    QMessageBox::information(
      this,
      tr("Help - Extended Metadata Manager"),
      tr(help_text) );
}

void ExtMetaManagerDialog::about()
{
    QMessageBox::information(
      this,
      tr("About - Extended Metadata Manager"),
      tr(about_text) );
}

void ExtMetaManagerDialog::clearTable()
{
    ui.tableWidget_metadata->clearContents();
    ui.tableWidget_metadata->setRowCount(0);
}

void ExtMetaManagerDialog::addTableEntry(input_item_t *p_item)
{
    // Add one row to the table
    int row =   ui.tableWidget_metadata->rowCount();
    ui.tableWidget_metadata->insertRow(row);

    // input_item_WriteMeta( VLC_OBJECT(THEPL), p_item); //TODO: write/store edited metadata.

    // Get metadata information from item
    char *title_text = input_item_GetTitle(p_item);
    char *artist_text = input_item_GetArtist(p_item);
    char *album_text = input_item_GetAlbum(p_item);
    char *genre_text = input_item_GetGenre(p_item);
    char *trackNum_text = input_item_GetTrackNum(p_item);
    char *publisher_text = input_item_GetPublisher(p_item);
    char *copyright_text = input_item_GetCopyright(p_item);
    char *uri_text = input_item_GetURI(p_item);

    // Create checkbox for the first column
    QTableWidgetItem *item = new QTableWidgetItem();
    item->data(Qt::CheckStateRole);
    item->setCheckState(Qt::Unchecked);

    // Inserte the obtained values in the table
    ui.tableWidget_metadata->setItem(row, COL_CHECKBOX, item );
    ui.tableWidget_metadata->setItem(row, COL_TITLE, new QTableWidgetItem( title_text ));
    ui.tableWidget_metadata->setItem(row, COL_ARTIST, new QTableWidgetItem( artist_text ));
    ui.tableWidget_metadata->setItem(row, COL_ALBUM, new QTableWidgetItem( album_text ));
    ui.tableWidget_metadata->setItem(row, COL_GENRE, new QTableWidgetItem( genre_text ));
    ui.tableWidget_metadata->setItem(row, COL_TRACKNUM, new QTableWidgetItem( trackNum_text ));
    ui.tableWidget_metadata->setItem(row, COL_PUBLISHER, new QTableWidgetItem( publisher_text ));
    ui.tableWidget_metadata->setItem(row, COL_COPYRIGHT, new QTableWidgetItem( copyright_text ));
    ui.tableWidget_metadata->setItem(row, COL_ARTWORK, new QTableWidgetItem( "**Artwork**" )); //TODO: this must be a file chooser
    ui.tableWidget_metadata->setItem(row, COL_PATH, new QTableWidgetItem( uri_text ));
}

void ExtMetaManagerDialog::updateArtwork(int row, int column)
{
    UNUSED(column); //TODO: delete this
    art_cover->setItem(getItemFromRow(row)); // Get the item selected and update the artwork widget
}

input_item_t* ExtMetaManagerDialog::getItemFromRow(int row)
{
    const char* uri = ui.tableWidget_metadata->item(row,COL_PATH)->text().toLatin1();
    input_item_t *item = getItemFromURI(uri);
    return item;
}

input_item_t* ExtMetaManagerDialog::getItemFromURI(const char* uri)
{
    input_item_t *p_item = input_item_New( uri, "Test" ); //TODO: give significant name

    //add to the playlist so it is preparsed (metadata is got)
    playlist_AddInput( THEPL, p_item, false, false );

    //Check it it is preparsed (metadata was added)
    while (!input_item_IsPreparsed(p_item)) ;

    return p_item;
}
