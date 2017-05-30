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

// #include <QTabWidget>
// #include <QLabel>
// #include <QGroupBox>
#include <QDialogButtonBox>
#include <QPushButton>

ExtMetaManagerDialog::ExtMetaManagerDialog( intf_thread_t *_p_intf)
               : QVLCDialog( (QWidget*)_p_intf->p_sys->p_mi, _p_intf )
{
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] Initializing" );
    setWindowFlags( Qt::Tool );
    setWindowTitle( qtr( "Extended Metadata Manager" ) );
    setWindowRole( "vlc-ext-meta-manager" );

    ui.setupUi( this ); //setup the UI from de compiled (.h) version of de QT ui (.ui)


    //TODO: delete this section
    ui.tableWidget_metadata->insertRow(0);
    ui.tableWidget_metadata->insertRow(1);
    ui.tableWidget_metadata->insertRow(2);
    ui.tableWidget_metadata->insertRow(3);
    ui.tableWidget_metadata->insertRow(4);
    ui.tableWidget_metadata->insertRow(5);
    ui.tableWidget_metadata->insertRow(6);
    ui.tableWidget_metadata->insertRow(7);
    ui.tableWidget_metadata->setItem(0, 0, new QTableWidgetItem( "test text 1" ));
    ui.tableWidget_metadata->setItem(0, 1, new QTableWidgetItem( "test text 2" ));
    ui.tableWidget_metadata->setItem(0, 2, new QTableWidgetItem( "test text 3" ));

    //button bindings
    BUTTONACT( ui.pushButton_cancel, cancel() );
    BUTTONACT( ui.pushButton_getFromPlaylist, getFromPlaylist() );
    BUTTONACT( ui.pushButton_getFromFolder, getFromFolder() );
    BUTTONACT( ui.pushButton_searchNow, searchNow() );
    BUTTONACT( ui.pushButton_saveAll, saveAll() );
    BUTTONACT( ui.pushButton_restoreAll, restoreAll() );
    BUTTONACT( ui.pushButton_help, help() );
    BUTTONACT( ui.pushButton_about, about() );

    // QGridLayout *mainLayout = new QGridLayout( this );
    // mainLayout->setSizeConstraint( QLayout::SetFixedSize );
    //
    // QPushButton *cancelButton = new QPushButton( qtr( "&Cancel" ) );
    // QDialogButtonBox *buttonBox = new QDialogButtonBox;
    //
    // buttonBox->addButton( cancelButton, QDialogButtonBox::RejectRole );
    //
    // mainLayout->addWidget( buttonBox, 1, 0, 1, 3 );
    //

    QVLCTools::restoreWidgetPosition( p_intf, "ExtMetaManagerDialog", this );

    msg_Dbg( p_intf, "[ExtMetaManagerDialog] Initialization Finished" ); //TODO: delete this
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

void ExtMetaManagerDialog::cancel()
{
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] Canceling" ); //TODO: delete this
    toggleVisible();
}

void ExtMetaManagerDialog::close()
{
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] Closing" ); //TODO: delete this
    toggleVisible();
}

void ExtMetaManagerDialog::getFromPlaylist()
{
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] getFromPlaylist" ); //TODO: delete this
}

void ExtMetaManagerDialog::getFromFolder()
{
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] getFromFolder" ); //TODO: delete this

    //open a file explorer just with audio files
    QStringList uris = THEDP->showSimpleOpen(
        qtr("Open audio files to manage"),
        EXT_FILTER_AUDIO,
        p_intf->p_sys->filepath );
    clearTable();

    if( uris.isEmpty() ) return; //if no files selected, finish

    foreach( const QString &uri, uris )
    {
        addTableEntry(uri);
        //msg_Dbg( p_intf, url.toLatin1() ); //TODO: delete this
        //qtu(url) may be needed
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
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] help" ); //TODO: delete this
}

void ExtMetaManagerDialog::about()
{
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] about" ); //TODO: delete this
}

void ExtMetaManagerDialog::clearTable()
{
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] clearTable" ); //TODO: delete this
    ui.tableWidget_metadata->clearContents();
}

void ExtMetaManagerDialog::addTableEntry(QString uri)
{
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] addTableEntry" ); //TODO: delete this
    msg_Dbg( p_intf, uri.toLocal8Bit().constData() ); //TODO: delete this

    input_item_t *p_item;
    p_item = input_item_New( uri.toLocal8Bit().constData(), "Entry" );

    // input_item_WriteMeta( VLC_OBJECT(THEPL), p_item); //TODO: write/store edited metadata.

    char *title_text = input_item_GetTitle(p_item);
    char *artist_text = input_item_GetArtist(p_item);
    char *album_text = input_item_GetAlbum(p_item);
    char *genre_text = input_item_GetGenre(p_item);
    char *trackNum_text = input_item_GetTrackNum(p_item);
    char *publisher_text = input_item_GetPublisher(p_item);
    char *copyright_text = input_item_GetCopyright(p_item);


    ui.tableWidget_metadata->setItem(0, 0, new QTableWidgetItem( "**CheckBox**" ));
    ui.tableWidget_metadata->setItem(0, 1, new QTableWidgetItem( title_text ));
    ui.tableWidget_metadata->setItem(0, 2, new QTableWidgetItem( artist_text ));
    ui.tableWidget_metadata->setItem(0, 3, new QTableWidgetItem( album_text ));
    ui.tableWidget_metadata->setItem(0, 4, new QTableWidgetItem( genre_text ));
    ui.tableWidget_metadata->setItem(0, 5, new QTableWidgetItem( trackNum_text ));
    ui.tableWidget_metadata->setItem(0, 6, new QTableWidgetItem( publisher_text ));
    ui.tableWidget_metadata->setItem(0, 7, new QTableWidgetItem( copyright_text ));
    ui.tableWidget_metadata->setItem(0, 8, new QTableWidgetItem( "**Artwork**" ));

}
