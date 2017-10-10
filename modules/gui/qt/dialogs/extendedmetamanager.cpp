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
#include "components/interface_widgets.hpp"     /* CoverArtLabelExt */
#include "dialogs/fingerprintdialog.hpp"        /* fingerprinting dialog */
#include "adapters/chromaprint.hpp"             /* fingerprinting adapter (no UI)*/

#include <QMessageBox> /* for the Help and About popups */

#define UNUSED(x) (void)(x) //FIXME: delete this. Unused variable warning removal

/* Constructor */
ExtMetaManagerDialog::ExtMetaManagerDialog( intf_thread_t *_p_intf)
               : QVLCDialog( (QWidget*)_p_intf->p_sys->p_mi, _p_intf )
{
    msg_Dbg( p_intf, "[EMM_Dialog] Initializing" );

    /* Basic UI setup */
    ui.setupUi( this ); //setup the UI from de compiled (.h) version of de QT ui (.ui)
    setWindowFlags( Qt::Tool );
    setWindowRole( "vlc-ext-meta-manager" );
    setWindowTitle( qtr( "Extended Metadata Manager" ) );

    /* Set button icons */
    ui.pushButton_getFromPlaylist->setIcon(QIcon(QPixmap (":/toolbar/playlist") ) );
    ui.pushButton_getFromFolder->setIcon(QIcon(QPixmap (":/type/folder-grey") ) );
    ui.pushButton_help->setIcon(QIcon(QPixmap (":/menu/help") ) );
    ui.pushButton_about->setIcon(QIcon(QPixmap (":/menu/info") ) );
    ui.pushButton_searchNow->setIcon(QIcon(QPixmap (":/ext-meta-manager/search") ) );
    ui.pushButton_saveAll->setIcon(QIcon(QPixmap (":/ext-meta-manager/save") ) );
    ui.pushButton_restoreAll->setIcon(QIcon(QPixmap (":/buttons/playlist/repeat_all") ) );
    ui.pushButton_clearTable->setIcon(QIcon(QPixmap (":/ext-meta-manager/clean") ) );
    ui.pushButton_cancel->setIcon(QIcon(QPixmap (":/menu/exit") ) );

    /* Button bindings */
    BUTTONACT( ui.pushButton_getFromPlaylist, getFromPlaylist() );
    BUTTONACT( ui.pushButton_getFromFolder, getFromFolder() );
    BUTTONACT( ui.pushButton_help, helpDialog() );
    BUTTONACT( ui.pushButton_about, aboutDialog() );
    BUTTONACT( ui.pushButton_searchNow, searchNow() );
    BUTTONACT( ui.pushButton_saveAll, saveAll() );
    BUTTONACT( ui.pushButton_restoreAll, restoreAll() );
    BUTTONACT( ui.pushButton_clearTable, cleanUp() );
    BUTTONACT( ui.pushButton_cancel, close() );

    /* Events for the table */
    CONNECT( ui.tableWidget_metadata, cellClicked(int, int), this, updateArtwork(int, int) );
    CONNECT( ui.tableWidget_metadata,  itemChanged(QTableWidgetItem*), this,  multipleItemsChanged(QTableWidgetItem*));

    /* Set de table columns' size */
    ui.tableWidget_metadata->setColumnWidth(COL_CHECKBOX, 30);
    ui.tableWidget_metadata->setColumnWidth(COL_PREVIEW, 70);
    ui.tableWidget_metadata->setColumnWidth(COL_TITLE, 200);
    ui.tableWidget_metadata->setColumnWidth(COL_ARTIST, 200);
    ui.tableWidget_metadata->setColumnWidth(COL_ALBUM, 200);
    ui.tableWidget_metadata->setColumnWidth(COL_GENRE, 120);
    ui.tableWidget_metadata->setColumnWidth(COL_TRACKNUM, 80);
    ui.tableWidget_metadata->setColumnWidth(COL_PUBLISHER, 120);
    ui.tableWidget_metadata->setColumnWidth(COL_COPYRIGHT, 120);
    ui.tableWidget_metadata->setColumnWidth(COL_ARTWORK, 80);
    ui.tableWidget_metadata->setColumnWidth(COL_PATH, 50);

    /* Make table columns able to change order */
    ui.tableWidget_metadata->horizontalHeader()->setSectionsMovable(true);
    //TODO: the above only works on Qt5.x (on Qt4.x it's "ui->tableView->horizontalHeader()->setMovable(true);")

    /* Add the Artwork label */
    art_cover = new CoverArtLabelExt( this, p_intf );
    ui.gridLayout_artwork->layout()->addWidget(art_cover);

    /* Set tips for multiple widgets (extra text when de mouse is over the widget) */
    ui.pushButton_getFromPlaylist->setToolTip(getFromPlaylist_tip);
    ui.pushButton_getFromFolder->setToolTip(getFromFolder_tip);
    ui.pushButton_help->setToolTip(help_tip);
    ui.pushButton_about->setToolTip(about_tip);
    ui.pushButton_searchNow->setToolTip(searchNow_tip);
    ui.pushButton_saveAll->setToolTip(saveAll_tip);
    ui.pushButton_restoreAll->setToolTip(restoreAll_tip);
    ui.pushButton_clearTable->setToolTip(clearTable_tip);
    ui.pushButton_cancel->setToolTip(cancel_tip);
    ui.progressBar_search->setToolTip(progressBar_tip);
    ui.checkBox_disableFastSearch->setToolTip(disableFastSearch_tip);
    art_cover->setToolTip(artwork_tip);

    /* Set the mapper's connection to changeArtwork. This is used to be able
    to know from which row the button (added later on each row) it's being
    clicked */
    connect(&changeArtwork_SignalMapper, SIGNAL(mapped(int)), this, SLOT(changeArtwork(int)));
    connect(&preview_SignalMapper, SIGNAL(mapped(int)), this, SLOT(previewItem(int)));

    /* Initilize the array for the currently working items */
    workspace = new vlc_array_t();
    vlc_array_init(workspace);

    /* Start with the progressBar disabled */
    ui.progressBar_search->setEnabled(false);

    QVLCTools::restoreWidgetPosition( p_intf, "ExtMetaManagerDialog", this );
}
/* Destructor */
ExtMetaManagerDialog::~ExtMetaManagerDialog()
{
    msg_Dbg( p_intf, "[EMM_Dialog] Destroying" );
    QVLCTools::saveWidgetPosition( p_intf, "ExtMetaManagerDialog", this );
}

/* Override the closing (click X) event*/
void ExtMetaManagerDialog::closeEvent(QCloseEvent *event)
{
    msg_Dbg( p_intf, "[EMM_Dialog] Close event" );
    UNUSED(event); //FIXME: delete this
    close();
}

/* Show/hide */
void ExtMetaManagerDialog::toggleVisible()
{
    msg_Dbg( p_intf, "[EMM_Dialog] toggleVisible" );
    QVLCDialog::toggleVisible();
    if(isVisible()) //If changed to shown
        activateWindow();
}

/*----------------------------------------------------------------------------*/
/*-----------------------------Main use cases---------------------------------*/
/*----------------------------------------------------------------------------*/

/* Just closes the window (and the module itself) */
void ExtMetaManagerDialog::close()
{
    toggleVisible();

    /* Clean before closing */
    cleanUp();

    msg_Dbg( p_intf, "[EMM_Dialog] Closing" );
}

/* Loads files into the table from the current playlist */
void ExtMetaManagerDialog::getFromPlaylist()
{
    msg_Dbg( p_intf, "[EMM_Dialog] getFromPlaylist" );

    /* Clean before changing the workspace (if it's not an append operation) */
    if (isClearSelected()){
        cleanUp();
    }

    /* We dont want the table to mess things while we update it, so que block
    its signals (this is caused because we edited "multipleItemsChanged"). */
    ui.tableWidget_metadata->blockSignals(true);

    /* Lock the playlist so we can work with it */
    playlist_Lock(THEPL);

    /* NOTE: Since the playlist usage is confusing, here is what I understood:
    --The number of elements playing is THEPL->items.i_size-5
    --Items currently playing start on position 8 and finish at sizeOfPlaylist+8 ('playlist_CurrentSize(THEPL)' only returns 0 or 1) */
    int sizeOfPlaylist = THEPL->items.i_size - 5;
    int whereTheLoadedItemsStart = 8;
    int whereTheLoadedItemsFinish = sizeOfPlaylist + 8;

    /* This will be used to show a warning if nothing is loaded */
    bool itemsWereLoaded = false;

    /* If the playlist is NOT empty, load it's info */
    if( sizeOfPlaylist !=0 )
    {
        input_item_t *p_item;  //This is where each item will be stored
        char *uri_text; //This is where each item's URI will be stored
        input_item_t *p_item_duplicate; //This is where duplicated items will be stored

        /* Go through the playlist */
        for(int i = whereTheLoadedItemsStart;  i < whereTheLoadedItemsFinish; i++)
        {
            /* Get the current playlist_item's input_item_t */
            p_item = playlist_ItemGetById(THEPL, i)->p_input;

            /* Evaluate if it's an audio file */
            if (isAudioFile(input_item_GetURI(p_item)))
            {
                //Create a duplicate to avoid loosing object when playlist is cleared
                uri_text = input_item_GetURI(p_item);
                p_item_duplicate = getItemFromURI(uri_text);

                /* Add item to the table */
                addTableEntry(p_item_duplicate);

                itemsWereLoaded = true;
            }
        }

        /* If table is not empty, prepare it */
        if (ui.tableWidget_metadata->rowCount()>0)
        {
            /* Select the first cell and update artwork label */
            ui.tableWidget_metadata->setCurrentCell(0,COL_TITLE);
            //updateArtwork(0,0); //TODO: this creates seg. fault
        }
    }

    /* If playlist was empty, show warning */
    if (!itemsWereLoaded)
    {
        emptyPlaylistDialog();
    }

    /* We have finished, so we unlock all the table's signals. */
    ui.tableWidget_metadata->blockSignals(false);

    /* Always unlock the playlist */
    playlist_Unlock(THEPL);
}

/* Loads files into the table from a file explorer window */
void ExtMetaManagerDialog::getFromFolder()
{
    msg_Dbg( p_intf, "[EMM_Dialog] getFromFolder" );

    /* Hide */
    toggleVisible();

    /* Open a file explorer just with audio files */
    QStringList uris = THEDP->showSimpleOpen(
        qtr("Open audio files to manage"),
        EXT_FILTER_AUDIO,
        p_intf->p_sys->filepath );

    /* Show again */
    toggleVisible();

    /* If no files selected, finish */
    if( uris.isEmpty() ) return;

    /* Clean before changing the workspace */
    if (isClearSelected()){
        cleanUp();
    }

    /* We dont want the table to mess things while we update it, so que block
    its signals (this is caused because we edited "multipleItemsChanged"). */
    ui.tableWidget_metadata->blockSignals(true);

    foreach( const QString &uri, uris )
    {
        if (isAudioFile(uri.toLatin1().constData()))
        {
            // Get the item from the URI
            input_item_t *p_item = getItemFromURI(uri.toLatin1().constData());

            addTableEntry(p_item); //Add the item to the table
        }
    }

    /* If table is not empty, prepare it */
    if (ui.tableWidget_metadata->rowCount()>0)
    {
        /* Select the first cell and update artwork label */
        ui.tableWidget_metadata->setCurrentCell(0,COL_TITLE);
        //updateArtwork(0,0); //TODO: this creates seg. fault
    }

    /* We have finished, so we unlock all the table's signals. */
    ui.tableWidget_metadata->blockSignals(false);
}

/* Initiates the metadata search and analysis based on choosed options. It just
redirects to other methods depending on the activated checkboxes */
void ExtMetaManagerDialog::searchNow()
{
    msg_Dbg( p_intf, "[EMM_Dialog] searchNow" );


    if (ui.checkBox_disableFastSearch->isChecked())
    {
        fingerprintTable(false);
    }
    else
    {
        fingerprintTable(true);
    }
}

/* Saves changes permanently*/
void ExtMetaManagerDialog::saveAll()
{
    msg_Dbg( p_intf, "[EMM_Dialog] saveAll" );

    input_item_t *p_item; // This is where the current working item will be
    const char *title_text;
    const char *artist_text;
    const char *album_text;
    const char *genre_text;
    const char *trackNum_text;
    const char *publisher_text;
    const char *copyright_text;

    /* Iterate over all the items on the table */
    int rows = ui.tableWidget_metadata->rowCount();

    for(int row = 0;  row < rows; row++) //Iterate over the table
    {
        /* Check if the row is checked/selected and ignore if not */
        if (rowIsSelected(row))
        {
            p_item = getItemFromRow(row); //First we obtain the input_item from the row

            /* ---Extract information form the table---
            getting the reference to one columns info is tricky, because the
            pointer is only valid while the QString (got with text()) hasn't
            changed. This means every time you get info from a new row, you
            loose the pointers to the previous row. That's why here we get the
            info and store it inmediately*/
            title_text = ui.tableWidget_metadata->item(row,COL_TITLE)->text().toLocal8Bit().constData();
            input_item_SetTitle(p_item, title_text);
            artist_text = ui.tableWidget_metadata->item(row,COL_ARTIST)->text().toLocal8Bit().constData();
            input_item_SetArtist(p_item, artist_text);
            album_text = ui.tableWidget_metadata->item(row,COL_ALBUM)->text().toLocal8Bit().constData();
            input_item_SetAlbum(p_item, album_text);
            genre_text = ui.tableWidget_metadata->item(row,COL_GENRE)->text().toLocal8Bit().constData();
            input_item_SetGenre(p_item, genre_text);
            trackNum_text = ui.tableWidget_metadata->item(row,COL_TRACKNUM)->text().toLocal8Bit().constData();
            input_item_SetTrackNum(p_item, trackNum_text);
            publisher_text = ui.tableWidget_metadata->item(row,COL_PUBLISHER)->text().toLocal8Bit().constData();
            input_item_SetPublisher(p_item, publisher_text);
            copyright_text = ui.tableWidget_metadata->item(row,COL_COPYRIGHT)->text().toLocal8Bit().constData();
            input_item_SetCopyright(p_item, copyright_text);

            // Save the meta on the item
            input_item_WriteMeta( VLC_OBJECT(THEPL), p_item ); // Write the metas
        }
    }
}

/* Just reloads all the items of the table, discarding unsaved changes */
void ExtMetaManagerDialog::restoreAll()
{
    msg_Dbg( p_intf, "[EMM_Dialog] restoreAll" );

    clearTable();

    // input_item_t *p_item; // This is where the current working item will be

    /* Iterate over all the items in the workspace */
    int arraySize = vlc_array_count(workspace);
    for(int i = 0; i < arraySize; i++)
    {
        /* Get one item from the "workspace", get it's URI, create a new
        input_item_t and add it to the table (we do this because we want to
        fetch the original metadata again)*/
        input_item_t *p_item_old = (input_item_t*)vlc_array_item_at_index(workspace, i);
        char *uri_text = input_item_GetURI(p_item_old);
        input_item_t *p_item_new = getItemFromURI(uri_text);

        addTableEntry(p_item_new);
    }
}

/*----------------------------------------------------------------------------*/
/*---------------------------Fingerpinting------------------------------------*/
/*----------------------------------------------------------------------------*/

/* Initiates the fingerprint process for all the table. If "fast" is true, 1st
entry is applied automatically */
void ExtMetaManagerDialog::fingerprintTable( bool fast )
{
    msg_Dbg( p_intf, "[EMM_Dialog] fingerprintTable" );

    input_item_t *p_item; // This is where the current working item will be

    /* Get the number of items we'll be working with and if there are no
    items, finish */
    int rows = ui.tableWidget_metadata->rowCount();
    int selectedRowsCount=0;

    for(int row = 0;  row < rows; row++) //Iterate over table
    {
        /* Check if the row is checked/selected and ignore if not */
        if (rowIsSelected(row)){
            selectedRowsCount++;
        }
    }
    if (selectedRowsCount == 0)
        return;

    /* Calculate how much the progress bar will advance each step (progressBar
    goes from 0 to 100). Then progress variable is set to 0 and the widget is
    updated */
    int progress_unit= 100/selectedRowsCount;
    int progress=0;
    ui.progressBar_search->setValue(progress);

    /* We dont want the table to mess things while we update it, so que block
    its signals (this is caused because we edited "multipleItemsChanged"). */
    ui.tableWidget_metadata->blockSignals(true);

    /* if fast search is activated, initilize custom fingerprinter */
    if (fast)
    {
        t = new (std::nothrow) Chromaprint( p_intf );
        if ( !t )
        {
            return; // Error
        }
    }

    /* Iterate the table */
    for(int row = 0; row < rows; row++)
    {
        /* Check if the row is checked/selected and ignore if not */
        if (rowIsSelected(row))
        {
            /* Get the item from the current row */
            p_item = getItemFromRow(row);

            /* Fingerprint the item and wait for results */
            fingerprint(p_item, fast);

            /* Update the table with the new info */
            updateTableEntry(p_item, row);

            /* Update the progress bar */
            progress=progress+progress_unit; // Increase the progress
            ui.progressBar_search->setValue(progress); // Update the progressBar
        }
    }

    /* Lost decimals can cause the progress bar to not reach 100, so here is
    the fix */
    ui.progressBar_search->setValue(100); //
    ui.progressBar_search->setEnabled(false);

    /* If fast search is activated, delete the custom fingerprinter */
    if (fast)
    {
        if ( t ) delete t;
        if ( p_r ) fingerprint_request_Delete( p_r );
    }

    /* We have finished, so we unlock all the table's signals. */
    ui.tableWidget_metadata->blockSignals(false);
}

/* Initiates the fingerprint process just for one item. If "fast" is true, 1st
entry is applied automatically */
void ExtMetaManagerDialog::fingerprint(input_item_t *p_item, bool fast)
{
    msg_Dbg( p_intf, "[EMM_Dialog] fingerprint" );

    if (fast)
    {
        /* Add the item to the finperprinter's queue */
        if ( t )
            t->enqueue( p_item );

        /* Wait for results */
        p_r = t->fetchResults();
        while (!p_r)
            p_r = t->fetchResults();

        /* Check if metadata was found and if not, exit */
        if ( vlc_array_count( & p_r->results.metas_array ) == 0 )
            return;

        /* Apply first option */
        t->apply( p_r, 0 );
    }
    else
    {
        //Create a fingerprint dialog and wait until it's closed
        FingerprintDialog dialog(this, p_intf, p_item);
        dialog.exec();
    }

}

/*----------------------------------------------------------------------------*/
/*------------------------------Item management-------------------------------*/
/*----------------------------------------------------------------------------*/

/* Recovers the item on a certain row (from the table) */
input_item_t* ExtMetaManagerDialog::getItemFromRow(int row)
{
    msg_Dbg( p_intf, "[EMM_Dialog] getItemFromRow" );

    /* Retrieve URI from the wanted row */
    const char * wantedUri = ui.tableWidget_metadata->item(row,COL_PATH)->text().toLocal8Bit().constData();

    /* Get size of "workspace" and travel through it */
    int arraySize = vlc_array_count(workspace);
    for(int i = 0; i < arraySize; i++)
    {
        /* Get item on position "i" */
        input_item_t *p_item = (input_item_t*)vlc_array_item_at_index(workspace, i);

        /* Get uri from that item */
        char * temp_uri = input_item_GetURI(p_item);

        /* Compare it to "wantedUri". If true, means it's que item we wanted, so return it. */
        if (strcmp(temp_uri,wantedUri) == 0){
            return p_item;
        }

    }

    //TODO: return error
}

/* Gets an item from an URI and preparses it (gets it's metadata) */
input_item_t* ExtMetaManagerDialog::getItemFromURI(const char* uri)
{
    msg_Dbg( p_intf, "[EMM_Dialog] getItemFromURI" );

    /* Create the item from the given uri */
    input_item_t *p_item = input_item_New( uri, "Entry" ); //The name is not relevant

    /* Preparse the item (get the metadata ) */
    libvlc_MetadataRequest(THEPL->obj.libvlc, p_item, META_REQUEST_OPTION_SCOPE_ANY,-1, NULL );

    /* Wait until already preparsed (metadata was added) */
    while (!input_item_IsPreparsed(p_item)) ;

    return p_item;
}

/* Evaluate if file is a valid audio file or not */
bool ExtMetaManagerDialog::isAudioFile(const char* uri)
{
    msg_Dbg( p_intf, "[EMM_Dialog] isAudioFile" );

    /* The following section extracts the extension from the uri */
    char extension[255];
    memset(extension, 0, sizeof extension);
    for (size_t i = 0; i < strlen(uri); i++){
        if (uri[i]=='.')
        {
            memset(extension, 0, sizeof extension);;
        }
        else
        {
            int len = strlen(extension);
            extension[len] = uri[i];
            extension[len+1] = '\0';
        }
    }

    char temp[255];
    memset(temp, 0, sizeof temp);

    /* Now look for all the audio extension available */
    for (size_t i = 0; i < strlen(EXTENSIONS_AUDIO); i++){
        /* End of an extension, compare our file's extension to the found one
        and reset temp */
        if (EXTENSIONS_AUDIO[i]=='.')
        {
            /* If our file's extension is valid, return true */
            if (strcmp(extension,temp) == 0)
            {
                msg_Dbg( p_intf, "[EMM_Dialog] file IS audio" );
                return true;
            }

            /* Reset "temp" */
            memset(temp, 0, sizeof temp);
        }
        /* If charater is found (not separator), add it to the current extension */
        else if (EXTENSIONS_AUDIO[i] != '*' && EXTENSIONS_AUDIO[i] != ';')
        {
            int len = strlen(temp);
            temp[len] = EXTENSIONS_AUDIO[i];
            temp[len+1] = '\0';
        }
    }

    /* If our file's extension is invalid, return false */
    msg_Dbg( p_intf, "[EMM_Dialog] file is NOT audio" );
    return false;
}

void ExtMetaManagerDialog::previewItem(int item_index){
    msg_Dbg( p_intf, "[EMM_Dialog] previewItem" );
    input_item_t *p_item = getItemFromRow(item_index);

    //TODO: this creates seg. fault when loading playlist after using this method
    playlist_Clear( THEPL, false ); //Clear playlist
    playlist_AddInput( THEPL, p_item, true, true ); //Add our item
    THEMIM->play(); //Start playback
}

/*----------------------------------------------------------------------------*/
/*--------------------------Table management----------------------------------*/
/*----------------------------------------------------------------------------*/

/* Modify the table's behavior so multiple items can be edited at the same time
when more than one cell is selected. */
void ExtMetaManagerDialog::multipleItemsChanged( QTableWidgetItem *item )
{
    // msg_Dbg( p_intf, "[EMM_Dialog] multipleItemsChanged" ); //Too many messages on the console

    ui.tableWidget_metadata->blockSignals(true);
	QList<QTableWidgetItem*> selectedItems = ui.tableWidget_metadata->selectedItems();
	foreach(QTableWidgetItem* selectItem, selectedItems)
	{
		selectItem->setText(item->text());
	}
	ui.tableWidget_metadata->blockSignals(false);
}

/* Adds a row on the table with the metadata from a given item */
void ExtMetaManagerDialog::addTableEntry(input_item_t *p_item)
{
    msg_Dbg( p_intf, "[EMM_Dialog] addTableEntry" );

    //Check if item is already on the table
    if (checkRepeatedItem(p_item)){
        return; //Item was already loaded
    }

    /* Add one row to the table Initilize */
    int rowToAdd = ui.tableWidget_metadata->rowCount(); //Last row is always rowCount-1
    ui.tableWidget_metadata->insertRow(rowToAdd);

    /* Add item to our workspace (to track repeated files) */
    vlc_array_insert(workspace, p_item, rowToAdd);

    /* Create checkbox for the first column and set it as checked (and set it's tip). */
    QCheckBox *checkbox = new QCheckBox ();
    checkbox->setChecked(1);
    checkbox->setToolTip(checkbox_tip);
    /* Insert the checkbox in the cell */
    ui.tableWidget_metadata->setCellWidget(rowToAdd, COL_CHECKBOX, checkbox );


    /* Create button for the artwork update (and set it's tip). We don't add
    it to the UI yet, it will be added in each row later */
    QPushButton *button_changeArtwork = new QPushButton( qtr("Change Art"), this);
    button_changeArtwork->setToolTip(artworkButton_tip);

    /* Prepare the mapping with the row and connect the button to it.
    The mapper is used to be able to know from which row is the button is
    being clicked */
    changeArtwork_SignalMapper.setMapping(button_changeArtwork, rowToAdd);
    connect(button_changeArtwork, SIGNAL(clicked()), &changeArtwork_SignalMapper, SLOT(map()));

    /* Insert the button in the cell (we have created it on the constructor) */
    ui.tableWidget_metadata->setCellWidget(rowToAdd, COL_ARTWORK, button_changeArtwork );


    /* Create button for the artwork update (and set it's tip). We don't add
    it to the UI yet, it will be added in each row later */
    QPushButton *button_previewItem = new QPushButton( qtr("Preview"), this);
    button_previewItem->setToolTip(previewButton_tip);

    /* Prepare the mapping with the row and connect the button to it.
    The mapper is used to be able to know from which row is the button is
    being clicked */
    preview_SignalMapper.setMapping(button_previewItem, rowToAdd);
    connect(button_previewItem, SIGNAL(clicked()), &preview_SignalMapper, SLOT(map()));

    /* Insert the button in the cell (we have created it on the constructor) */
    ui.tableWidget_metadata->setCellWidget(rowToAdd, COL_PREVIEW, button_previewItem );

    /* Now that we have the row ready, we load the metadata using "updateTableEntry" */
    updateTableEntry(p_item,rowToAdd);
}

/* Updates/Writes a (given) row on the table with one itme's metadata */
void ExtMetaManagerDialog::updateTableEntry(input_item_t *p_item, int row)
{
    msg_Dbg( p_intf, "[EMM_Dialog] updateTableEntry (with row)" );

    /* Get metadata information from item */
    char *title_text = input_item_GetTitle(p_item);
    char *artist_text = input_item_GetArtist(p_item);
    char *album_text = input_item_GetAlbum(p_item);
    char *genre_text = input_item_GetGenre(p_item);
    char *trackNum_text = input_item_GetTrackNum(p_item);
    char *publisher_text = input_item_GetPublisher(p_item);
    char *copyright_text = input_item_GetCopyright(p_item);
    char *uri_text = input_item_GetURI(p_item);

    /* Fill all the cells with the extracted info */
    ui.tableWidget_metadata->setItem(row, COL_TITLE, new QTableWidgetItem( title_text ));
    ui.tableWidget_metadata->setItem(row, COL_ARTIST, new QTableWidgetItem( artist_text ));
    ui.tableWidget_metadata->setItem(row, COL_ALBUM, new QTableWidgetItem( album_text ));
    ui.tableWidget_metadata->setItem(row, COL_GENRE, new QTableWidgetItem( genre_text ));
    ui.tableWidget_metadata->setItem(row, COL_TRACKNUM, new QTableWidgetItem( trackNum_text ));
    ui.tableWidget_metadata->setItem(row, COL_PUBLISHER, new QTableWidgetItem( publisher_text ));
    ui.tableWidget_metadata->setItem(row, COL_COPYRIGHT, new QTableWidgetItem( copyright_text ));
    ui.tableWidget_metadata->setItem(row, COL_PATH, new QTableWidgetItem( uri_text ));
    ui.tableWidget_metadata->item(row, COL_PATH)->setFlags(0); // Make the path not selectable/editable
}

/* Returns a true if that row is seleted (checkbox is seletedc) and a flase if not */
bool ExtMetaManagerDialog::rowIsSelected(int row)
{
    msg_Dbg( p_intf, "[EMM_Dialog] rowIsSelected" );

    /* Check if the row's is checkbox checked */
    QCheckBox  *checkbox = (QCheckBox*) ui.tableWidget_metadata->cellWidget(row,COL_CHECKBOX);
    return checkbox->isChecked();
}

/* Deletes all entries from the table (still can be recovered with restoreAll) */
void ExtMetaManagerDialog::clearTable()
{
    msg_Dbg( p_intf, "[EMM_Dialog] clearTable" );

    ui.tableWidget_metadata->clearContents();
    ui.tableWidget_metadata->setRowCount(0);
    art_cover->clear();
}

/*----------------------------------------------------------------------------*/
/*----------------------------Artwork management------------------------------*/
/*----------------------------------------------------------------------------*/

/* When a cell on the table is selected, this function changes the Artwork
label's content to the selected item's artwork */
void ExtMetaManagerDialog::updateArtwork(int row, int column)
{
    msg_Dbg( p_intf, "[EMM_Dialog] updateArtwork" );

    UNUSED(column); //FIXME: delete this, jus avoids a warning
    /* Get the item form the row, decode it's Artwork and update it in the UI */
    art_cover->showArtUpdate( getItemFromRow(row) );
}

/* Change the artwork of the currently selected item */
void ExtMetaManagerDialog::changeArtwork(int row)
{
    msg_Dbg( p_intf, "[EMM_Dialog] changeArtwork" );

    /* Fix to select the row is the button being clicked and select it's cover */
    ui.tableWidget_metadata->setCurrentCell(row, COL_ARTWORK);
    updateArtwork(0, 0);

    art_cover->setArtFromFile();
}

/*----------------------------------------------------------------------------*/
/*-------------------------------Others---------------------------------------*/
/*----------------------------------------------------------------------------*/

/* Cleans the playlist, clears the table, empties workspace */
void ExtMetaManagerDialog::cleanUp()
{
    msg_Dbg( p_intf, "[EMM_Dialog] cleanUp" );

    /* Clear the table, the playlist (if files have been loaded to it) and the
    array with with the currently working items*/
    clearTable();
    vlc_array_clear(workspace); // This last or the previous ones won't work
}

/* Launches the "Help" dialog */
void ExtMetaManagerDialog::helpDialog()
{
    msg_Dbg( p_intf, "[EMM_Dialog] helpDialog" );

    QMessageBox::information(
      this,
      qtr("Help - Extended Metadata Manager"), //Title
      help_text ); //Text
}

/* Launches the "About" dialog */
void ExtMetaManagerDialog::aboutDialog()
{
    msg_Dbg( p_intf, "[EMM_Dialog] aboutDialog" );

    QMessageBox::information(
      this,
      qtr("About - Extended Metadata Manager"), //Title
      about_text ); //Text
}

/* Launches the "Help" dialog */
void ExtMetaManagerDialog::emptyPlaylistDialog()
{
    msg_Dbg( p_intf, "[EMM_Dialog] emptyPlaylistDialog" );

    QMessageBox::information(
      this,
      qtr("Playlist empty! - Extended Metadata Manager"), //Title
      emptyPlaylist_text ); //Text
}

bool ExtMetaManagerDialog::isClearSelected()
{
    return ui.checkBox_cleaTable->isChecked();
}

//TODO: this code may be repeated on other branches
bool ExtMetaManagerDialog::checkRepeatedItem(input_item_t *new_item){
    msg_Dbg( p_intf, "[EMM_Dialog] checkRepeatedItem" );

    /* Get uri from that "new_item" */
    char * wantedUri = input_item_GetURI(new_item);

    input_item_t *p_item;
    char * temp_uri;

    int arraySize = vlc_array_count(workspace);
    for(int i = 0; i < arraySize; i++)
    {
        /* Get item on position "i" */
        p_item = (input_item_t*)vlc_array_item_at_index(workspace, i);

        /* Get uri from that item */
        temp_uri = input_item_GetURI(p_item);

        /* Compare it to "wantedUri". If true, means it's que item is repeated, so return true. */
        if (strcmp(temp_uri,wantedUri) == 0){
            msg_Dbg( p_intf, "[EMM_Dialog] repeated item found, ignoring it." );
            return true;
        }

    }
    return false; //No matches found, so item is not already loaded.
}
