/*****************************************************************************
 * extendedmetamanager.cpp : Extended Metadata Manager dialogs
 ****************************************************************************
 * Copyright (C) 2017 Asier Santos Valcárcel
 * Authors: Asier Santos Valcarcel
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
#include "components/interface_widgets.hpp"     // CoverArtLabelExt
#include "dialogs/fingerprintdialog.hpp"        // fingerprinting dialog
#include "adapters/chromaprint.hpp"             // fingerprinting adapter (no UI)

#include <QMessageBox>

#define UNUSED(x) (void)(x) //FIXME: delete this. Unused variable warning removal

ExtMetaManagerDialog::ExtMetaManagerDialog( intf_thread_t *_p_intf)
               : QVLCDialog( (QWidget*)_p_intf->p_sys->p_mi, _p_intf )
{
    msg_Dbg( p_intf, "[EMM_Dialog] Initializing" );
    configureWindow();
    initializeWorkspace();
    QVLCTools::restoreWidgetPosition( p_intf, "ExtMetaManagerDialog", this );
}

ExtMetaManagerDialog::~ExtMetaManagerDialog() {
    msg_Dbg( p_intf, "[EMM_Dialog] Destroying" );
    QVLCTools::saveWidgetPosition( p_intf, "ExtMetaManagerDialog", this );
}

/* Override the closing (click X) event*/
void ExtMetaManagerDialog::closeEvent(QCloseEvent *event) { //TODO: clean this method
    msg_Dbg( p_intf, "[EMM_Dialog] Close event" );
    UNUSED(event); //FIXME: delete this
    close();
}

/* Show/hide */
void ExtMetaManagerDialog::toggleVisible() { //TODO: clean this method
    msg_Dbg( p_intf, "[EMM_Dialog] toggleVisible" );
    QVLCDialog::toggleVisible();
    if(isVisible()) //If changed to shown
        activateWindow();
}

/*----------------------------------------------------------------------------*/
/*-----------------------------Main use cases---------------------------------*/
/*----------------------------------------------------------------------------*/

/* Just closes the window (and the module itself) */
void ExtMetaManagerDialog::close() { //TODO: clean this method
    toggleVisible();

    /* Clean before closing */
    cleanUp();

    msg_Dbg( p_intf, "[EMM_Dialog] Closing" );
}

/* Loads files into the table from the current playlist */
void ExtMetaManagerDialog::getFromPlaylist() { //TODO: clean this method
    msg_Dbg( p_intf, "[EMM_Dialog] getFromPlaylist" );
    /* Clean before changing the workspace */
    cleanUp();

    /* Lock the playlist so we can work with it */
    playlist_Lock(THEPL);

    /* Get the size of the playlist and if no files selected, finish */
    int size = THEPL->items.i_size;

    /* If the playlist is NOT empty, load it's info */
    if( size !=0 )
    {
        input_item_t *p_item;  //This is where each item will be stored
        int row; //This is where each item's position will be stored

        for(int i = 4;  i <= size+3; i++) //the list starts at 4 because the first 3 are not files
        {
            p_item = playlist_ItemGetById(THEPL, i)->p_input; // Get the playlist_item's input_item_t

            if (isAudioFile(input_item_GetURI(p_item)))
            {
                addTableEntry(p_item); //add item to the table

                /*Now we get the size of the table and store the item on that position
                on the workspace array, so item at row X on the table is also stored at
                array position X*/
                row = ui.tableWidget_metadata->rowCount();
                vlc_array_insert(workspace, p_item, row-1);
            }
        }

        /* If table is not empty, prepare it */
        if (ui.tableWidget_metadata->rowCount()>0)
        {
            /* Select the first cell and update artwork label */
            ui.tableWidget_metadata->setCurrentCell(0,1);
            updateArtwork(0,0);
        }
    }

    /* If table is empty, show warning */
    if (ui.tableWidget_metadata->rowCount()<1){
        launchEmptyPlaylistDialog();
    }

    /* Always unlock the playlist */
    playlist_Unlock(THEPL);
}

/* Loads files into the table from a file explorer window */
void ExtMetaManagerDialog::getFromFolder() { //TODO: clean this method
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
    cleanUp();

    int row; //This is where each item's position will be stored

    foreach( const QString &uri, uris )
    {
        if (isAudioFile(uri.toLatin1().constData()))
        {
            // Get the item from the URI
            input_item_t *p_item = createItemFromURI(uri.toLatin1().constData());

            addTableEntry(p_item); //Add the item to the table

            /*Now we get the size of the table and store the item on that position
            on the workspace array, so item at row X on the table is also stored at
            array position X*/
            row = ui.tableWidget_metadata->rowCount();
            vlc_array_insert(workspace, p_item, row-1);
        }
    }
    /* If table is not empty, prepare it */
    if (ui.tableWidget_metadata->rowCount()>0)
    {
        /* Select the first cell and update artwork label */
        ui.tableWidget_metadata->setCurrentCell(0,1);
        updateArtwork(0,0);
    }
}

void ExtMetaManagerDialog::initiateMetadataSearch() {
    bool isFastSearch;
    if (ui.checkBox_disableFastSearch->isChecked()) {
        isFastSearch = false;
    } else {
        isFastSearch = true;
    }
    fingerprintTable(isFastSearch);
}

void ExtMetaManagerDialog::saveChanges() {
    msg_Dbg( p_intf, "[EMM_Dialog] saveChanges" );

    input_item_t *temp_item;

    int rows = ui.tableWidget_metadata->rowCount();
    for(int row = 0;  row < rows; row++) {
        if (isRowSelected(row)) {
            temp_item = recoverItemFromRow(row);
            saveItemChanges(temp_item, row);
        }
    }
}

void ExtMetaManagerDialog::discardUnsavedChanges() {
    msg_Dbg( p_intf, "[EMM_Dialog] discardUnsavedChanges" );

    clearTable();

    input_item_t *temp_item;

    int workspaceSize = vlc_array_count(workspace);
    for(int i = 0; i < workspaceSize; i++)
    {
        temp_item = (input_item_t*)vlc_array_item_at_index(workspace, i);
        addTableEntry(temp_item);
    }
}

/*----------------------------------------------------------------------------*/
/*---------------------------Fingerpinting------------------------------------*/
/*----------------------------------------------------------------------------*/

/* Initiates the fingerprint process for all the table. If "fast" is true, 1st
entry is applied automatically */
void ExtMetaManagerDialog::fingerprintTable( bool fast ) { //TODO: clean this method
    msg_Dbg( p_intf, "[EMM_Dialog] fingerprintTable" );

    input_item_t *p_item; // This is where the current working item will be

    /* Get the number of items we'll be working with and if there are no
    items, finish */
    int rows = ui.tableWidget_metadata->rowCount();
    int selectedRowsCount=0;

    for(int row = 0;  row < rows; row++) //Iterate over table
    {
        /* Check if the row is checked/selected and ignore if not */
        if (isRowSelected(row)){
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
        if (isRowSelected(row))
        {
            /* Get the item from the current row */
            p_item = recoverItemFromRow(row);

            /* Fingerprint the item and wait for results */
            fingerprintItem(p_item, fast);

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
void ExtMetaManagerDialog::fingerprintItem(input_item_t *p_item, bool fast) { //TODO: clean this method
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

input_item_t* ExtMetaManagerDialog::recoverItemFromRow(int row) { //TODO: clean this method
    /* Item at row X is stored at workspace postion X */
    input_item_t *p_item = (input_item_t*)vlc_array_item_at_index(workspace, row);
    return p_item;
}

/* Gets an item from an URI and preparses it (gets it's metadata) */
input_item_t* ExtMetaManagerDialog::createItemFromURI(const char* uri) { //TODO: clean this method
    msg_Dbg( p_intf, "[EMM_Dialog] createItemFromURI" );

    input_item_t *p_item = input_item_New( uri, "" ); //The name is not relevant

    /* Preparse the item (get the metadata ) */
    libvlc_MetadataRequest(THEPL->obj.libvlc, p_item, META_REQUEST_OPTION_SCOPE_ANY,-1, NULL );
    while (!input_item_IsPreparsed(p_item)) ;

    return p_item;
}

bool ExtMetaManagerDialog::isAudioFile(const char* uri) { //TODO: clean this method
    // The following section extracts the extension from the uri
    char extension[255];
    memset(extension, 0, sizeof extension);
    for (size_t i = 0; i < strlen(uri); i++){
        if (uri[i]=='.'){
            memset(extension, 0, sizeof extension);;
        }else{
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
    return false;
}

void ExtMetaManagerDialog::saveItemChanges( input_item_t *p_item, int rowFrom) {
    const char *title_text;
    const char *artist_text;
    const char *album_text;
    const char *genre_text;
    const char *trackNum_text;
    const char *publisher_text;
    const char *copyright_text;

    /* ---Extract information form the table---
    getting the reference to one columns info is tricky, because the
    pointer is only valid while the QString (got with text()) hasn't
    changed. This means every time you get info from a new row, you
    loose the pointers to the previous row. That's why here we get the
    info and store it inmediately*/
    title_text = ui.tableWidget_metadata->item(rowFrom,COL_TITLE)->text().toLocal8Bit().constData();
    input_item_SetTitle(p_item, title_text);
    artist_text = ui.tableWidget_metadata->item(rowFrom,COL_ARTIST)->text().toLocal8Bit().constData();
    input_item_SetArtist(p_item, artist_text);
    album_text = ui.tableWidget_metadata->item(rowFrom,COL_ALBUM)->text().toLocal8Bit().constData();
    input_item_SetAlbum(p_item, album_text);
    genre_text = ui.tableWidget_metadata->item(rowFrom,COL_GENRE)->text().toLocal8Bit().constData();
    input_item_SetGenre(p_item, genre_text);
    trackNum_text = ui.tableWidget_metadata->item(rowFrom,COL_TRACKNUM)->text().toLocal8Bit().constData();
    input_item_SetTrackNum(p_item, trackNum_text);
    publisher_text = ui.tableWidget_metadata->item(rowFrom,COL_PUBLISHER)->text().toLocal8Bit().constData();
    input_item_SetPublisher(p_item, publisher_text);
    copyright_text = ui.tableWidget_metadata->item(rowFrom,COL_COPYRIGHT)->text().toLocal8Bit().constData();
    input_item_SetCopyright(p_item, copyright_text);

    // Save the metas on the item
    input_item_WriteMeta( VLC_OBJECT(THEPL), p_item );
}

/*----------------------------------------------------------------------------*/
/*--------------------------Table management----------------------------------*/
/*----------------------------------------------------------------------------*/

/* Modify the table's behavior so multiple items can be edited at the same time
when more than one cell is selected. */
void ExtMetaManagerDialog::multipleItemsChanged( QTableWidgetItem *item ) { //TODO: clean this method
    ui.tableWidget_metadata->blockSignals(true);
    QList<QTableWidgetItem*> selectedItems = ui.tableWidget_metadata->selectedItems();
    foreach(QTableWidgetItem* selectItem, selectedItems)
    {
        selectItem->setText(item->text());
    }
    ui.tableWidget_metadata->blockSignals(false);
}

/* Adds a row on the table with the metadata from a given item */
void ExtMetaManagerDialog::addTableEntry(input_item_t *p_item) { //TODO: clean this method
    msg_Dbg( p_intf, "[EMM_Dialog] addTableEntry" );

    /* Add one row to the table Initilize */
    int row = ui.tableWidget_metadata->rowCount(); //Last row is always rowCount-1
    ui.tableWidget_metadata->insertRow(row);

    /* Create checkbox for the first column and set it as checked(and set it's tip). */
    QCheckBox *checkbox = new QCheckBox ();
    checkbox->setChecked(1);
    checkbox->setToolTip(checkbox_tip);
    /* Insert the checkbox in the cell */
    ui.tableWidget_metadata->setCellWidget(row, COL_CHECKBOX, checkbox );

    /* Create button for the artwork update (and set it's tip). We don't add
    it to the UI yet, it will be added in each row later */
    QPushButton *button_changeArtwork = new QPushButton( qtr("Change"), this);
    button_changeArtwork->setToolTip(artworkButton_tip);

    /* Prepare the mapping with the row and connect the button to it.
    The mapper is used to be able to know from which row is the button is
    being clicked */
    ButtonSignalMapper.setMapping(button_changeArtwork, row);
    connect(button_changeArtwork, SIGNAL(clicked()), &ButtonSignalMapper, SLOT(map()));

    /* Insert the button in the cell (we have created it on the constructor) */
    ui.tableWidget_metadata->setCellWidget(row, COL_ARTWORK, button_changeArtwork );

    updateTableEntry(p_item,row);
}

// Updates/Writes a (given) row on the table with one item's metadata
void ExtMetaManagerDialog::updateTableEntry(input_item_t *p_item, int row) {
    msg_Dbg( p_intf, "[EMM_Dialog] updateTableEntry" );

    // Get metadata information from item
    char *title_text = input_item_GetTitle(p_item);
    char *artist_text = input_item_GetArtist(p_item);
    char *album_text = input_item_GetAlbum(p_item);
    char *genre_text = input_item_GetGenre(p_item);
    char *trackNum_text = input_item_GetTrackNum(p_item);
    char *publisher_text = input_item_GetPublisher(p_item);
    char *copyright_text = input_item_GetCopyright(p_item);
    char *uri_text = input_item_GetURI(p_item);

    // Fill all the cells with the extracted info
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

bool ExtMetaManagerDialog::isRowSelected(int row) {
    QCheckBox *checkbox = (QCheckBox*) ui.tableWidget_metadata->cellWidget(row,COL_CHECKBOX);
    return checkbox->isChecked();
}

/* Deletes all entries from the table (still can be recovered with discardUnsavedChanges) */
void ExtMetaManagerDialog::clearTable() { //TODO: clean this method
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
void ExtMetaManagerDialog::updateArtwork(int row, int column) { //TODO: clean this method
    msg_Dbg( p_intf, "[EMM_Dialog] updateArtwork" );

    UNUSED(column); //FIXME: delete this
    /* Get the item form the row, decode it's Artwork and update it in the UI */
    art_cover->showArtUpdate( recoverItemFromRow(row) );
}

/* Change the artwork of the currently selected item */
void ExtMetaManagerDialog::changeArtwork(int row) { //TODO: clean this method
    msg_Dbg( p_intf, "[EMM_Dialog] changeArtwork" );

    /* Fix to select the row is the button being clicked and select it's cover */
    ui.tableWidget_metadata->setCurrentCell(row, COL_ARTWORK);
    updateArtwork(row, COL_ARTWORK);

    art_cover->setArtFromFile();
}

/*----------------------------------------------------------------------------*/
/*------------------------- UI configuration ---------------------------------*/
/*----------------------------------------------------------------------------*/

void ExtMetaManagerDialog::configureWindow(){
    configureBasicUI();
    configureButtons();
    configureTable();
    configureArtworkLabel();
    configureProgressBar();
    setToolTips();
}

void ExtMetaManagerDialog::configureBasicUI() {
    ui.setupUi( this ); //setup the UI from de compiled (.h) version of de QT ui (.ui)
    setWindowFlags( Qt::Tool );
    setWindowRole( "vlc-ext-meta-manager" );
    setWindowTitle( qtr( "Extended Metadata Manager" ) );
}

void ExtMetaManagerDialog::configureTable() {
    setTableEvents();
    setColumnSizes();

    /* Set the mapper's connection to changeArtwork. This is used to be able
    to know from which row the button (added later on each row) it's being
    clicked */
    connect(&ButtonSignalMapper, SIGNAL(mapped(int)), this, SLOT(changeArtwork(int)));

}

void ExtMetaManagerDialog::configureButtons() {
    setButtonIcons();
    setButtonBindings();
}

void ExtMetaManagerDialog::configureArtworkLabel() {
    art_cover = new CoverArtLabelExt( this, p_intf );
    ui.gridLayout_artwork->layout()->addWidget(art_cover);
}

void ExtMetaManagerDialog::configureProgressBar() {
    ui.progressBar_search->setEnabled(false);
}

void ExtMetaManagerDialog::setTableEvents() {
    CONNECT( ui.tableWidget_metadata, cellClicked(int, int), this, updateArtwork(int, int) );
    CONNECT( ui.tableWidget_metadata,  itemChanged(QTableWidgetItem*), this,  multipleItemsChanged(QTableWidgetItem*));
}

void ExtMetaManagerDialog::setColumnSizes() {
    ui.tableWidget_metadata->setColumnWidth(COL_CHECKBOX, 30);
    ui.tableWidget_metadata->setColumnWidth(COL_TITLE, 200);
    ui.tableWidget_metadata->setColumnWidth(COL_ARTIST, 200);
    ui.tableWidget_metadata->setColumnWidth(COL_ALBUM, 200);
    ui.tableWidget_metadata->setColumnWidth(COL_GENRE, 120);
    ui.tableWidget_metadata->setColumnWidth(COL_TRACKNUM, 70);
    ui.tableWidget_metadata->setColumnWidth(COL_PUBLISHER, 120);
    ui.tableWidget_metadata->setColumnWidth(COL_COPYRIGHT, 120);
    ui.tableWidget_metadata->setColumnWidth(COL_ARTWORK, 70);
    ui.tableWidget_metadata->setColumnWidth(COL_PATH, 50);
}

void ExtMetaManagerDialog::setButtonIcons() {
    // Set icons
    ui.pushButton_getFromPlaylist->setIcon(QIcon(QPixmap (":/toolbar/playlist") ) );
    ui.pushButton_getFromFolder->setIcon(QIcon(QPixmap (":/type/folder-grey") ) );
    ui.pushButton_help->setIcon(QIcon(QPixmap (":/menu/help") ) );
    ui.pushButton_about->setIcon(QIcon(QPixmap (":/menu/info") ) );
    ui.pushButton_searchNow->setIcon(QIcon(QPixmap (":/ext-meta-manager/search") ) );
    ui.pushButton_saveAll->setIcon(QIcon(QPixmap (":/ext-meta-manager/save") ) );
    ui.pushButton_restoreAll->setIcon(QIcon(QPixmap (":/buttons/playlist/repeat_all") ) );
    ui.pushButton_clearTable->setIcon(QIcon(QPixmap (":/ext-meta-manager/clean") ) );
    ui.pushButton_cancel->setIcon(QIcon(QPixmap (":/menu/exit") ) );
}

void ExtMetaManagerDialog::setButtonBindings() {
    BUTTONACT( ui.pushButton_getFromPlaylist, getFromPlaylist() );
    BUTTONACT( ui.pushButton_getFromFolder, getFromFolder() );
    BUTTONACT( ui.pushButton_help, launchHelpDialog() );
    BUTTONACT( ui.pushButton_about, launchAboutDialog() );
    BUTTONACT( ui.pushButton_searchNow, initiateMetadataSearch() );
    BUTTONACT( ui.pushButton_saveAll, saveChanges() );
    BUTTONACT( ui.pushButton_restoreAll, discardUnsavedChanges() );
    BUTTONACT( ui.pushButton_clearTable, cleanUp() );
    BUTTONACT( ui.pushButton_cancel, close() );
}

void ExtMetaManagerDialog::setToolTips() {
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
}

/*----------------------------------------------------------------------------*/
/*-------------------------------Others---------------------------------------*/
/*----------------------------------------------------------------------------*/

// Cleans the playlist, clears the table, empties workspace
void ExtMetaManagerDialog::cleanUp() { //TODO: clean this method
    msg_Dbg( p_intf, "[EMM_Dialog] cleanUp" );

    clearTable();
    vlc_array_clear(workspace); // This last or the previous ones won't work
}

void ExtMetaManagerDialog::initializeWorkspace(){
    workspace = new vlc_array_t();
    vlc_array_init(workspace);
}

void ExtMetaManagerDialog::launchHelpDialog() {
    QMessageBox::information(
      this,
      help_dialog_title,
      help_dialog_text );
}

void ExtMetaManagerDialog::launchAboutDialog() {
    QMessageBox::information(
      this,
      about_dialog_title,
      about_dialog_text );
}

void ExtMetaManagerDialog::launchEmptyPlaylistDialog() {
    QMessageBox::information(
      this,
      emptyPlaylist_dialog_title,
      emptyPlaylist_dialog_text );
}
