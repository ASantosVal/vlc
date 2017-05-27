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

    ui.setupUi( this );


    //TODO: delete this section
    ui.tableWidget_metadata->insertRow(0);
    ui.tableWidget_metadata->insertRow(1);
    ui.tableWidget_metadata->insertRow(2);
    ui.tableWidget_metadata->insertRow(3);
    ui.tableWidget_metadata->insertRow(4);
    ui.tableWidget_metadata->insertRow(5);
    ui.tableWidget_metadata->insertRow(6);
    ui.tableWidget_metadata->insertRow(7);
    ui.tableWidget_metadata->setItem(0, 0, new QTableWidgetItem( "patata1" ));
    ui.tableWidget_metadata->setItem(1, 1, new QTableWidgetItem( "patata2" ));
    ui.tableWidget_metadata->setItem(2, 2, new QTableWidgetItem( "patata3" ));

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

    msg_Dbg( p_intf, "[ExtMetaManagerDialog] Initialization Finished" );
}

ExtMetaManagerDialog::~ExtMetaManagerDialog()
{
    QVLCTools::saveWidgetPosition( p_intf, "ExtMetaManagerDialog", this );
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] Saving position" );
}

void ExtMetaManagerDialog::toggleVisible()
{
    QVLCDialog::toggleVisible();
    if(isVisible())
        activateWindow();

    msg_Dbg( p_intf, "[ExtMetaManagerDialog] Toggle Visible" );
}

void ExtMetaManagerDialog::cancel()
{
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] Canceling" );
    toggleVisible();
}

void ExtMetaManagerDialog::close()
{
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] Closing" );
    toggleVisible();
}

void ExtMetaManagerDialog::getFromPlaylist()
{
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] getFromPlaylist" );
}

void ExtMetaManagerDialog::getFromFolder()
{
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] getFromFolder" );
}

void ExtMetaManagerDialog::searchNow()
{
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] searchNow" );
}

void ExtMetaManagerDialog::saveAll()
{
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] saveAll" );
}

void ExtMetaManagerDialog::restoreAll()
{
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] restoreAll" );
}

void ExtMetaManagerDialog::help()
{
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] help" );
}

void ExtMetaManagerDialog::about()
{
    msg_Dbg( p_intf, "[ExtMetaManagerDialog] about" );
}
