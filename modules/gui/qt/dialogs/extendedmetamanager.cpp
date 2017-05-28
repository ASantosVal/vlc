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
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] Initializing" ); //TODO: delete this
    setWindowFlags( Qt::Tool );
    setWindowTitle( qtr( "Extended Metadata Manager" ) );
    setWindowRole( "vlc-ext-meta-manager" );

    ui.setupUi( this ); //setup the UI from de compiled (.h) version of de QT ui (.ui)

    //button bindings
    BUTTONACT( ui.pushButton_cancel, cancel() );
    BUTTONACT( ui.pushButton_getFromPlaylist, getFromPlaylist() );
    BUTTONACT( ui.pushButton_getFromFolder, getFromFolder() );
    BUTTONACT( ui.pushButton_searchNow, searchNow() );
    BUTTONACT( ui.pushButton_saveAll, saveAll() );
    BUTTONACT( ui.pushButton_restoreAll, restoreAll() );
    BUTTONACT( ui.pushButton_help, help() );
    BUTTONACT( ui.pushButton_about, about() );
    BUTTONACT( ui.pushButton_clearTable, clearTable() );

    //Set de table columns' size
    ui.tableWidget_metadata->setColumnWidth(0, 30); //CheckBox
    ui.tableWidget_metadata->setColumnWidth(1, 200); //Title
    ui.tableWidget_metadata->setColumnWidth(2, 200); //Artist
    ui.tableWidget_metadata->setColumnWidth(3, 200); //Album
    ui.tableWidget_metadata->setColumnWidth(4, 120); //Genre
    ui.tableWidget_metadata->setColumnWidth(5, 70); //Track #
    ui.tableWidget_metadata->setColumnWidth(6, 120); //Publisher
    ui.tableWidget_metadata->setColumnWidth(7, 120); //Copyright
    ui.tableWidget_metadata->setColumnWidth(8, 70); //Artwork

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
    QStringList urls = THEDP->showSimpleOpen(
        qtr("Open audio files to manage"),
        EXT_FILTER_AUDIO,
        p_intf->p_sys->filepath );
    //clearTable();

    foreach( const QString &url, urls )
    {
        addTableEntry(url);
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
    ui.tableWidget_metadata->setRowCount(0);
}

void ExtMetaManagerDialog::addTableEntry(QString url)
{
    int row =   ui.tableWidget_metadata->rowCount();
    ui.tableWidget_metadata->insertRow(row);
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] addTableEntry" ); //TODO: delete this
    ui.tableWidget_metadata->setItem(row, 0, new QTableWidgetItem( "**CheckBox**" ));
    ui.tableWidget_metadata->setItem(row, 1, new QTableWidgetItem( "Title" ));
    ui.tableWidget_metadata->setItem(row, 2, new QTableWidgetItem( "Artist" ));
    ui.tableWidget_metadata->setItem(row, 3, new QTableWidgetItem( "Album" ));
    ui.tableWidget_metadata->setItem(row, 4, new QTableWidgetItem( "Genre" ));
    ui.tableWidget_metadata->setItem(row, 5, new QTableWidgetItem( "Track #" ));
    ui.tableWidget_metadata->setItem(row, 6, new QTableWidgetItem( "Publisher" ));
    ui.tableWidget_metadata->setItem(row, 7, new QTableWidgetItem( "Copyright" ));
    ui.tableWidget_metadata->setItem(row, 8, new QTableWidgetItem( "Artwork" ));

}
